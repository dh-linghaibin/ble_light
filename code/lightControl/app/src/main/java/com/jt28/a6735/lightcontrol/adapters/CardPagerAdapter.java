package com.jt28.a6735.lightcontrol.adapters;

import android.app.Activity;
import android.support.v4.view.PagerAdapter;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.jt28.a6735.lightcontrol.R;
import com.jt28.a6735.lightcontrol.models.BleDevice;
import com.jt28.a6735.lightcontrol.models.CardItem;
import com.jt28.a6735.lightcontrol.widgets.CardAdapter;
import com.jt28.a6735.lightcontrol.widgets.OnItemClickListener;
import com.wuxiaolong.pullloadmorerecyclerview.PullLoadMoreRecyclerView;

import java.util.ArrayList;
import java.util.List;

public class CardPagerAdapter extends PagerAdapter implements CardAdapter {

    private List<CardView> mViews;
    private List<CardItem> mData;
    private float mBaseElevation;
    private PullLoadMoreRecyclerView recyclerView;
    private Activity mContext;
    private RecyclerView mRecyclerView;
    private BleDeviceAdapter mBleDeviceAdapter;
    //private List<BleDevice> arraylist;
    private OnItemClickListener mOnItemClickListener;

    public CardPagerAdapter(Activity context) {
        mData = new ArrayList<>();
        mViews = new ArrayList<>();
        mContext = context;
    }

    @Override
    public int getItemPosition(Object object) {
        // 最简单解决 notifyDataSetChanged() 页面不刷新问题的方法
        return POSITION_NONE;
    }

    public void addCardItem(CardItem item) {
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
    public void upData(int position,List<BleDevice> mTextResource) {
        mData.get(position).mTextResource = mTextResource;
        mBleDeviceAdapter.updateDataSet(mTextResource);
        notifyDataSetChanged();
        //mRecyclerView.scrollToPosition(mBleDeviceAdapter.getItemCount()-1);
//        mBleDeviceAdapter.notifyItemChanged(mBleDeviceAdapter.getItemCount()-1);
//        mRecyclerView.getLayoutManager().scrollToPosition(mBleDeviceAdapter.getItemCount()-1);
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == object;
    }

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        View view = LayoutInflater.from(container.getContext())
                .inflate(R.layout.adapter, container, false);
        container.addView(view);
        bind(mData.get(position), view);
        CardView cardView = (CardView) view.findViewById(R.id.cardView);

        if (mBaseElevation == 0) {
            mBaseElevation = cardView.getCardElevation();
        }

        cardView.setMaxCardElevation(mBaseElevation * MAX_ELEVATION_FACTOR);
        mViews.set(position, cardView);

        mBleDeviceAdapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                if (mOnItemClickListener != null) {
                    mOnItemClickListener.onItemClick(view, position);
                }
            }
        });

        return view;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        container.removeView((View) object);
        mViews.set(position, null);
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mOnItemClickListener = listener;
    }

    private void bind(CardItem item, View view) {
        TextView titleTextView = (TextView) view.findViewById(R.id.titleTextView);
        TextView contentTextView = (TextView) view.findViewById(R.id.contentTextView);
        titleTextView.setText(item.getTitle());
        contentTextView.setText("test");//item.getText());

        recyclerView = view.findViewById(R.id.recyclerView);
        recyclerView.setLinearLayout();
        //获取mRecyclerView对象
        mRecyclerView = recyclerView.getRecyclerView();
        //代码设置scrollbar无效？未解决！
        mRecyclerView.setVerticalScrollBarEnabled(true);
        //设置下拉刷新是否可见
        recyclerView.setRefreshing(false);
        //设置是否可以下拉刷新
        recyclerView.setPullRefreshEnable(false);
        //设置是否可以上拉刷新
        recyclerView.setPushRefreshEnable(false);
        //显示下拉刷新
        recyclerView.setRefreshing(false);
        //设置上拉刷新文字
        //recyclerView.setFooterViewText("loading");
        //设置上拉刷新文字颜色
        //mPullLoadMoreRecyclerView.setFooterViewTextColor(R.color.white);
        //设置加载更多背景色
        //mPullLoadMoreRecyclerView.setFooterViewBackgroundColor(R.color.colorBackground);
        recyclerView.setLinearLayout();

        recyclerView.setGridLayout(3);//参数为列数

        recyclerView.setOnPullLoadMoreListener(new PullLoadMoreRecyclerView.PullLoadMoreListener() {
            @Override
            public void onRefresh() {
                recyclerView.setPullLoadMoreCompleted();
            }

            @Override
            public void onLoadMore() {
                recyclerView.setPullLoadMoreCompleted();
            }
        });
//        arraylist = new ArrayList<>();
//        for(int i = 0; i < 7;i++) {
//            arraylist.add(new BleDevice());
//        }
//        arraylist.add(new BleDevice(1,1,1));
//        arraylist.add(new BleDevice(2,2,2));
        mBleDeviceAdapter = new BleDeviceAdapter(mContext,item.getText());
        recyclerView.setAdapter(mBleDeviceAdapter);
    }

}
