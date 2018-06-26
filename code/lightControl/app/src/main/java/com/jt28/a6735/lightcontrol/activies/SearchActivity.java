package com.jt28.a6735.lightcontrol.activies;

import android.Manifest;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.Toast;

import com.jt28.a6735.ble.BleController;
import com.jt28.a6735.ble.callback.ConnectCallback;
import com.jt28.a6735.ble.callback.OnReceiverCallback;
import com.jt28.a6735.ble.callback.OnWriteCallback;
import com.jt28.a6735.ble.callback.ScanCallback;
import com.jt28.a6735.lightcontrol.R;
import com.jt28.a6735.lightcontrol.adapters.BlePagerAdapter;
import com.jt28.a6735.lightcontrol.adapters.CardFragmentPagerAdapter;
import com.jt28.a6735.lightcontrol.models.BleCardItem;
import com.jt28.a6735.lightcontrol.timely.ShadowTransformer;
import com.jt28.a6735.lightcontrol.widgets.BleConnect;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class SearchActivity extends AppCompatActivity{
    private BleController mBleController;
    private ViewPager mViewPager;
    private BlePagerAdapter mCardAdapter;
    private ShadowTransformer mCardShadowTransformer;
    private CardFragmentPagerAdapter mFragmentCardAdapter;
    private ShadowTransformer mFragmentCardShadowTransformer;

    public static boolean isGpsEnable(final Context context) {
        LocationManager locationManager
                = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
        assert locationManager != null;
        boolean gps = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        boolean network = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        return gps || network;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search);
        //Android6.0需要动态申请权限
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {
            //请求权限
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.ACCESS_COARSE_LOCATION,
                            Manifest.permission.ACCESS_FINE_LOCATION},
                    3);
            ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.ACCESS_COARSE_LOCATION);
        }

        if(isGpsEnable(this)) {
            mBleController = BleController.getInstance().init(this);
            BleSearch();
        } else {
            //跳转到gps设置页
            Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
            startActivityForResult(intent, 0);
        }

        mViewPager = findViewById(R.id.viewPager);
        mCardAdapter = new BlePagerAdapter();
        mCardAdapter.bleConnect(new BleConnect() {
            @Override
            public void connect(String address) {
                mBleController.connect(0, address, new ConnectCallback() {
                    @Override
                    public void onConnSuccess() {
                        Toast.makeText(SearchActivity.this, "连接成功", Toast.LENGTH_SHORT).show();
                        //toMainActivity();
                        // TODO 接收数据的监听
                        mBleController.registReciveListener("SearchActivity", new OnReceiverCallback() {
                            @Override
                            public void onRecive(byte[] value) {
                                Log.d("lhb","dat+  " + Arrays.toString(value));
                            }
                        });
                        handler.postDelayed(runnable, 2000);//每两秒执行一次runnable.
                    }

                    @Override
                    public void onConnFailed() {
                        Toast.makeText(SearchActivity.this, "连接超时，请重试", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });
    }

    Handler handler=new Handler();
    Runnable runnable=new Runnable() {
        @Override
        public void run() {
            // TODO Auto-generated method stub
            //要做的事情
            mBleController.writeBuffer(new byte[1], new OnWriteCallback() {
                @Override
                public void onSuccess() {
                    Log.d("lhb","发送成功");
                }

                @Override
                public void onFailed(int state) {
                    Log.d("lhb","发送失败");
                }
            });

            handler.postDelayed(this, 2000);
        }
    };


    public static float dpToPixels(int dp, Context context) {
        return dp * (context.getResources().getDisplayMetrics().density);
    }

    private void toMainActivity() {
        Intent intent = new Intent();
        intent.setComponent(new ComponentName(SearchActivity.this, MainActivity.class));
        startActivity(intent);
    }

    private List<BluetoothDevice> bluetoothDevices = new ArrayList<>();
    private void BleSearch() {
        mBleController.scanBle(0, new ScanCallback() {
            @Override
            public void onSuccess() {
                if (bluetoothDevices.size() > 0) {
                    for(BluetoothDevice tmp: bluetoothDevices) {
                        Log.d("lhb","HA--"+tmp.getName() + tmp.getType() + tmp.getAddress());
                        mCardAdapter.addCardItem(new BleCardItem(" " + tmp.getName(), tmp.getAddress()));
                    }

                    mFragmentCardAdapter = new CardFragmentPagerAdapter(getSupportFragmentManager(),
                            dpToPixels(2, SearchActivity.this));

                    mCardShadowTransformer = new ShadowTransformer(mViewPager, mCardAdapter);
                    mFragmentCardShadowTransformer = new ShadowTransformer(mViewPager, mFragmentCardAdapter);

                    mViewPager.setAdapter(mCardAdapter);
                    mViewPager.setPageTransformer(false, mCardShadowTransformer);
                    mViewPager.setOffscreenPageLimit(3);
                }
            }

            @Override
            public void onScanning(BluetoothDevice device, int rssi, byte[] scanRecord) {
                if (!bluetoothDevices.contains(device)) {
                    bluetoothDevices.add(device);
                }
            }
        });
    }
}
