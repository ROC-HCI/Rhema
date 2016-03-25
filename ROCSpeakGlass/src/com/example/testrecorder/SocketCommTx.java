package com.example.testrecorder;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

// Coded by Md. Iftekhar Tanveer (go2chayan@gmail.com)
public class SocketCommTx implements Runnable {
	public volatile Thread communicator;
	private ServerSocket listener;
	private Socket socket;
	private BufferedReader in;
	private DataOutputStream out;
	private byte[] buff;
	private int attemptEveryMilliSec = 0;
	
	//Constructor
	public SocketCommTx(int attemptEveryMilliSec_){
		communicator = new Thread(this);
        communicator.start();
        attemptEveryMilliSec = attemptEveryMilliSec_;
	}
	
	// Stop communication thread
	public void stopThread(){
		 	if(socket!=null)
				try {
					listener.close();
					socket.close();
				} catch (Exception e) {
					Log.e("SocketComm_stopThread", e.getMessage());
				}
			Thread surrogate = communicator;
			communicator = null;
			surrogate.interrupt();
	}
	
	// Fill the buffer
	public synchronized void fillBuffer(byte[] buffer){
		buff = buffer.clone();
	}
	
	// Run looper for communication thread (TCP server)
	public void run(){
		Thread thisThread = Thread.currentThread();
		try{
			// A new connection will start every time the connection closes
			while(communicator==thisThread){
				listener = new ServerSocket(9090);
				// Will stop and listen for a connection
                socket = listener.accept();	
                
                // Connected
                in = new BufferedReader(
                        new InputStreamReader(socket.getInputStream()));
                out =
                    new DataOutputStream(socket.getOutputStream());
                
                // Send a greetings!
                long lastSendTime = 0;
                while (communicator==thisThread) {
                	
                	if((System.currentTimeMillis() - lastSendTime) >= attemptEveryMilliSec)
                    	synchronized(this){
                    		if(buff!=null){
	                    		out.write(buff);	// Write the buffer
	                    		out.flush();
	                    		buff = null;
	                    		lastSendTime = System.currentTimeMillis();
                    		}else
                    			out.writeBytes("null");
                    	}
                }
                
                listener.close();
                socket.close();
			}
			
		} catch (Exception e){
			Log.e("Communication Thread", e.toString());
		}
	}

}
