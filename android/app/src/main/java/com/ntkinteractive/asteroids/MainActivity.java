package com.ntkinteractive.asteroids;

import android.os.Bundle;
import android.view.SurfaceHolder;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "Asteroids";

    static {
        System.loadLibrary("game");
    }

    private SurfaceHolder surfaceHolder;

    public native String HelloFromNDK();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}