package org.seed_up.usbblelockscreen;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

/**
 * Created by barbare on 23/09/16.
 */
public class BluetoothDeviceArrayAdapter extends ArrayAdapter<BluetoothDevice> {

    // Vars
    private LayoutInflater mInflater;

    public BluetoothDeviceArrayAdapter(Context context, List<BluetoothDevice> objects) {
        super(context, 0, objects);
        init(context);
    }

    // Headers
    public void drawText(TextView textView, BluetoothDevice object) {
        textView.setText(object.getName());
    }

    private void init(Context context) {
        this.mInflater = LayoutInflater.from(context);
    }

    @Override public View getView(int position, View convertView, ViewGroup parent) {
        final ViewHolder vh;
        if (convertView == null) {
            convertView = mInflater.inflate(android.R.layout.simple_list_item_1, parent, false);
            vh = new ViewHolder(convertView);
            convertView.setTag(vh);
        } else {
            vh = (ViewHolder) convertView.getTag();
        }

        drawText(vh.textView, getItem(position));

        return convertView;
    }

    static class ViewHolder {

        TextView textView;

        private ViewHolder(View rootView) {
            textView = (TextView) rootView.findViewById(android.R.id.text1);
        }
    }
}