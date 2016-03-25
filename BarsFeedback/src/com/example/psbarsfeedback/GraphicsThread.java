/**
 * Bars Feedback System
 */
package com.example.psbarsfeedback;

import android.graphics.Canvas;

public class GraphicsThread extends Thread {
	MySurfaceView mySurfaceView;
	private boolean isRunning = false;
	private final int SLEEP_TIME = 250;
	
	double stepSizeUp = 5.55;	//red to green
	double stepSizeDown = 1.8;	//green to red
	
	//Vertical bars
//	static Box b1 = new Box(0, 0, 120, 260, 220, 360, 200, 0, 0);	//volume LOW
//	static Box b2 = new Box(0, 0, 120, 150, 220, 250, 200, 0, 0);	//volume MID
//	static Box b3 = new Box(0, 0, 120, 40,  220, 140, 200, 0, 0);	//volume HIGH
//	static Box b4 = new Box(0, 0, 420, 260, 520, 360, 200, 0, 0);	//speed LOW
//	static Box b5 = new Box(0, 0, 420, 150, 520, 250, 200, 0, 0);	//speed MID
//	static Box b6 = new Box(0, 0, 420, 40,  520, 140, 200, 0, 0);	//speed HIGH
	
	//For L-shape
	static Box b1 = new Box(0, 0, 100, 260, 200, 360, 200, 0, 0);	//volume LOW
	static Box b2 = new Box(0, 0, 100, 150, 200, 250, 200, 0, 0);	//volume MID
	static Box b3 = new Box(0, 0, 100, 40,  200, 140, 200, 0, 0);	//volume HIGH
	static Box b4 = new Box(0, 0, 300, 150, 400, 250, 200, 0, 0);	//speed LOW
	static Box b5 = new Box(0, 0, 410, 150, 510, 250, 200, 0, 0);	//speed MID
	static Box b6 = new Box(0, 0, 520, 150, 620, 250, 200, 0, 0);	//speed HIGH
	
	public GraphicsThread(MySurfaceView mySurfaceView) {
		this.mySurfaceView = mySurfaceView;
	}
	
	public void setRunning(boolean isRunning) {
		this.isRunning = isRunning;
	}
	
	public void changeState(int currentState, int futureState, Box b) {
		b.currentState = currentState;
		b.futureState = futureState;
	}	
	
	public void colorRedToGreen(Box b, Canvas canvas) {
		if(b.G <= 200 && (b.G + stepSizeUp) < 255 && (b.R - stepSizeUp) > 0) {
			b.G += stepSizeUp;
			b.R -= stepSizeUp;
		}
		mySurfaceView.reDraw(canvas, b);
	} //end red to green
	
	public void colorGreenToRed(Box b, Canvas canvas) {
		if(b.R <= 200 && (b.G - stepSizeDown) > 0 && (b.R + stepSizeDown) < 255) {
			b.G -= stepSizeDown;
			b.R += stepSizeDown;
		}
		mySurfaceView.reDraw(canvas, b);
	} //end green to red
	
	public void run() {
		Canvas canvas;
		while(isRunning) {
			try { Thread.sleep(SLEEP_TIME); } catch (InterruptedException e) {}	//sleep to prevent overheating
			canvas = mySurfaceView.getHolder().lockCanvas();
			if(canvas != null) {						
				
				if(b1.currentState - b1.futureState == -1) 	//red -> green
					colorRedToGreen(b1, canvas);
				if(b2.currentState - b2.futureState == -1) 
					colorRedToGreen(b2, canvas);
				if(b3.currentState - b3.futureState == -1) 
					colorRedToGreen(b3, canvas);
				if(b4.currentState - b4.futureState == -1) 
					colorRedToGreen(b4, canvas);
				if(b5.currentState - b5.futureState == -1) 
					colorRedToGreen(b5, canvas);
				if(b6.currentState - b6.futureState == -1) 
					colorRedToGreen(b6, canvas);
				
				if(b1.currentState - b1.futureState == 1) 	//green -> red
					colorGreenToRed(b1, canvas);
				if(b2.currentState - b2.futureState == 1) 
					colorGreenToRed(b2, canvas);
				if(b3.currentState - b3.futureState == 1) 
					colorGreenToRed(b3, canvas);
				if(b4.currentState - b4.futureState == 1) 
					colorGreenToRed(b4, canvas);
				if(b5.currentState - b5.futureState == 1) 
					colorGreenToRed(b5, canvas);
				if(b6.currentState - b6.futureState == 1) 
					colorGreenToRed(b6, canvas);
				
				mySurfaceView.getHolder().unlockCanvasAndPost(canvas);
			} //end if(canvas != null)
		} //end while(isRunning)
	}//end run

}//end class
