package com.github.rochci.rocspeakglass_calib;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.WindowManager;
import android.view.GestureDetector;
import android.widget.TextView;

// Coded by Md. Iftekhar Tanveer (go2chayan@gmail.com)
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
    private String serverIP = "192.168.1.101"; //CHECK IP ADDRESS********
	
	private static final String TAG = "ROCHCI::Activity";
	private long lastKeyUp = 0;
	private GestureDetector gestureDetector;
	private TextView txtView;
	private TextView txtView_recording;
	private boolean start = false;
	private ArrayList<Double> allLoudness = new ArrayList<Double>();

	// constructor
	public MainActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}
	
	public synchronized void setValues(String maxloudness, String rate) {
		allLoudness.add(Double.parseDouble(maxloudness));
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.activity_main_glass);
		txtView = (TextView) findViewById(R.id.textView1);
		txtView_recording = (TextView)findViewById(R.id.textView3);

		gestureDetector = new GestureDetector(this,
			new GestureDetector.SimpleOnGestureListener() {
				@Override
				public boolean onDoubleTap(MotionEvent e) {

					return true;
				}
			});
	}

	@Override
	protected void onResume() {
		super.onResume();
		readIPaddress();
		comm = new SocketCommTx(serverIP, 9090, this);
	}
	
	@Override
	protected void onPause() {
    	comm.stopThread();
		super.onPause();
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		int pid = android.os.Process.myPid();
		android.os.Process.killProcess(pid);
	}
	
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
        int minBufferSize = AudioRecord.getMinBufferSize(sampleRate,
                        AudioFormat.CHANNEL_IN_MONO, encoding);
        return minBufferSize>BufferElements2Rec*BytesPerElement?
        		minBufferSize:BufferElements2Rec * BytesPerElement;
    }

	@Override
	// for phone
	public boolean onTouchEvent(MotionEvent e) {
		if (!gestureDetector.onTouchEvent(e))
			return super.onTouchEvent(e);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// Write code for taking picture
		if (keyCode == KeyEvent.KEYCODE_CAMERA) {
			return true;
		} else if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN
				|| keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
			
			return true;
		} else if (keyCode == KeyEvent.KEYCODE_C) {
			
			return true;
		} else {
			return super.onKeyDown(keyCode, event);
		}
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER
				&& (System.currentTimeMillis() - lastKeyUp) <= 500) {
			if (!start){
				start = true;
				txtView.setText("Stop");
				txtView_recording.setText("Listening ...");
				startRecording();
			}else{
				start = false;
				txtView.setText("Start");
				txtView_recording.setText("");
				stopRecording();
				processCalibrationData();
			}
			lastKeyUp = 0; // prevent accidental 3x and 4x taps
			return true;
		} else {
			lastKeyUp = System.currentTimeMillis();
			return super.onKeyUp(keyCode, event);
		}

	}
	
	public void processCalibrationData(){
		Collections.sort(allLoudness);
		Collections.reverse(allLoudness);
		writeCalibrationData();
	}
	
	/** Method to write ascii text characters to file on SD card. Note that you must add a 
	   WRITE_EXTERNAL_STORAGE permission to the manifest file or this method will throw
	   a FileNotFound Exception because you won't have write permission. */

	private int writeCalibrationData(){
		if(allLoudness.size()>0)
			Log.i("writeCalibrationData", "Writing " + allLoudness.size() + "Calibration points");
		else
			Log.i("writeCalibrationData", "No calibration data to write");
		
	    File root = android.os.Environment.getExternalStorageDirectory(); 
	    File dir = new File (root.getAbsolutePath() + "/ROCHCIStorage");
	    if (!dir.exists())dir.mkdirs();
	    File file = new File(dir, "ROCSpeakGlass_Calibration.info");

	    try {
	        FileOutputStream f = new FileOutputStream(file);
	        PrintWriter pw = new PrintWriter(f);
	        // Write all the data to the file
	        for(int i = 0; i < allLoudness.size(); i++)
	        	pw.println(allLoudness.get(i));
	        pw.flush();
	        pw.close();
	        f.close();
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();
	        Log.i(TAG, "******* File not found. Did you" +
	                " add a WRITE_EXTERNAL_STORAGE permission to the   manifest?");
	    } catch (IOException e) {
	        e.printStackTrace();
	    }   
	    return allLoudness.size();
	}

	/** Method to read in a text file placed in the res/raw directory of the application. The
	  method reads in all lines of the file sequentially. */

	private void readCalibrationData(){
		File root = android.os.Environment.getExternalStorageDirectory(); 
	    File dir = new File (root.getAbsolutePath() + "/ROCHCIStorage");
	    File file = new File(dir, "ROCSpeakGlass_Calibration.info");

	    try {
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        String test;
	        while ((test = br.readLine()) != null){               
	            // Put the data in array
	        }
	        br.close();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
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

	SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {
		public void surfaceCreated(SurfaceHolder holder) {

		}

		public void surfaceChanged(SurfaceHolder holder, int format, int width,
				int height) {

		}

		public void surfaceDestroyed(SurfaceHolder holder) {

		}
	};
}
