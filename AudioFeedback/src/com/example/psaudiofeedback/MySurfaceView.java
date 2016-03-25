/**
 * Audio Feedback System
 */
package com.example.psaudiofeedback;

import android.content.Context;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MySurfaceView extends SurfaceView {
	private SurfaceHolder surfaceHolder;
	static AudioThread audioThread;
	
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
		audioThread = new AudioThread(1);
		surfaceHolder = getHolder();
		
		surfaceHolder.addCallback(new SurfaceHolder.Callback() {
			
			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				boolean retry = true;
				audioThread.setRunning(false);
				while(retry) {
					try {
						audioThread.join();
						retry = false;
					} catch(InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			
			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				audioThread.setRunning(true);
				audioThread.start();
			}
			
			@Override
			public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {}
		});
	} //end init
	
} //end class
