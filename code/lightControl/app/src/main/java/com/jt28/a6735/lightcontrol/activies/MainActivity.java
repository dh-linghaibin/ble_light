package com.jt28.a6735.lightcontrol.activies;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;

import com.jt28.a6735.lightcontrol.R;
import com.jt28.a6735.lightcontrol.adapters.CardFragmentPagerAdapter;
import com.jt28.a6735.lightcontrol.adapters.CardPagerAdapter;
import com.jt28.a6735.lightcontrol.models.CardItem;
import com.jt28.a6735.lightcontrol.timely.ShadowTransformer;
import com.jt28.a6735.lightcontrol.widgets.OnItemClickListener;

public class MainActivity extends AppCompatActivity implements CompoundButton.OnCheckedChangeListener{
    private ViewPager mViewPager;
    private CardPagerAdapter mCardAdapter;
    private ShadowTransformer mCardShadowTransformer;
    private CardFragmentPagerAdapter mFragmentCardAdapter;
    private ShadowTransformer mFragmentCardShadowTransformer;
    private Dialog mDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mViewPager = (ViewPager) findViewById(R.id.viewPager);
        mCardAdapter = new CardPagerAdapter(this);
        mCardAdapter.addCardItem(new CardItem(R.string.title_1));
        mCardAdapter.addCardItem(new CardItem(R.string.title_2 ));
        mCardAdapter.addCardItem(new CardItem(R.string.title_3 ));
        mCardAdapter.addCardItem(new CardItem(R.string.title_4 ));
        mFragmentCardAdapter = new CardFragmentPagerAdapter(getSupportFragmentManager(),
                dpToPixels(2, this));

        mCardShadowTransformer = new ShadowTransformer(mViewPager, mCardAdapter);
        mFragmentCardShadowTransformer = new ShadowTransformer(mViewPager, mFragmentCardAdapter);

        mViewPager.setAdapter(mCardAdapter);
        mViewPager.setPageTransformer(false, mCardShadowTransformer);
        mViewPager.setOffscreenPageLimit(3);
        mCardAdapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                changeItem();

            }
        });
//        mViewPager.setAdapter(mFragmentCardAdapter);
//        mViewPager.setPageTransformer(false, mFragmentCardShadowTransformer);
//        mCardShadowTransformer.enableScaling(true);
//        mFragmentCardShadowTransformer.enableScaling(true);
        mDialog = new Dialog(this, R.style.Dialog);
        View contentView = LayoutInflater.from(this).inflate(R.layout.dialog_changeitem, null);
        mDialog.setContentView(contentView);
        ViewGroup.LayoutParams layoutParams = contentView.getLayoutParams();
        layoutParams.width = 600;
        layoutParams.height = 500;
        contentView.setLayoutParams(layoutParams);
        mDialog.getWindow().setGravity(Gravity.CENTER);
        mDialog.setCanceledOnTouchOutside(true);
    }

    public void changeItem() {
        mDialog.show();
    }

    public static float dpToPixels(int dp, Context context) {
        return dp * (context.getResources().getDisplayMetrics().density);
    }

    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
        mCardShadowTransformer.enableScaling(b);
        mFragmentCardShadowTransformer.enableScaling(b);
    }
}
