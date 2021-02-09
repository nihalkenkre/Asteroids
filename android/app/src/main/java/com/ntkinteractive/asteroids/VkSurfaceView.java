package com.ntkinteractive.asteroids;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class VkSurfaceView extends SurfaceView implements SurfaceHolder.Callback, Runnable {
    private final static String TAG = "Asteroids";

    private Surface surface;
    private boolean surfaceAvailable = false;
    private int tickRate = 15;

    private long start;

    private int green = 0;
    private int blue = 0;

    static {
        System.loadLibrary("game");
    }

    private native void GameInit(Surface surface, AssetManager assetManager);

    private native void GameUpdate(long deltaTime);

    private native void GameSubmitPresent();

    private native void GameShutdown();

    private native void GameAddAsteroid();

    private final Context ctx;

    public VkSurfaceView(Context context) {
        super(context);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);

        ctx = context;
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        surface = holder.getSurface();
        surfaceAvailable = true;

        start = System.currentTimeMillis();

        Thread surfaceDrawThread = new Thread(this);
        surfaceDrawThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        surfaceAvailable = false;
        GameShutdown();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            GameAddAsteroid();

            return true;
        }

        return false;
    }

    @Override
    public void run() {
        GameInit(surface, ctx.getAssets());
        while (surfaceAvailable) {
            long now = System.currentTimeMillis();

            long deltaTimeMsecs = now - start;

            if (deltaTimeMsecs >= tickRate) {
                GameUpdate(deltaTimeMsecs);
                start = now;
            }

            GameSubmitPresent();
        }
    }
}