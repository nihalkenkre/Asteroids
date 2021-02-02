package com.ntkinteractive.asteroids;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.DragEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class VkSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
    private final static String TAG = "Asteroids";

    private int green = 0;
    private int blue = 0;

    static {
        System.loadLibrary("game");
    }

    private native void GameInit (Surface surface, AssetManager assetManager);
    private native void GameUpdate();
    private native void GameSubmitPresent();
    private native void GameShutdown ();

    private final Context ctx;

    public VkSurfaceView(Context context) {
        super(context);
        setWillNotDraw(false);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setWillNotDraw(false);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setWillNotDraw(false);

        ctx = context;
    }

    public VkSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        setWillNotDraw(false);

        ctx = context;
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        getHolder().addCallback(this);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        GameUpdate();
        GameSubmitPresent();
        invalidate();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        GameInit(holder.getSurface(), ctx.getAssets());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        GameShutdown();
    }
}