/**
 * Audio Feedback System
 */
package com.example.psaudiofeedback;

import android.util.Log;

public class AudioThread extends Thread {
	private boolean isRunning = false;
	int tickRate;
	
	public AudioThread(int tickRate) {
		this.tickRate = tickRate;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}

	public void setTickRate(int tickRate) {
		this.tickRate = tickRate;
	}
	
	public void run() {
		while(isRunning) {
			playSound(tickRate);
		}
	}
	
	protected void playSound(int tickRate) {
		try {
			MainActivity.audioManager.playSoundEffect(14);
			Thread.sleep((1300/tickRate));					//CHANGE MATH, INVERSION
		} catch (InterruptedException e) { 
			Log.i("debugRun", "playSound interrupted");
		}
	}

}//end class
