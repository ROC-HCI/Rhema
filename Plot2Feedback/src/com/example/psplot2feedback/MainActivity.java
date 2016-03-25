/**
 * Plot2 Feedback System
 */
package com.example.psplot2feedback;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

public class MainActivity extends Activity implements Runnable {
	private static final int RECORDER_SAMPLERATE = 44100;
    private static final int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO;
    private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;
    private static final int TIME_FRAME_IN_MILIS = 2000; // The value of each chunk
    private AudioRecord recorder;
    private Thread streamingThread;
    private boolean isRecording = false;
    private int BufferElements2Rec = MainActivity.RECORDER_SAMPLERATE 
                                         * (TIME_FRAME_IN_MILIS/1000);  // total number of elements to buffer is equal
                                                                        // number of samples per sec * the value of each chunk
    private int BytesPerElement = 2; // 2 bytes in 16bit format
    private SocketCommTx comm;
    private String serverIP = "192.168.1.101";	//CHECK IP ADDRESS**********

	static final int amountData = 25;
	static List<Double> dataVolume = new ArrayList<Double>();
	static List<Double> dataSpeed = new ArrayList<Double>();
	
	static int volumeValue, speedValue;
	private double maxThresholdVol = 63;	//CALIBRATE
	private double minThresholdVol = 50;	//CALIBRATE
	private double maxThresholdRate = 6;	//CALIBRATE
	
	Thread mainThread;
	
	public synchronized void setValues(String loudness, String rate) {
		double avgLoud = Double.parseDouble(loudness);
		double avgRateF = Double.parseDouble(rate);
		Log.i("debugRun", "avgLoud: " + avgLoud + " avgRateF: " + avgRateF);
		checkInputData(avgLoud, avgRateF);
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

		mainThread = new Thread(this, "mainThread");
		mainThread.start();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		loadCalibData();
		readIPaddress();
		comm = new SocketCommTx(serverIP, 9090, this);
		startRecording();
	}
	
	private void loadCalibData(){
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
			Toast.makeText(getApplicationContext(), "value: " + maxThresholdVol, Toast.LENGTH_LONG).show();
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
	
	public void run() {
		Log.i("debugRun", "RUNNING MAINTHREAD");
		checkRandNum();
	}
	
	public void checkRandNum() {
		Log.i("debugRun", "CHECKING RANDOM NUMBER");
		dataVolume.add(180.);
		dataSpeed.add(360.);
		if(dataVolume.size() >= amountData)
			dataVolume.remove(0);
		if(dataSpeed.size() >= amountData)
			dataSpeed.remove(0);
	} //end check
	
	public void checkInputData(double avgLoud, double avgRateF) {
		//VOLUME//
		double rangeVol = maxThresholdVol - minThresholdVol;
		double scaleDataVol = 180;			//any loudness under [minThresholdVol] dB will be plotted as zero
		if(avgLoud >= maxThresholdVol)
			avgLoud = maxThresholdVol;
		if(avgLoud >= minThresholdVol)
			scaleDataVol = 180-(((avgLoud-minThresholdVol)/rangeVol)*180);
		
		//VOLUME//
		dataVolume.add(scaleDataVol);		//add newest input data, scaled and adjusted to graph coordinates

		if(dataVolume.size() >= amountData)	//keep list of size [amountData]
			dataVolume.remove(0);
		
		//SPEAKING RATE//
		if(avgRateF >= maxThresholdRate)
			avgRateF = maxThresholdRate;
		double multiplierRate = 180/maxThresholdRate;
		double scaleDataRate = 360-(avgRateF*multiplierRate);
		
		//SPEAKING RATE//
		dataSpeed.add(scaleDataRate);

		if(dataSpeed.size() >= amountData) 
			dataSpeed.remove(0);
		
		Log.i("debugRun", "scaleDataVol: " + scaleDataVol + " scaleDataRate: " + scaleDataRate);
	} //end check
	
	//****************************************************************************************************
	
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