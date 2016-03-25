/**
 * Plot SPEED Feedback System
 */
package com.example.psplotspeed;

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
        attemptEveryMilliSec = 300;
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
	            	if(input.equals("ok")){
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
	            	}else if (input.startsWith("avgLoudness:")){
	            		// Results arrived. Send okay
	            		byte[] toSend = "ok".getBytes("UTF-8");
	            	
	            		String speakingRateString = input.substring(34, 35);
	            		Log.i("debugRun", "rate string: " + speakingRateString);
	            		callerObj.setSpeed(speakingRateString);
	            		
	        			Log.i("Communication_Thread", "Sending: " + new String(toSend));
	            		out.write(toSend);
	            		out.flush();
	            		
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
