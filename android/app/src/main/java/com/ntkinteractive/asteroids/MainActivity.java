package com.ntkinteractive.asteroids;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "Asteroids";

    static {
        System.loadLibrary("game");
    }

    private SurfaceHolder surfaceHolder;

    public native String HelloFromNDK();

    private native void GameInit(Surface surface, AssetManager assetManager);

    private native void GameUpdate();

    private native void GameSubmitPresent();

    private native void GameShutdown();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();

        surfaceHolder.addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                Surface surface = holder.getSurface();
                Log.d(TAG, "surfaceCreated: " + surface.isValid());

                GameInit(surface, getAssets());
                GameUpdate();
                GameSubmitPresent();
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                Log.d(TAG, "surfaceDestroyed:");
                GameShutdown();
            }
        });

        final Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                button.setText(HelloFromNDK());
            }
        });
    }
}