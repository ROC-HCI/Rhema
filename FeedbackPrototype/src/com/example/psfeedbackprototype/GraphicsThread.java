/**
 * Quadrant Feedback System
 */
package com.example.psfeedbackprototype;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;
	private final int SLEEP_TIME = 250;
	
	double stepSizeUp = 5.55;	//red to green
	double stepSizeDown = 1.7;	//green to red
	
	//Coordinates for Google Glass:			r1   r2   r3   r4   R 	 G  B
	static Quadrant q1 = new Quadrant(0, 0, 348, 0,   640, 153, 200, 0, 0);
	static Quadrant q2 = new Quadrant(0, 0, 50,  0,   343, 153, 200, 0, 0);
	static Quadrant q3 = new Quadrant(0, 0, 50,  158, 343, 310, 200, 0, 0);
	static Quadrant q4 = new Quadrant(0, 0, 348, 158, 640, 310, 200, 0, 0);
	
	public GraphicsThread(MySurfaceView mySurfaceView) {
		this.mySurfaceView = mySurfaceView;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}
	
	public void changeState(int currentState, int futureState, Quadrant q) {
		q.currentState = currentState;
		q.futureState = futureState;
	}	
	
	public void colorRedToGreen(Quadrant q, Canvas canvas) {
		if(q.G <= 200 && (q.G + stepSizeUp) < 255 && (q.R - stepSizeUp) > 0) {
			q.G += stepSizeUp;
			q.R -= stepSizeUp;
		}
		mySurfaceView.reDraw(canvas, q);
	} //end red to green
	
	public void colorGreenToRed(Quadrant q, Canvas canvas) {
		if(q.R <= 200 && (q.G - stepSizeDown) > 0 && (q.R + stepSizeDown) < 255) {
			q.G -= stepSizeDown;
			q.R += stepSizeDown;
		}
		mySurfaceView.reDraw(canvas, q);
	} //end green to red
	
	public void run() {
		Canvas canvas;
		while(isRunning) {
			try { Thread.sleep(SLEEP_TIME); } catch (InterruptedException e) {}	//sleep to prevent overheating	
			canvas = mySurfaceView.getHolder().lockCanvas();
			if(canvas != null) {					
						
				if(q1.currentState - q1.futureState == -1) 	//red -> green
					colorRedToGreen(q1, canvas);
				if(q2.currentState - q2.futureState == -1) 
					colorRedToGreen(q2, canvas);
				if(q3.currentState - q3.futureState == -1) 
					colorRedToGreen(q3, canvas);
				if(q4.currentState - q4.futureState == -1) 
					colorRedToGreen(q4, canvas);
				
				if(q1.currentState - q1.futureState == 1) 	//green -> red
					colorGreenToRed(q1, canvas);
				if(q2.currentState - q2.futureState == 1) 
					colorGreenToRed(q2, canvas);
				if(q3.currentState - q3.futureState == 1) 
					colorGreenToRed(q3, canvas);
				if(q4.currentState - q4.futureState == 1) 
					colorGreenToRed(q4, canvas);

				mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if(canvas != null)
		} //end while(isRunning)
	}//end run

}//end class
