package org.seed_up.usbblelockscreen;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.util.Log;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.UUID;

import static android.content.ContentValues.TAG;

/**
 * Created by barbare on 22/09/16.
 */

public class BleConnectThread extends Thread {
    private BluetoothGatt mmSocket;
    private final BluetoothAdapter mBluetoothAdapter;
    private final BluetoothDevice mmDevice;
    private BluetoothGattCharacteristic mTxCharacteristic;
    private BluetoothGatt myGatt;
    private double distance = 0;
    private int txPower = 6; //dbm get in hm10, configurate POWE3
    private int myrssi = -1;

    private int debounce = 0;
    private int moyrssi = 0;
    private int tmprssi = 0;

    public BleConnectThread(BluetoothDevice device, BluetoothAdapter mBluetoothAdapter, TextView info) {
        mmDevice = device;
        this.mBluetoothAdapter = mBluetoothAdapter;
    }

    public void run() {
        mBluetoothAdapter.cancelDiscovery();
        mmSocket = this.mmDevice.connectGatt(MainActivity.getLastSetContext(), true, new BluetoothGattCallback() {
            @Override
            public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
                myGatt = gatt;
                if (newState == BluetoothProfile.STATE_CONNECTED) {
                    gatt.readRemoteRssi();
                    Log.i(TAG, "Connected to GATT server.");
                    Log.i(TAG, "Attempting to start service discovery:" + gatt.discoverServices());
                    this.writeSendOnCharacteristic("test");
                } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                    Log.i(TAG, "Disconnected from GATT server.");
                }
            }
            public double calculateDistance( int txPower, double rssi ) {
                if (rssi == 0) return -1.0;
                double ratio = rssi * 1.0/ txPower;
                if (ratio < 1.0) return Math.pow(ratio,10);
                else return  (0.89976) * Math.pow(ratio,7.7095) + 0.111;
            }

            @Override
            public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
                myrssi = rssi;
                Log.d(TAG, "onReadRemoteRssi : " + Integer.toString(rssi));
                distance = calculateDistance(txPower, (double)rssi);
            }

            @Override
            public void  onServicesDiscovered(BluetoothGatt gatt, int status) {
                try {
                    if (status == BluetoothGatt.GATT_SUCCESS) {
                        Log.w(TAG, "Successfully discovered services");
                        ArrayList<String> lst = new ArrayList<>();
                        for (BluetoothGattService myGatt : gatt.getServices()) {
                            String x = myGatt.getUuid().toString() + " -> [";
                            for (BluetoothGattCharacteristic charac : myGatt.getCharacteristics()) {
                                if (charac == null) {
                                    Log.w(TAG, "WTF");
                                    continue;
                                }
                                if (charac.getUuid().equals(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"))) {
                                    mTxCharacteristic = charac;
                                    gatt.setCharacteristicNotification(charac, true);
                                }
                                x += charac.getUuid().toString() + ", ";
                            }
                            x += "]";
                            lst.add(x);
                        }
                        Log.w(TAG, lst.toString());
                        //broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
                    } else {
                        Log.w(TAG, "onServicesDiscovered received: " + status);
                    }
                } catch (Exception e) {
                    Log.e(TAG, Log.getStackTraceString(e));
                    throw e;
                }
            }
            @Override
            public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
                if (status == BluetoothGatt.GATT_SUCCESS) {
                    Log.d(TAG, "Read characteristic " + characteristic.getValue());
                }
            }
            @Override
            public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
                final byte[] data = characteristic.getValue();
                if (data != null && data.length > 0) {
                    String dataReceived = new String(data);
                    Log.d(TAG, dataReceived);
                    // To write :
                    if (dataReceived.equals("PING")) { // Renvoie OK
                        writeSendOnCharacteristic("PONG");
                    }
                    if (dataReceived.equals("PASS")) {
                        writeSendOnCharacteristic("qwerty");
                    }

                    gatt.readRemoteRssi();
                    //writeSendOnCharacteristic("test distance");
                }
            }

            public void writeSendOnCharacteristic(String val) {

               /* if (debounce < 10) {
                    tmprssi += myrssi;
                    debounce++;
                }
                else {
                    moyrssi = tmprssi / 10;
                    tmprssi = 0;
                    debounce = 0;
                }
                // Test ranged rssi
                if (moyrssi <= -67) {
                    return;
                }*/
                Log.d(TAG, "Test moyRSSI: " + Integer.toString(moyrssi));
                mTxCharacteristic.setValue(val);
                myGatt.writeCharacteristic(mTxCharacteristic);
            }
        });
    }

    public void cancel() {
        mmSocket.close();
    }

    public BluetoothGatt getSocket() {
        return mmSocket;
    }
}
