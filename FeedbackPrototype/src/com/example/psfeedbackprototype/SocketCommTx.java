/**
 * Quadrant Feedback System
 */
package com.example.psfeedbackprototype;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.net.Socket;

import android.util.Log;

// Coded by Md. Iftekhar Tanveer (go2chayan@gmail.com)
public class SocketCommTx implements Runnable {
	public volatile Thread communicator;
	private Socket socket;
	private BufferedInputStream in;
	private BufferedOutputStream out;
	private byte[] buff;
	private int attemptEveryMilliSec = 0;
	private String Server_IP;
	private int Server_Port;
	private MainActivity callerObj;
	
	//Constructor
	public SocketCommTx(String serverIP, int serverPort, MainActivity callerID){
		callerObj = callerID;
		Server_IP = serverIP;
		Server_Port = serverPort;
		communicator = new Thread(this);
        communicator.start();
        attemptEveryMilliSec = 500;
        buff = null;
	}
	
	// Stop communication thread
	public void stopThread(){
	 	if(socket!=null)
			try {
				socket.close();
			} catch (Exception e) {
				Log.e("Communication_Thread", e.getMessage());
			}
		Thread surrogate = communicator;
		communicator = null;
		surrogate.interrupt();
	}
	
	// Fill the buffer
	public synchronized void fillBuffer(byte[] buffer){
		buff = buffer.clone();
		Log.i("Communication_Thread","Buffer Filled");
	}
	
	// Run looper for communication thread (TCP server)
	public void run(){
		Thread thisThread = Thread.currentThread();
		try{
			// A new connection
            socket = new Socket(Server_IP,Server_Port);	
            if (socket.isConnected())
            	Log.i("Communication_Thread","Socket Connected");
            else
            	Log.i("Communication_Thread","Socket Connection Failed");
        
            // Connected. Initialize stream with 10MB buffer
            in = new BufferedInputStream(socket.getInputStream(),10485760);
            out = new BufferedOutputStream(socket.getOutputStream(),10485760);
            
            // Main loop for the thread
            byte[] tempBuff = new byte[10240];
            String input = "";
            while (communicator==thisThread) {
            	input = "";
        		if(in.available()>0){
            		int bytesReceived = in.read(tempBuff);
            		input = new String(tempBuff,0,bytesReceived);
            		Log.i("Communication_Thread", "Arrived: " + input);
            	}
            	// If the latest arrival is a pulse then either send acknowledgement
            	// either in the form of ok or the actual data
            	if(input.length()>0){
            		if (input.contains("avgLoudness:") || input.contains("speakingRate:")){
	            		// The input may accompanied with "ok" signals
	            		// remove those if it has
	            		input = input.replaceAll("ok", "");
	            		
	            		// Split the string into sequences of string
	            		String avgLoudnessString = "";
	            		String avgRateString = "";
	            		String[] inputs = input.split(",");
	            		for(int i = 0; i < inputs.length; i++){
	            			String[] fields = inputs[i].split(":");
	            			if(fields[0].equalsIgnoreCase("avgLoudness"))
	            				avgLoudnessString = fields[1];
	            			else if(fields[0].equalsIgnoreCase("speakingRate"))
	            				avgRateString = fields[1];
	            		}
	            		callerObj.setValues(avgLoudnessString, avgRateString);
	            		
	            		// Results arrived. Send okay
	            		byte[] toSend = "ok".getBytes("UTF-8");
	            		Log.i("Communication_Thread", "Sending: " + new String(toSend));
	            		out.write(toSend);
	            		out.flush();
	            	}else if(input.contains("ok")){
                		// If buffer is loaded send the data
                		if(buff!=null){
                			//out.write(Base64.encode(buff, Base64.DEFAULT));	// Write the buffer
                			out.write(buff);
                    		out.flush();
    	        			Log.i("Communication_Thread", "Sending: " + buff.length + " Bytes");                		
                    		buff = null;
                		}else{
                			// Otherwise, just send okay
		            		byte[] toSend = "ok".getBytes("UTF-8");
		        			Log.i("Communication_Thread", "Sending: " + new String(toSend));
		            		out.write(toSend);
		            		out.flush();
		            	}
	            	}else{
	            		Log.i("Communication_Thread", "Unknown Request");
	            	}
            	}
            	Thread.sleep(attemptEveryMilliSec);
            }
		    socket.close();

		} catch (Exception e){
			Log.e("Communication_Thread", e.toString() + " " + e.getStackTrace());
		}
	}

}
