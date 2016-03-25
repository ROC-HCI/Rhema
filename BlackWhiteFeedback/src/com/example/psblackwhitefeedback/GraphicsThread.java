/**
 * Black&White Feedback System
 */
package com.example.psblackwhitefeedback;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;

	double stepSizeUp = 0.5;	//black -> white (vol = quiet)
	double stepSizeMid = 0.5;	//white -> black (vol = louder) (slow decrease)
	double stepSizeDown = 0.8;	//white -> black (vol = loud)	(goal?)
	
	static Box b1 = new Box(1, 1, 0, 0, 640, 360, 255, 255, 255);
	
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
	
	public void colorInc(Box b, Canvas canvas) {	//black -> white (vol = quiet)
		if(b.R < 255 && (b.R + stepSizeUp) < 255) {
			b.R += stepSizeUp;
			b.G += stepSizeUp;
			b.B += stepSizeUp;
		}
		mySurfaceView.reDraw(canvas, b);
	} 
	
	public void colorMid(Box b, Canvas canvas) {	//white -> black (vol = louder)
		if(b.R > 0 && (b.R - stepSizeMid) > 0) {
			b.R -= stepSizeMid;
			b.G -= stepSizeMid;
			b.B -= stepSizeMid;
		}
		mySurfaceView.reDraw(canvas, b);
	}
	
	public void colorDec(Box b, Canvas canvas) {	//white -> black (vol = loud)
		if(b.R > 0 && (b.R - stepSizeDown) > 0) {
			b.R -= stepSizeDown;
			b.G -= stepSizeDown;
			b.B -= stepSizeDown;
		}
		mySurfaceView.reDraw(canvas, b);
	}
	
	public void run() {
		while(isRunning) {
			Canvas canvas = mySurfaceView.getHolder().lockCanvas();
			
			if(canvas != null) {
					while(isRunning) {
						mySurfaceView.getHolder().lockCanvas();
							
						if(b1.volumeValue == 1)		//QUIET: black -> white
							colorInc(b1, canvas);
						if(b1.volumeValue == 2)		//MID: white -> black (slower)
							colorMid(b1, canvas);
						if(b1.volumeValue == 3)		//LOUD: white -> black
							colorDec(b1, canvas);
						
						mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
					} //end while
					mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if
		} //end while
	}//end run
	
}//end class
