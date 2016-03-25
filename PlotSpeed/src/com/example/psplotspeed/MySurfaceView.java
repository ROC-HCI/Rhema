/**
 * Plot SPEED Feedback System
 */
package com.example.psplotspeed;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MySurfaceView extends SurfaceView {
	private SurfaceHolder surfaceHolder;
	private Paint paint;
	Handler handler = new Handler();
	
	static GraphicsThread graphicsThread;
	
	public MySurfaceView(Context context) {
		super(context);
		init();
	}
	
	public MySurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}

	public MySurfaceView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
		graphicsThread = new GraphicsThread(this);
		surfaceHolder = getHolder();
		paint = new Paint();
				
		surfaceHolder.addCallback(new SurfaceHolder.Callback() {
			
			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				boolean retry = true;
				graphicsThread.setRunning(false);
				while(retry) {
					try {
						graphicsThread.join();
						retry = false;
					} catch(InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			
			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				graphicsThread.setRunning(true);
				graphicsThread.start();
			}
			
			@Override
			public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
				//things flicker in here...
			}
		});
	} //end init

	protected void reDraw(Canvas canvas, Box b, float[] coord) {
		
		//Background:
		paint.setColor(Color.BLACK);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(b.r1, b.r2, b.r3, b.r4, paint);
		
		//Text:
		paint.setColor(Color.GRAY);
	    paint.setTextSize(21);
	    canvas.drawText("FAST", 0, 30, paint);
	    canvas.drawText("SLOW", 0, 340, paint);
	    canvas.save();
	    canvas.rotate(-90, 25, 225);
	    paint.setTextSize(25);
	    canvas.drawText("SPEED", 25, 225, paint);
	    canvas.restore();
		
		//Axes:
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(2);
		canvas.drawLine(50, 360, 610, 360, paint); 	//x-axis
		canvas.drawLine(50, 0, 50, 360, paint);		//y-axis
		
		//Thresholds:
		paint.setColor(Color.DKGRAY);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(1);
		canvas.drawLine(50, 90, 610, 90, paint);	//high
		canvas.drawLine(50, 180, 610, 180, paint);	//mid
		canvas.drawLine(50, 270, 610, 270, paint);	//low
		
		//Lines:
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(5);
		for(int i = MainActivity.data.size()-1; i > 0; i--) {
			canvas.drawLine(coord[(2*i)-2], coord[(2*i)-1], coord[(2*i)], coord[(2*i)+1], paint);
		}
		
		//Header point:
		paint.setStrokeWidth(25);
		canvas.drawPoint(coord[MainActivity.data.size()*2-2], coord[MainActivity.data.size()*2-1], paint);
	    
	}//end reDraw
	
} //end class
