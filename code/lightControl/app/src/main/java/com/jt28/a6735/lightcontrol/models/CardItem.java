package com.jt28.a6735.lightcontrol.models;


import java.util.List;

public class CardItem {

    public List<BleDevice> mTextResource;
    public int mTitleResource;

    public CardItem(int title, List<BleDevice> text) {
        mTitleResource = title;
        mTextResource = text;
    }

    public List<BleDevice> getText() {
        return mTextResource;
    }
    public int getTitle() {
        return mTitleResource;
    }
}
