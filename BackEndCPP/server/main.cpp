#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <MMSystem.h>
#include "server.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <list>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

HWAVEOUT hWaveOut;

#define DEFAULT_BUFLEN 88200

//std::vector<int> speakingRateHistory;
//int windowSize = 2;
int lastSpeakingRate = 0;
int lastlastSpeakingRate = 0;
int lastAvgLoud = 0;
int lastlastAvgLoud = 0;

template <typename T>
void write(std::ofstream& stream, const T& t) {
  stream.write((const char*)&t, sizeof(T));
}

template <typename T>
void writeFormat(std::ofstream& stream) {
  write<short>(stream, 1);
}

template <>
void writeFormat<float>(std::ofstream& stream) {
  write<short>(stream, 3);
}

template <typename SampleType>
void writeWAVData(
  char* outFile,
  SampleType* buf,
  size_t bufSize,
  int sampleRate,
  short channels)
{
	std::ofstream stream(outFile, std::ios::out|std::ios::binary);
  stream.write("RIFF", 4);
  write<int>(stream, 36 + bufSize);
  stream.write("WAVE", 4);
  stream.write("fmt ", 4);
  write<int>(stream, 16);
  writeFormat<SampleType>(stream);                                // Format
  write<short>(stream, channels);                                 // Channels
  write<int>(stream, sampleRate);                                 // Sample Rate
  write<int>(stream, sampleRate * 2);			 // Byterate
  write<short>(stream, channels * 2);            // Frame size
  write<short>(stream, 8 * 2);                   // Bits per sample
  stream.write("data", 4);
  stream.write((const char*)&bufSize, 4);
  stream.write((const char*)buf, bufSize);
}

void playAudioBlock(HWAVEOUT hWaveOut, LPSTR block, DWORD size){
	WAVEHDR header;
	/*
	* initialise the block header with the size
	* and pointer.
	*/
	ZeroMemory(&header, sizeof(WAVEHDR));
	header.dwBufferLength = size;
	header.lpData = block;
	/*
	* prepare the block for playback
	*/
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	/*
	* write the block to the device. waveOutWrite returns immediately
	* unless a synchronous driver is used (not often).
	*/
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	/*
	* wait a while for the block to play then start trying
	* to unprepare the header. this will fail until the block has
	* played.
	*/
	Sleep(500);
	while(waveOutUnprepareHeader(
		hWaveOut, 
		&header, 
		sizeof(WAVEHDR)
		) == WAVERR_STILLPLAYING)
		Sleep(100);
}

