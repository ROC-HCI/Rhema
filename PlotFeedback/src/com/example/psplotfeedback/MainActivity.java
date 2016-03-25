/**
 * Plot VOLUME Feedback System
 */
package com.example.psplotfeedback;

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

public class MainActivity extends Activity implements Runnable {

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
	
	static final int amountData = 25;
	static List<Float> data = new ArrayList<Float>();
	
	private String avgLoudness = "";
	private float maxThreshold = 0.08f;	//CALIBRATE
	
	Thread mainThread;
	
	public synchronized void setLoudness(String loudness) {
		avgLoudness = loudness;
		float avgLoud = Float.parseFloat(avgLoudness);
			Log.i("debugRun", "avgLoud: " + avgLoud);
		checkInputData(avgLoud);
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
		super.onStop();
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
	}
	
	public void run() {
		checkRandNum();
	}
	
	public void checkRandNum() {
		data.add(360f);	//first point drawn on startup...
		if(data.size() >= amountData)
			data.remove(0);
	} //end check

	public void checkInputData(float avgLoud) {
//		if(avgLoud > maxThreshold)	//necessary check?
//			avgLoud = maxThreshold;
		float multiplier = 360/maxThreshold;
		float scaleData = 360-(avgLoud*multiplier);
		Log.i("debugRun", "scaleData: " + scaleData);
		
		if(scaleData <= 0)				//if loudness surpasses graph maximum
			data.add(0f);				//draw the line at the top of the graph
		else
			data.add(scaleData);		//add newest input data, scaled and adjusted to graph coordinates

		if(data.size() >= amountData)	//keep list of size [amountData]
			data.remove(0);
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
