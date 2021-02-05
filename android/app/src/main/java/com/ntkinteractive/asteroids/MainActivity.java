package com.ntkinteractive.asteroids;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "Asteroids";

    static {
        System.loadLibrary("game");
    }

    private SurfaceHolder surfaceHolder;

    public native String HelloFromNDK();

    private native void UpdateTriggerInput(boolean fire);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ImageView triggerView = findViewById(R.id.triggerView);

        triggerView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    UpdateTriggerInput(true);

                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP) {
                    UpdateTriggerInput(false);

                    return true;
                }

                return false;
            }
        });
    }
}