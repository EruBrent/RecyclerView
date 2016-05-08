package com.oyj.recyclerview;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * Created by oyj*/


public class WeatherAdapter extends  RecyclerView.Adapter<WeatherAdapter.mHolder>
{
    private String []data;
    @Override
    public mHolder onCreateViewHolder(ViewGroup parent, int viewType){
        View view = LayoutInflater.from(parent.getContext())    //取得xml里定义的view
                .inflate(R.layout.list_item,parent,false);    //加载

        return new mHolder(view);
    }



    //将数据与viewHolder绑定
    @Override
    public void onBindViewHolder(mHolder holder, int position){
        holder.txt.setText(data[position]);
    }



    public WeatherAdapter(String []data) {
        this.data =data;
    }

    @Override
    public int getItemCount(){
        return data.length;
    }

/**作用：在这里，我们通过自定义ViewHolder，来提前获得listView中每一个item中所用元素的引用
        这样可以加快listView的滑动速度。*/


    public static class  mHolder extends RecyclerView.ViewHolder
    {
        protected TextView txt;
        private mHolder(View v){
            super(v);
            txt =(TextView)v.findViewById(R.id.textView);
        }
    }
}
