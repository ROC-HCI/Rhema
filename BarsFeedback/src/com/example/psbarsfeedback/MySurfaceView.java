/**
 * Bars Feedback System
 */
package com.example.psbarsfeedback;

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
		int reColor = Color.rgb((int)b.R, (int)b.G, (int)b.B);
		paint.setColor(reColor);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(b.r1, b.r2, b.r3, b.r4, paint);
		
		//Text:
		paint.setColor(Color.WHITE);
	    paint.setTextSize(36);
//	    canvas.drawText("Volume", 110, 35, paint);	//vertical
//		canvas.drawText("Speed", 420, 35, paint);	//vertical
	    canvas.drawText("Volume", 90, 35, paint);	//horizontal
	    canvas.drawText("Speed", 410, 35, paint);	//horizontal
	    
	    paint.setColor(Color.GRAY);
	    paint.setTextSize(24);
//		canvas.drawText("Loud", 60, 70, paint);		//vertical
//		canvas.drawText("Soft", 60, 340, paint);	//vertical
//		canvas.drawText("Fast", 530, 70, paint);	//vertical
//		canvas.drawText("Slow", 530, 340, paint);	//vertical
	    canvas.drawText("Loud", 40, 70, paint);		//horizontal
	    canvas.drawText("Soft", 40, 340, paint);	//horizontal
	    canvas.drawText("Fast", 565, 125, paint);	//horizontal
	    canvas.drawText("Slow", 305, 125, paint);	//horizontal
	}//end reDraw
	
} //end class
