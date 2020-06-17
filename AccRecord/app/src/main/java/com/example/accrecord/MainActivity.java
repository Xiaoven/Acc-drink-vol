package com.example.accrecord;

import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    private Button btn_new;
    private Button btn_stop;

    private AccListener listener;
    private SensorManager manager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn_new = (Button) findViewById(R.id.btn_new);
        btn_stop = (Button) findViewById(R.id.btn_stop);
        btn_stop.setEnabled(false);

        TextView tvText = findViewById(R.id.textView);

        // check
        String state = Environment.getExternalStorageState();
        if (!Environment.MEDIA_MOUNTED.equals(state)){
            Log.e("[MEDIA MOUNTED]","Cannot write to external storage");
        }

        listener = new AccListener(tvText);

        manager = (SensorManager) getSystemService(SENSOR_SERVICE);
        Sensor accelerator = manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        manager.registerListener(listener, accelerator,SensorManager.SENSOR_DELAY_FASTEST);
    }

    @Override
    protected void onStop(){
        super.onStop();
        manager.unregisterListener(listener);
    }

    public void newFile(View view){
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss");
        Date date = new Date(System.currentTimeMillis());

        File outFile = new File(getApplicationContext().getExternalFilesDir("")
                .getAbsolutePath()+"/" + simpleDateFormat.format(date) + ".txt");

        if (!outFile.exists()){
            try {
                outFile.createNewFile();
            }catch (IOException e) {
                Log.e("[Create File]", e.toString());
            }
        }

        listener.setFile(outFile);

        ((Button) view).setEnabled(false);
        btn_stop.setEnabled(true);
    }

    public void stopRecord(View view){
        listener.setFile(null);

        ((Button) view).setEnabled(false);
        btn_new.setEnabled(true);
    }

}
