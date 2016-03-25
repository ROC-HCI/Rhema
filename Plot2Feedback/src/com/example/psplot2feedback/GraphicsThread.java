/**
 * Plot2 Feedback System
 */
package com.example.psplot2feedback;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;
	private final int SLEEP_TIME = 1000;
	
	//						s  v			 	  R	   G    B
	static Box b1 = new Box(1, 1, 0, 0, 640, 360, 255, 255, 255);
	static double[] coordSpeed = new double[MainActivity.amountData * 2];
	static double[] coordVolume = new double[MainActivity.amountData * 2];
	
	public GraphicsThread(MySurfaceView mySurfaceView) {
		this.mySurfaceView = mySurfaceView;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}
	
	public void changeValue(int speedVal, int volumeVal, Box b) {
		b.speedValue = speedVal;
		b.volumeValue = volumeVal;
	}
	
	/**
	 * Fills coord[] arrays with corresponding xy-coordinates for volume and speed values
	 * @param canvas
	 * @param b
	 */	
	public void plotData(Canvas canvas, Box b) {
		for(int i = 0; i < MainActivity.dataVolume.size(); i++) {
			coordVolume[(i << 1) + 0] = 50 + (int)(590/MainActivity.amountData)*i;	//x-coordinates, fixed
			coordVolume[(i << 1) + 1] = MainActivity.dataVolume.get(i);				//y-coordinates
		}
		for(int i = 0; i < MainActivity.dataSpeed.size(); i++) {
			coordSpeed[(i << 1) + 0] = 50 + (int)(590/MainActivity.amountData)*i;	//x-coordinate, fixed
			coordSpeed[(i << 1) + 1] = MainActivity.dataSpeed.get(i);				//y-coordinate
		}
		mySurfaceView.reDraw(canvas, b);
	}
	
	public void run() {
		Canvas canvas;
		while(isRunning) {
			try { Thread.sleep(SLEEP_TIME); } catch (InterruptedException e) {} //sleep to prevent overheating
			canvas = mySurfaceView.getHolder().lockCanvas();
			if(canvas != null) {
				plotData(canvas, b1);
				mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if
		} //end while
	}//end run

}//end class