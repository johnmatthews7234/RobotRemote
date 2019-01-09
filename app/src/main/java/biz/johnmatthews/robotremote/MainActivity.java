package biz.johnmatthews.robotremote;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbDevice;


import java.util.HashMap;
import java.util.Map;


public class MainActivity extends AppCompatActivity {
    public final String ACTION_USB_PERMISSION = "biz.johnmatthews.robotremote.USB_PERMISSION";
    UsbManager usbManager;
    UsbDevice device;
    UsbDeviceConnection deviceConnection;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClickStop(View view) {
    }

    public void onClickStart(View view) {
        HashMap <String, UsbDevice> usbDevices = usbManager.getDeviceList();
        if (!usbDevices.isEmpty()){
            boolean keep = true;
            for (Map.Entry entry :usbDevices.entrySet()) {
                device = (UsbDevice) entry.getValue();
                int deviceVID = device.getVendorId();
                if (deviceVID == 0x2341) {
                    PendingIntent pi = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
                    usbManager.requestPermission(device, pi);
                    keep = false;
                } else {
                    deviceConnection = null;
                    device = null;
                }
                if (!keep) {
                    break;
                }

            }
        }
    }

    public void onClickSend(View view) {
    }

    public void onClickClear(View view) {
    }
}