std::string processReceivedData(std::vector<char>& data){
	// Indicate how many seconds of history will be preserved
	// All the calculations will be done on a time frame of
	// this many seconds
	const int keepHistoryForSec = 2;
	const long bufferSize = 99999;	// a large buffer

	// Temporary variables
	char elements_pitch[bufferSize]; // for pitch
	char elements_loud[bufferSize];  // and for loudness
	std::stringstream allData_pitch;
	std::stringstream allData_loud;
	std::string onePitchStr;
	std::string oneLoudStr;
	double pitch = 0, loud = 0;	int count = 0;

	// These static variables will preserve the history
	static std::list<double> pitchHistory;
	static std::list<double> loudHistory;
	static std::list<int> histItemCount;
	// iterators
	std::list<double>::iterator itd;
	std::list<int>::iterator iti;

	// Get the data. Check its validity. For our context, the data
	// size must not be less than 1(Sec)*44100(Sampling rate)*2(Bytes/Sample)
	// = 88200 bytes
	std::cout<<"Received: "<<data.size()<<" Bytes"<<std::endl;
	if(data.size()<88200){
		// Garbage received
		return "ok";
	}
	
	// save data in file
	writeWAVData<char>("audiochunk.wav", &data[0], data.size(), 44100, 1);
	
	// Analyze audio using PRAAT
	system("praatcon.exe prosody-tanveer.praat audiochunk.wav");
	
	// Read the PRAAT output
	std::fstream ifs_loud("output.loud",std::ios::in);
	std::fstream ifs_pitch("output.pitch",std::ios::in);
	if(ifs_loud.bad() || ifs_pitch.bad()){
		return "ok";
	}

	// Process PRAAT output
	// Read the whole data file add in string stream
	ifs_pitch.getline(elements_pitch,bufferSize);
	ifs_loud.getline(elements_loud,bufferSize);
	allData_pitch << elements_pitch;
	allData_loud << elements_loud;

	// Read each element from the string stream and calculate
	// average loudness and speaking speed
	while(allData_pitch.good() && allData_loud.good()){
		allData_pitch >> onePitchStr;pitch = atof(onePitchStr.c_str());
		allData_loud  >> oneLoudStr;loud = atof(oneLoudStr.c_str());
		
		// Debug: Enable to see the numbers read
		//std::cout<<pitch<<"    "<<loud<<std::endl;		
		
		// Write the history here
		pitchHistory.push_back(pitch);
		loudHistory.push_back(loud);
		count++;
	}
	histItemCount.push_back(count);

	// Now, delete the history beyond memorable limit
	// Remember, the latest one was added last
	if(histItemCount.size()>keepHistoryForSec){
		count = histItemCount.front();histItemCount.pop_front();
		// Delete furthest element in pitch history
		itd = pitchHistory.begin();
		std::advance(itd,count);
		pitchHistory.erase(pitchHistory.begin(),itd);
		// Delete furthest element in loudness history
		itd = loudHistory.begin();
		std::advance(itd,count);
		loudHistory.erase(loudHistory.begin(),itd);
	}

	// Debug: Enable to see history counts
	//std::cout<<"Pitch-History-Count: "<<pitchHistory.size()
	//	<<"Loud-History-Count: "<<loudHistory.size()<<std::endl;

	// Calculate speaking rate
	int wrdCount = 0; bool upEvent = false;double wPSec = 0.;
	count = 0; double loudSum = 0; double upCount = 0;
	std::list<double>::iterator itLoud = loudHistory.begin();
	double loudWeight = 1.;
	double avgLoud = 0;
	for (itd = pitchHistory.begin();itd!=pitchHistory.end();++itd){
		if((*itd>95) && !upEvent){
			upEvent = true;
			wrdCount++;
		}else if(*itd < 95){
			upEvent = false;
		}
		// Process loudness data
		// We'll apply a leaky integrator on the values
		// The latest chunk of data will be weighted as 1.
		// Weights for all the chunks before will be exponentially decreased
		// TODO
		if(upEvent){
			upCount = upCount + 1;
			loudSum = loudSum + loudWeight*(*itLoud);
		}

		++itLoud;
	}
	//averageLoudness
	if(upCount==0)
		avgLoud = 0;
	else
		avgLoud = loudSum/upCount;
	// Words (Syllables) per sec
	wPSec = double(wrdCount)/(histItemCount.size()*0.01*histItemCount.back()); 

	// Clear everything
	allData_loud.clear();
	allData_pitch.clear();
	ifs_loud.close();
	ifs_pitch.close();
	remove("output.loud");		//uncomment to DEBUG
	remove("output.pitch");		//uncomment to DEBUG

	char retval[256] = {'\0'};
	sprintf(retval,",avgLoudness:%f,speakingRate:%f,",avgLoud,wPSec);
	std::cout<<retval<<std::endl;	//Debug: 
	return retval;
}

void initializeSound(){
	//HWAVEOUT hWaveOut; /* device handle */
	WAVEFORMATEX wfx; /* look this up in your documentation */
	LPSTR block;/* pointer to the block */
	DWORD blockSize;/* holds the size of the block */

	MMRESULT resultMM;/* for waveOut return values */
	/*
	* first we need to set up the WAVEFORMATEX structure. 
	* the structure describes the format of the audio.
	*/
	wfx.nSamplesPerSec = 44100; /* sample rate */
	wfx.wBitsPerSample = 16; /* sample size */
	wfx.nChannels = 1; /* channels*/
	/*
	* WAVEFORMATEX also has other fields which need filling.
	* as long as the three fields above are filled this should
	* work for any PCM (pulse code modulation) format.
	*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	/*
	* try to open the default wave device. WAVE_MAPPER is
	* a constant defined in mmsystem.h, it always points to the
	* default wave device on the system (some people have 2 or
	* more sound cards).
	*/
	if(waveOutOpen(
		&hWaveOut, 
		WAVE_MAPPER, 
		&wfx, 
		0, 
		0, 
		CALLBACK_NULL
		) != MMSYSERR_NOERROR) {
			fprintf(stderr, "unable to open WAVE_MAPPER device\n");
			ExitProcess(1);
	}
}


int main(int argc, char **argv) {
	std::vector<char> charVec;
    
	// Initialize sound player
	// initializeSound();

	// Network communication. Creating the server
	// passing data receive callback and wait for communication
	while(true){
		Server comm(9090);
		if(comm.initialize()){
			if(comm.waitForConnection())
				comm.startMainLoop(processReceivedData);
		}else{
			std::cout<<"Server receiver failed to initialize"<<std::endl;
			return 0;
		}
	}


    // cleanup sound player
	//waveOutClose(hWaveOut);
    //WSACleanup();

    return 0;
}
