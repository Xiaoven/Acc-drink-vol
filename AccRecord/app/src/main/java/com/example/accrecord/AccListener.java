package com.example.accrecord;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.widget.TextView;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class AccListener implements SensorEventListener {
    private File outFile;
    private TextView textView;

    private final int UPTATE_INTERVAL_TIME = 10;

    private long lastUpdateTime;

    public AccListener(TextView tv){
        outFile = null;
        textView = tv;

        lastUpdateTime = System.currentTimeMillis();
    }

    public void setFile(File f){
        outFile = f;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        // check update time
        long currentUpdateTime = System.currentTimeMillis();
        if (currentUpdateTime - lastUpdateTime < UPTATE_INTERVAL_TIME)
            return;
        lastUpdateTime = currentUpdateTime;

        // get values
        float accX = sensorEvent.values[0];
        float accY = sensorEvent.values[1];
        float accZ = sensorEvent.values[2];

        // update text view
        textView.setText(String.format("%f,%f,%f", accX, accY, accZ));

        // write file
        if (outFile == null){
            return;
        }

        FileWriter writer = null;
        try {
            writer = new FileWriter(outFile, true);
            writer.write(String.format("%d,%f,%f,%f\n",lastUpdateTime, accX, accY, accZ));
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if(writer != null){
                    writer.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}
