package com.jt28.a6735.lightcontrol.adapters;

import android.app.Activity;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.jt28.a6735.lightcontrol.R;
import com.jt28.a6735.lightcontrol.models.BleDevice;
import com.jt28.a6735.lightcontrol.widgets.OnItemClickListener;

import java.util.List;

public class TimePageAdapter extends RecyclerView.Adapter<TimePageAdapter.ItemHolder> {
    private List<BleDevice> arraylist;
    private Activity mContext;
    private OnItemClickListener mOnItemClickListener;

    public TimePageAdapter(Activity context, List<BleDevice> arraylist) {
        this.arraylist = arraylist;
        this.mContext = context;
    }

    @Override
    public ItemHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View v = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.item_ble_device, null);
        ItemHolder ml = new ItemHolder(v);
        return ml;
    }

    @Override
    public void onBindViewHolder(final ItemHolder itemHolder, int i) {
        BleDevice localItem = arraylist.get(i);
        switch (localItem.type) {
            case 0:
                //itemHolder.type.setBackgroundResource( R.drawable.item_black_broad );
                itemHolder.type.setText("黑板灯");
                itemHolder.content.setVisibility(View.VISIBLE);
                break;
            case 1:
                //itemHolder.type.setBackgroundResource( R.drawable.item_light );
                itemHolder.type.setText("照明灯");
                itemHolder.content.setVisibility(View.VISIBLE);
                break;
            case 2:
                //itemHolder.type.setBackgroundResource( R.drawable.item_add );
                itemHolder.type.setText("添加");
                break;
        }
    }

    @Override
    public int getItemCount() {
        return (null != arraylist ? arraylist.size() : 0);
    }

    public void updateDataSet(List<BleDevice> arraylist) {
        this.arraylist = arraylist;
        notifyDataSetChanged();
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mOnItemClickListener = listener;
    }

    public class ItemHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        protected TextView type;
        protected LinearLayout content;

        public ItemHolder(View view) {
            super(view);
            this.type = view.findViewById(R.id.itemBleType);
            this.content = view.findViewById(R.id.itemBleContent);
            view.setOnClickListener(this);
        }

        @Override
        public void onClick(View v) {
            if (mOnItemClickListener != null) {
                mOnItemClickListener.onItemClick(v, (int) getAdapterPosition());//注意这里使用getTag方法获取position
            }
        }

    }
}
