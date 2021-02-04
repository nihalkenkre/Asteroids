package com.ntkinteractive.asteroids;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class JoystickView extends SurfaceView implements SurfaceHolder.Callback, Runnable {

    static {
        System.loadLibrary("game");
    }

    private static final String TAG = "Asteroids";

    private Thread drawingThread = null;
    private boolean shouldDraw = false;

    private int width;
    private int height;

    private float centerX;
    private float centerY;

    private Paint ballPaint = new Paint();
    private Paint backgroundPaint = new Paint();

    private float ballDrawX;
    private float ballDrawY;

    private native void UpdateMovementInput(float moveForwardBack, float turnRightLeft);

    public JoystickView(Context context) {
        super(context);
    }

    public JoystickView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public JoystickView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public JoystickView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        getHolder().addCallback(this);

        backgroundPaint.setStyle(Paint.Style.FILL);
        backgroundPaint.setColor(Color.BLUE);

        ballPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        ballPaint.setColor(Color.RED);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        width = getWidth();
        height = getHeight();

        centerX = (float) width / 2;
        centerY = (float) height / 2;
        ballDrawX = centerX;
        ballDrawY = centerY;

        shouldDraw = true;

        Thread drawingThread = new Thread(this);
        drawingThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        shouldDraw = false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_MOVE || event.getAction() == MotionEvent.ACTION_DOWN) {
            ballDrawX = Math.max (0, Math.min (width, event.getX()));
            ballDrawY = Math.max (0, Math.min (height, event.getY()));

            return true;
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            ballDrawX = centerX;
            ballDrawY = centerY;

            return true;
        }

        return false;
    }

    @Override
    public void run() {
        while (shouldDraw) {
            Canvas canvas = getHolder().lockCanvas();
            canvas.drawARGB(0, 0, 0, 0);
            canvas.drawCircle(width / 2, height / 2, width / 2, backgroundPaint);
            canvas.drawCircle(ballDrawX, ballDrawY, 50, ballPaint);
            getHolder().unlockCanvasAndPost(canvas);

            float moveForwardBack = (centerY - ballDrawY) / height * 2.5f;
            float turnRightLeft = (centerX - ballDrawX) / width * 1.f;

            UpdateMovementInput(moveForwardBack, turnRightLeft);
        }
    }
}