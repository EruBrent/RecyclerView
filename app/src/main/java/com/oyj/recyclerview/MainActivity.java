package com.oyj.recyclerview;

import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.DefaultItemAnimator;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;

public class MainActivity extends AppCompatActivity
{
    RecyclerView recyclerView;
    private String []data = {"Monday","Tuesday","Wednesday","Thurday","Friday",
            "Saturday","Sunday" ,"one","two","three","january","february","March"
            ,"April","May","June","July","August","Set","Oct","analyze","Refactor"};

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar)findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
         /*ToolBarr与DrawerLayout绑定*/
        DrawerLayout drawer = (DrawerLayout)findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this,drawer,toolbar,R.string.navigation_drawer_open,R.string.navigation_drawer_close){
            @Override
            public void onDrawerOpened(View drawerView) {super.onDrawerOpened(drawerView);}
            @Override
            public void onDrawerClosed(View drawerView) {
                super.onDrawerClosed(drawerView);
            }
        };
        toggle.syncState();
        WeatherAdapter adapter = new WeatherAdapter(data);
        recyclerView = (RecyclerView)findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));//这里表明用线性显示
        recyclerView.setAdapter(adapter);
        recyclerView.setItemAnimator(new DefaultItemAnimator());
    }
}
