package com.jt28.a6735.lightcontrol.models;


public class BleCardItem {

    private String mTextResource;
    private String mTitleResource;

    public BleCardItem(String _title, String _text) {
        mTitleResource = _title;
        mTextResource = _text;
    }

    public String getText() {
        return mTextResource;
    }
    public String getTitle() {
        return mTitleResource;
    }
}
