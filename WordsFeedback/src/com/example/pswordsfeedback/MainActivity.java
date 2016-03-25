/**
 * Words Feedback System
 */
package com.example.pswordsfeedback;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.graphics.Color;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	private static final int RECORDER_SAMPLERATE = 44100;
    private static final int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO;
    private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;
    private static final int TIME_FRAME_IN_MILIS = 1000; // The value of each chunk
    private AudioRecord recorder;
    private Thread streamingThread;
    private boolean isRecording = false;
    private int BufferElements2Rec = MainActivity.RECORDER_SAMPLERATE 
                                         * (TIME_FRAME_IN_MILIS/1000);  // total number of elements to buffer is equal
                                                                        // number of samples per sec * the value of each chunk
    private int BytesPerElement = 2; // 2 bytes in 16bit format
    private SocketCommTx comm;
    private String serverIP = "192.168.1.101"; //CHECK IP ADDRESS**********
    
   	public static TextView txtViewVol, txtViewSpeed;

    private final int DATA_TO_DISPLAY = 20;		//number of seconds until words are displayed
    private final int rangeHits = DATA_TO_DISPLAY/2 + 1;
    
	int count = 0;
	int rangeLowVol = 0, rangeMidVol = 0, rangeHighVol = 0;
	int rangeLowRate = 0, rangeMidRate = 0, rangeHighRate = 0;
    
    static int volumeValue, speedValue;
	double maxThresholdVol = 67;		//CALIBRATE
    double upperThresholdVol = 58.9;	//CALIBRATE
	double lowerThresholdVol = 54.0;	//CALIBRATE
	double lowerThresholdRate = 1.65;	//CALIBRATE
	double upperThresholdRate = 2.7;	//CALIBRATE
	
	public synchronized void setValues(String loudness, String rate) {
		double avgLoud = Double.parseDouble(loudness);
		double avgSpeed = Double.parseDouble(rate);
		Log.i("debugRun", "avgLoud: " + avgLoud + " avgRateF: " + avgSpeed);
		checkInputData(avgLoud, avgSpeed);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 
			WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.activity_main);
		
		txtViewVol = (TextView) findViewById(R.id.textViewVol);
		txtViewSpeed = (TextView) findViewById(R.id.textViewSpeed);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		loadCalibData();
		readIPaddress();
		comm = new SocketCommTx(serverIP, 9090, this);
		startRecording();
	}
	
	private void loadCalibData() {
		File root = android.os.Environment.getExternalStorageDirectory(); 
	    File dir = new File (root.getAbsolutePath() + "/ROCHCIStorage");
	    File file = new File(dir, "ROCSpeakGlass_Calibration.info");
	    
	    if(!file.exists()){
	    	Toast.makeText(getApplicationContext(), "No Calibration Data Found", Toast.LENGTH_LONG).show();
	    	return;
	    }
	    try {
	    	BufferedReader br = new BufferedReader(new FileReader(file));
			String maxLoudVal = br.readLine();
			if(maxLoudVal == null) {
				Toast.makeText(getApplicationContext(), "No Calibration Data Found", Toast.LENGTH_LONG).show();
				return;
			}
			maxThresholdVol = Double.parseDouble(maxLoudVal);
			Log.i("debugRun", "maxThresholdVol: " + maxThresholdVol);
//			Toast.makeText(getApplicationContext(), "max=" + maxThresholdVol, Toast.LENGTH_LONG).show();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			Toast.makeText(getApplicationContext(), "No Calibration Data Found", Toast.LENGTH_LONG).show();
			e.printStackTrace();
		}
	} //end loadCalibData
	
	private void readIPaddress(){
		File root = android.os.Environment.getExternalStorageDirectory(); 
	    File dir = new File (root.getAbsolutePath() + "/ROCHCIStorage");
	    File file = new File(dir, "ipaddress.txt");

	    try {
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        serverIP = br.readLine();	//***
	        Log.i("debugRun", "serverIP: " + serverIP);
	        br.close();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
	}
	
	@Override
	protected void onPause() {
		stopRecording();
    	comm.stopThread();
		super.onPause();
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
	}
	
	public void resetWords() {
		MySurfaceView.setWordVolume("");
		MySurfaceView.setWordSpeed("");
		MySurfaceView.setBGcolor(Color.BLACK);
	}
	
	public void checkInputData(double avgLoud, double avgSpeed) {		
		count = ++count%(DATA_TO_DISPLAY+1);
		Log.i("debugRun", "count: " + count);
		
		if(avgLoud <= lowerThresholdVol)
			rangeLowVol++;
		if(avgLoud > lowerThresholdVol && avgLoud < upperThresholdVol)
			rangeMidVol++;
		if(avgLoud >= upperThresholdVol)
			rangeHighVol++;
		
		if(avgSpeed <= lowerThresholdRate)
			rangeLowRate++;
		if(avgSpeed > lowerThresholdRate && avgSpeed < upperThresholdRate)
			rangeMidRate++;
		if(avgSpeed >= upperThresholdRate)
			rangeHighRate++;
		
		//Reset display to blank until next 30 seconds:
		if(count == 3)
			resetWords();
		
		if(count == DATA_TO_DISPLAY) {
			MySurfaceView.setBGcolor(Color.rgb(0, 0, 85));
			if(rangeLowVol > rangeHits)
				MySurfaceView.setWordVolume("LOUDER");	//"LOUDER"
			else if(rangeMidVol >= rangeHits+3)
				MySurfaceView.setWordVolume("VARY VOLUME");
			else if(rangeHighVol > rangeHits)
				MySurfaceView.setWordVolume("SOFTER");	//"LOUDER"
			else 
				MySurfaceView.setWordVolume("good!");
			
			if(rangeLowRate > rangeHits)
				MySurfaceView.setWordSpeed("FASTER");	//"FASTER"
			else if(rangeMidRate >= rangeHits+2)
				MySurfaceView.setWordSpeed("VARY SPEED");
			else if(rangeHighRate > rangeHits)
				MySurfaceView.setWordSpeed("SLOWER");	//"SLOWER"
			else
				MySurfaceView.setWordSpeed("good!");
			
			rangeLowVol = 0; rangeMidVol = 0; rangeHighVol = 0;
			rangeLowRate = 0; rangeMidRate = 0; rangeHighRate = 0;
		}
		Log.i("debugRun", "LowVol: " + rangeLowVol + " MidVol: " + rangeMidVol + " HighVol: " + rangeHighVol);
		Log.i("debugRun", "LowRate: " + rangeLowRate + " MidRate: " + rangeMidRate + " HighRate: " + rangeHighRate);
		Log.i("debugRun", "wordVolume: " + MySurfaceView.getWordVolume() + " wordSpeed: " + MySurfaceView.getWordSpeed());
	} //end checkInputData
	
	private void readIPaddress(){
		File root = android.os.Environment.getExternalStorageDirectory(); 
	    File dir = new File (root.getAbsolutePath() + "/ROCHCIStorage");
	    File file = new File(dir, "ipaddress.txt");

	    try {
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        serverIP = br.readLine();	//***
	        Log.i("debugRun", "serverIP: " + serverIP);
	        br.close();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
	}
	//****************************************************************************************************************
	
	//Write the output audio in byte
	private void writeAudioDataToSocket() {
        short sData[] = new short[BufferElements2Rec]; // buffer to store the elements, using 
                                                       // short because encoding format is 16 bit PCM
        while (isRecording) {
            // gets the voice output from microphone to byte format
            int read = recorder.read(sData, 0, BufferElements2Rec);
            byte bData[] = short2byte(sData);
            comm.fillBuffer(bData);
            //Log.d("Extracted_Data", new String(bData));
            //comm.fillBuffer(bData);
        }
    }
	
	private byte[] short2byte(short[] sData) {
		int shortArrsize = sData.length;
		byte[] bytes = new byte[shortArrsize * 2];
		for (int i = 0; i < shortArrsize; i++) {
		    bytes[i * 2] = (byte) (sData[i] & 0x00FF);
		    bytes[(i * 2) + 1] = (byte) (sData[i] >> 8);
		    sData[i] = 0;
		}
		return bytes;
	}
	
	private void startRecording() {
        // Calculate the required buffersize for the desired sample rate and encoding rate
        int bufferSize = determineMinimumBufferSize(RECORDER_SAMPLERATE, RECORDER_AUDIO_ENCODING);
        // Prepare the recorder
        recorder = new AudioRecord(MediaRecorder.AudioSource.MIC,
                RECORDER_SAMPLERATE, RECORDER_CHANNELS,
                RECORDER_AUDIO_ENCODING, bufferSize); 
        recorder.startRecording();
        isRecording = true;
        Log.d("MainActivity_startRecording", "recording started");
        // Running thread for writing audio data
        streamingThread = new Thread(new Runnable() {
            public void run() {
                writeAudioDataToSocket();
            }
        }, "AudioRecorder Thread");
        streamingThread.start();
        Log.d("MainActivity_startRecording", "recording started");
    }
	
	//Stops the recording activity
    private void stopRecording() {
        if (recorder != null) {
            isRecording = false;
            recorder.stop();
            recorder.release();
            recorder = null;
            streamingThread = null;
         }
    }
    
    // Function for calculating minimum buffer size
    private int determineMinimumBufferSize(final int sampleRate, int encoding) {
        int minBufferSize =
                AudioRecord.getMinBufferSize(sampleRate,
                        AudioFormat.CHANNEL_IN_MONO, encoding);
        return minBufferSize>BufferElements2Rec*BytesPerElement?
        		minBufferSize:BufferElements2Rec * BytesPerElement;
    }	

}//end class
