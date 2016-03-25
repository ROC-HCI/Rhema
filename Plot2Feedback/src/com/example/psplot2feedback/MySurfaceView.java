/**
 * Plot2 Feedback System
 */
package com.example.psplot2feedback;

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
	
	protected void reDraw(Canvas canvas, Box b) {
		
		//Background:
		paint.setColor(Color.BLACK);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(b.r1, b.r2, b.r3, b.r4, paint);
		
		//Text:
		paint.setColor(Color.DKGRAY);
	    paint.setTextSize(21);
	    canvas.drawText("Loud", 0, 20, paint);
	    canvas.drawText("Soft", 0, 170, paint);
	    canvas.drawText("Fast", 0, 205, paint);
	    canvas.drawText("Slow", 0, 360, paint);
	    canvas.save();
	    canvas.rotate(-90, 30, 135);
	    paint.setColor(Color.GRAY);
	    paint.setTextSize(23);
	    canvas.drawText("VOLUME", 30, 135, paint);
	    canvas.restore();
	    canvas.save();
	    canvas.rotate(-90, 30, 310);	
	    canvas.drawText("SPEED", 30, 310, paint); 
	    canvas.restore();
		
		//Dividers:
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(2);
		canvas.drawLine(0, 180, 610, 180, paint);
		canvas.drawLine(50, 0, 50, 360, paint);		//y-axis
		
		//Inner Axes:
		paint.setColor(Color.GRAY);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(1);
		canvas.drawLine(50, 90, 610, 90, paint);	//x-axis volume
		canvas.drawLine(50, 270, 610, 270, paint);	//x-axis speed
		
		//Thresholds:
		paint.setColor(Color.DKGRAY);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(1);
		canvas.drawLine(50, 50, 610, 50, paint);	//volume top
		canvas.drawLine(50, 130, 610, 130, paint);	//volume bottom
		canvas.drawLine(50, 230, 610, 230, paint);	//speed top
		canvas.drawLine(50, 310, 610, 310, paint);	//speed bottom
		
		//Lines for volume:
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(5);
		for(int i = MainActivity.dataVolume.size()-1; i > 0; i--) {
			canvas.drawLine((float)GraphicsThread.coordVolume[(2*i)-2], (float)GraphicsThread.coordVolume[(2*i)-1], 
					(float)GraphicsThread.coordVolume[(2*i)], (float)GraphicsThread.coordVolume[(2*i)+1], paint);
		}
		paint.setStrokeWidth(25);
	    canvas.drawPoint((float)GraphicsThread.coordVolume[MainActivity.dataVolume.size()*2-2], 
	    		(float)GraphicsThread.coordVolume[MainActivity.dataVolume.size()*2-1], paint);

		//Lines for speed:
		paint.setColor(Color.WHITE);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(5);
		for(int i = MainActivity.dataSpeed.size()-1; i > 0; i--) {
			canvas.drawLine((float)GraphicsThread.coordSpeed[(2*i)-2], (float)GraphicsThread.coordSpeed[(2*i)-1], 
					(float)GraphicsThread.coordSpeed[(2*i)], (float)GraphicsThread.coordSpeed[(2*i)+1], paint);
		}
		paint.setStrokeWidth(25);
	    canvas.drawPoint((float)GraphicsThread.coordSpeed[MainActivity.dataSpeed.size()*2-2], 
	    		(float)GraphicsThread.coordSpeed[MainActivity.dataSpeed.size()*2-1], paint);
	  
	}//end reDraw
	
} //end class