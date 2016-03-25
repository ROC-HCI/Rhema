/**
 * Black&White Feedback System
 */
package com.example.psblackwhitefeedback;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

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
    private String serverIP = "192.168.1.103";
	
    private String avgLoudness = "";
	static int volumeValue, speedValue;
	double lowerThreshold = 0.02;	//CALIBRATE
	double upperThreshold = 0.04;	//CALIBRATE
	
	public synchronized void setLoudness(String loudness) {
		avgLoudness = loudness;
		float avgLoud = Float.parseFloat(avgLoudness);
			Log.i("debugRun", "avgLoud: " + avgLoud);
		checkInputData(avgLoud);
	}
	
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
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 
			WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.activity_main);
	}

	@Override
	protected void onResume() {
		super.onResume();
		readIPaddress();
		comm = new SocketCommTx(serverIP, 9090, this);
		startRecording();
	}
	
	
	
	@Override
	protected void onPause() {
		stopRecording();
    	comm.stopThread();
		super.onPause();
	}
	
	
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
	}

	public void checkInputData(float avgLoud) {		
		if(avgLoud <= lowerThreshold)
			volumeValue = 1;
		if(avgLoud > lowerThreshold && avgLoud < upperThreshold)
			volumeValue = 2;
		if(avgLoud >= upperThreshold)
			volumeValue = 3;
		
		MySurfaceView.graphicsThread.changeValue(speedValue, volumeValue, GraphicsThread.b1);
		
		Log.i("debugRun", "volumeValue: " + volumeValue + " speedValue: " + speedValue);
	} //end check

	//**************************************************************************************************
	
	//Write the output audio in byte
	private void writeAudioDataToSocket() {
        short sData[] = new short[BufferElements2Rec]; // buffer to store the elements, using 
                                                       // short because encoding format is 16 bit PCM
        while (isRecording) {
            // gets the voice output from microphone to byte format
            int read = recorder.read(sData, 0, BufferElements2Rec);
            byte bData[] = short2byte(sData);
            comm.fillBuffer(bData);
            Log.d("Extracted_Data", new String(bData));
            comm.fillBuffer(bData);
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
