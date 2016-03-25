/**
 * Words Feedback System
 */
package com.example.pswordsfeedback;

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
	
	static String wordVolume = "", wordSpeed = "";
	static String tempVol = "", tempSpeed = "";
	
	static int bgColor = Color.BLACK;
	
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
	
	public static void setBGcolor(int color) {
		bgColor = color;
	}
	
	public static void setWordVolume(String wordVol) {
		wordVolume = wordVol;
	}
	
	public static void setWordSpeed(String wordSpd) {
		wordSpeed = wordSpd;
	}
	
	public static void setTempWords(String wordV, String wordS) {
		tempVol = wordV;
		tempSpeed = wordS;
	}
	
//	public static void setTempVolume(String wordV) {
//		tempVol = wordV;
//	}
//	public static void setTempSpeed(String wordS) {
//		tempSpeed = wordS;
//	}
	
	public static String getWordVolume() {
		return wordVolume;
	}
	
	public static String getWordSpeed() {
		return wordSpeed;
	}
	
	public static String getTempVolume() {
		return tempVol;
	}
	
	public static String getTempSpeed() {
		return tempSpeed;
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
		paint.setColor(bgColor);
		paint.setStyle(Paint.Style.FILL);
		canvas.drawRect(b.r1, b.r2, b.r3, b.r4, paint);
		
		//Text:
		paint.setColor(Color.WHITE);
	    paint.setTextSize(85);
	    canvas.drawText(wordVolume, 40, 130, paint);
	    canvas.drawText(wordSpeed, 40, 280, paint);
	}//end reDraw
	
} //end class
