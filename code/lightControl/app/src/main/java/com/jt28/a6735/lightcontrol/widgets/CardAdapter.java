package com.jt28.a6735.lightcontrol.widgets;


import android.support.v7.widget.CardView;

import com.jt28.a6735.lightcontrol.models.BleDevice;

import java.util.List;

public interface CardAdapter {

    int MAX_ELEVATION_FACTOR = 8;

    float getBaseElevation();

    CardView getCardViewAt(int position);

    int getCount();

    void upData(int position,List<BleDevice> mTextResource);
}
