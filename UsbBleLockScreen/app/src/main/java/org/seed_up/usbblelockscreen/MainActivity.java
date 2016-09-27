package org.seed_up.usbblelockscreen;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    //Lock action
    boolean isLock = false;
    Thread threadRead;

    //Views
    ListView listViewBleScan;
    Button btnConnectBle;
    List<BluetoothDevice> devices = new ArrayList<>();
    TextView infoText;

    //Ble
    private final static int REQUEST_CODE_ENABLE_BLUETOOTH = 0;
    private BluetoothAdapter bluetoothAdapter;
    BluetoothDevice device;
    IntentFilter filter;

    //Thread
    BleConnectThread mConnectThread;
    BleConnectedThread mConnectedThread;
    public static Context ctx;


    public static Context getLastSetContext() {
        return ctx;
    }


    Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            byte[] writeBuf = (byte[]) msg.obj;
            int begin = (int) msg.arg1;
            int end = (int) msg.arg2;

            switch (msg.what) {
                case 1:
                    String writeMessage = new String(writeBuf);
                    writeMessage = writeMessage.substring(begin, end);
                    break;
            }
        }
    };

    private final BroadcastReceiver bluetoothReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            long startAt;
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice blueDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                Log.d("TOTO", blueDevice.getName() + " " + blueDevice.getAddress());
                for (BluetoothDevice dev : devices) {
                    if (dev.getAddress() == blueDevice.getAddress())
                        return;
                }
                Log.d("BroadcastR", "Received Safelock " + blueDevice.getName());
                if (blueDevice != null && blueDevice.getName() != null && blueDevice.getName().equals("Safelock")) {
                    Log.d("connect socket", "Received Safelock " + blueDevice.getName());
                    mConnectThread = new BleConnectThread(blueDevice, bluetoothAdapter, infoText);
                    mConnectThread.start();
                    startAt = System.currentTimeMillis();
                    while(mConnectThread.getSocket() == null || System.currentTimeMillis() < startAt + 20000) {
                        //Log.d("Rech.Device.Connect", "socket not connected");
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                    if ((mConnectThread.getSocket() == null))
                        Log.d("Rech.Device.Connect", "Tes foutu tu trouvera jamais");
                    else
                        Log.d("Rech.Device.Connect", "Socket Connected");
                }
                devices.add(blueDevice);
                ((BluetoothDeviceArrayAdapter)listViewBleScan.getAdapter()).notifyDataSetChanged();
            }

            if (devices.isEmpty())
                btnConnectBle.setEnabled(false);
            else
                btnConnectBle.setEnabled(true);
        }
    };

    public enum e_info {
        INFO_WARNING,
        INFO_ERROR
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ctx = this;
        setContentView(R.layout.activity_main);
        listViewBleScan = (ListView) findViewById(R.id.ViewBleScan);
        btnConnectBle = (Button) findViewById(R.id.btnConnectBle);
        infoText = (TextView) findViewById(R.id.InfoText);
        filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(bluetoothReceiver, filter);
        BluetoothDeviceArrayAdapter arrayAdapter = new BluetoothDeviceArrayAdapter(this, devices);
        listViewBleScan.setAdapter(arrayAdapter);
        listViewBleScan.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long arg3) {
                view.setSelected(true);
                for (BluetoothDevice dev : devices) {
                    if (dev.getAddress() == listViewBleScan.getItemAtPosition(position).toString())
                        device = dev;
                }
            }
        });
        ble();
    }

    @Override
    protected void onResume() {
        super.onResume();
        //listViewBleScan.set;
        if (devices.isEmpty())
            btnConnectBle.setEnabled(false);
        else {
            btnConnectBle.setEnabled(true);
            infoText.setText("BLE Scanned");
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (!bluetoothAdapter.isEnabled())
            bluetoothAdapter.enable();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        bluetoothAdapter.cancelDiscovery();
        unregisterReceiver(bluetoothReceiver);
    }

    // =============================================================================================
    //START Action Activity
    public void showSettingsActivity(View v) {
        Intent settingsActivity = new Intent(this, Settings.class);
        startActivity(settingsActivity);
    }

    public void scanBle(View v) {
        infoText.setText("BLE Scanning");
        ble();
        bluetoothAdapter.startDiscovery();
    }

    public void sendData(View v) {
    }



    public void connectBle(View v) {
        mConnectThread = new BleConnectThread(device, bluetoothAdapter, infoText);
        mConnectThread.start();
        /*if (!mConnectThread.getSocket().isConnected()) {
            return ;
        }*/
        /*try {
            mConnectedThread = new BleConnectedThread(mConnectThread.getSocket(), mHandler, infoText);
        } catch (IOException e) {
        }
        mConnectedThread.start();*/
        /*if (mConnectThread.isAlive()) {
            Intent connectedActivity = new Intent(this, Connected.class);
            startActivity(connectedActivity);
        }*/
        listViewBleScan.setVisibility(ListView.INVISIBLE);
    }
    //END Action Activity
    // =============================================================================================

    protected void setInfoMsg(e_info info, String err) {
        this.infoText.setText(err);
        if (info == e_info.INFO_ERROR) {
            this.infoText.setTextColor(0x009A0010); //RED
        } else if (info == e_info.INFO_WARNING) {
            this.infoText.setTextColor(0x00E88C0C); // Yellow
        }
    }

    protected void setResetInfoMsg() {
        this.infoText.setText("");
    }

    protected void ble() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            infoText.setText("Votre appareil n'est pas compatible: Pas de fonction bluetooth !");
            return ;
        }
        else if (!bluetoothAdapter.isEnabled()) {
            Intent enableBlueTooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBlueTooth, REQUEST_CODE_ENABLE_BLUETOOTH);
        }
       // devices.addAll(bluetoothAdapter.getBondedDevices());
    }
}
