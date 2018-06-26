package com.jt28.a6735.lightcontrol.models;

public class BleDevice {
    public final int type;
    public final int address;
    public final int val;

    public BleDevice() {
        this.type = 0;
        this.address = 0;
        this.val = 0;
    }

    public BleDevice(int _type,int _address,int _val) {
        this.type = _type;
        this.address = _address;
        this.val = _val;
    }
}
