/**
 * Words Feedback System
 */
package com.example.pswordsfeedback;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;
	private final int SLEEP_TIME = 500;
	
	static Box b1 = new Box(1, 1, 0, 0, 640, 360, 255, 255, 255);
	
	public GraphicsThread(MySurfaceView mySurfaceView) {
		this.mySurfaceView = mySurfaceView;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}
	
	public void run() {
		Canvas canvas;
		while(isRunning) {
			try { Thread.sleep(SLEEP_TIME); } catch (InterruptedException e) {}	//sleep to prevent overheating
			canvas = mySurfaceView.getHolder().lockCanvas();
			if(canvas != null) {
				mySurfaceView.reDraw(canvas, b1);
				mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if(canvas != null)
		} //end while(isRunning)
	}//end run

}//end class
