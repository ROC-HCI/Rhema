/**
 * Plot SPEED Feedback System
 */
package com.example.psplotspeed;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;
	
	//										R	 G    B
	static Box b1 = new Box(0, 0, 660, 380, 255, 255, 255);
	static float[] coord = new float[MainActivity.amountData * 2];
	
	public GraphicsThread(MySurfaceView mySurfaceView) {
		this.mySurfaceView = mySurfaceView;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}
	
	/**
	 * Fills coord[] arrays with corresponding xy-coordinates for the data in the data list
	 * @param canvas
	 * @param b
	 */	
	public void plotData(Canvas canvas, Box b) {
		for(int i = 0; i < MainActivity.data.size(); i++) {
			coord[(i << 1) + 0] = 50 + (int)(590/MainActivity.amountData)*i;	//x-coordinates, fixed
			coord[(i << 1) + 1] = MainActivity.data.get(i);						//y-coordinates
		}
		mySurfaceView.reDraw(canvas, b, coord);
	}
	
	public void run() {
		Canvas canvas;
		while(isRunning) {
			canvas = mySurfaceView.getHolder().lockCanvas();
			if(canvas != null) {
				try { Thread.sleep(250); } catch (InterruptedException e) {} //sleep to prevent overheating
				plotData(canvas, b1);
				mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if
		} //end while
	}//end run

}//end class
