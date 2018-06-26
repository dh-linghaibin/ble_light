package com.jt28.a6735.lightcontrol.adapters;

import android.support.v4.view.PagerAdapter;
import android.support.v7.widget.CardView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.jt28.a6735.lightcontrol.R;
import com.jt28.a6735.lightcontrol.models.BleCardItem;
import com.jt28.a6735.lightcontrol.models.BleDevice;
import com.jt28.a6735.lightcontrol.widgets.BleConnect;
import com.jt28.a6735.lightcontrol.widgets.CardAdapter;

import java.util.ArrayList;
import java.util.List;

public class BlePagerAdapter extends PagerAdapter implements CardAdapter {

    private List<CardView> mViews;
    private List<BleCardItem> mData;
    private float mBaseElevation;
    private BleConnect mBleConnect;

    public BlePagerAdapter() {
        mData = new ArrayList<>();
        mViews = new ArrayList<>();
    }

    public void addCardItem(BleCardItem item) {
        mViews.add(null);
        mData.add(item);
    }

    public float getBaseElevation() {
        return mBaseElevation;
    }

    @Override
    public CardView getCardViewAt(int position) {
        return mViews.get(position);
    }

    @Override
    public int getCount() {
        return mData.size();
    }

    @Override
    public void upData(int position, List<BleDevice> mTextResource) {

    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == object;
    }

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        View view = LayoutInflater.from(container.getContext())
                .inflate(R.layout.ble_page_adapter, container, false);
        container.addView(view);
        bind(mData.get(position), view);
        CardView cardView = (CardView) view.findViewById(R.id.cardView);

        if (mBaseElevation == 0) {
            mBaseElevation = cardView.getCardElevation();
        }

        cardView.setMaxCardElevation(mBaseElevation * MAX_ELEVATION_FACTOR);
        mViews.set(position, cardView);
        return view;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        container.removeView((View) object);
        mViews.set(position, null);
    }

    public void bleConnect(BleConnect _bleConnect) {
        this.mBleConnect = _bleConnect;
    }

    private void bind(BleCardItem item, View view) {
        TextView titleTextView = (TextView) view.findViewById(R.id.titleTextView);
        final TextView contentTextView = (TextView) view.findViewById(R.id.contentTextView);
        Button connectButton = view.findViewById(R.id.connectButton);
        titleTextView.setText(item.getTitle());
        contentTextView.setText(item.getText());
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(mBleConnect != null) {
                    mBleConnect.connect(contentTextView.getText().toString());
                }
            }
        });
    }

}
