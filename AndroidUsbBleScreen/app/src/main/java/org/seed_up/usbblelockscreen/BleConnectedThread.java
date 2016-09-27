package org.seed_up.usbblelockscreen;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BleConnectedThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private final Handler mHandler;
    TextView dataView = null;

    public BleConnectedThread (BluetoothSocket socket, Handler handler, TextView test) throws IOException {
        dataView = test;
        mmSocket = socket;
        mHandler = handler;
            mmInStream = socket.getInputStream();
            mmOutStream = socket.getOutputStream();
        if (mmInStream == null || mmOutStream == null)
            Log.d("Exception mmOutStream", "Impossible de recuperer le InStream");
    }

    public void run() {
        byte[] buffer = new byte[1024];
        int begin = 0, bytes = 0;
        Log.d("Test socket", Boolean.toString(mmSocket.isConnected()));
        while (true) {
            try {
                bytes += mmInStream.read(buffer, bytes, buffer.length - bytes);
                for (int i = begin; i < bytes; i++) {
                    if (buffer[i] == "#".getBytes()[0]) {

                        if (dataView != null)
                            dataView.setText(buffer.toString());
                        mHandler.obtainMessage(1, begin, i, buffer).sendToTarget();
                        begin = i + 1;
                        if (i == bytes - 1) {
                            bytes = 0;
                            begin = 0;
                        }
                    }
                }
            } catch (IOException e) {
                break ;
            }
        }
    }

    public void write(byte[] bytes) {
        Log.d("socket methode write", Boolean.toString(this.mmSocket.isConnected()));
        try {
            mmOutStream.write(bytes);
        } catch (IOException e) {}
    }

    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {}
    }
}
