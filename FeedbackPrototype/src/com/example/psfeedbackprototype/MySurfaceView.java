/**
 * Quadrant Feedback System
 */
package com.example.psfeedbackprototype;

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
	
	protected void reDraw(Canvas canvas, Quadrant q) {
		//Text:
	    paint.setTextSize(25);
	    paint.setColor(Color.WHITE);
	    canvas.drawText("SPEED", 305, 350, paint);
		canvas.save();
	    canvas.rotate(-90, 30, 205);
	    canvas.drawText("VOLUME", 30, 205, paint);
	    canvas.restore();
	    paint.setTextSize(16);
	    paint.setColor(Color.GRAY);
	    canvas.drawText("SLOW", 55, 345, paint);
	    canvas.drawText("FAST", 580, 345, paint);
	    canvas.drawText("SOFT", 0, 300, paint);
	    canvas.drawText("LOUD", 0, 25, paint);
	    
		//Axes:
		paint.setColor(Color.GRAY);
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(3);
		canvas.drawLine(40, 320, 640, 320, paint);	//x-axis
		canvas.drawLine(40, 0, 40, 320, paint);		//y-axis
	    
	    //Quadrant
		int reColor = Color.rgb((int)q.R, (int)q.G, (int)q.B);
		paint.setColor(reColor);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(q.r1, q.r2, q.r3, q.r4, paint);
	}//end reDraw

} //end class
