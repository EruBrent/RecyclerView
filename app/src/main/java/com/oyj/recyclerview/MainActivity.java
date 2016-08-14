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
void CPIctTestDlg::OnBnClickedButton1()
{

	CDbSQLite sqlite; 
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	string sql =  "SELECT ST,STL FROM COMPANY WHERE STL >=2" ;
	string st ="INSERT INTO COMPANY(ST,STL)"\
			   "VALUES('HELLO','1');"
			   "INSERT INTO COMPANY(ST,STL)"\
			     "VALUES('DOYOU','3');";
 
	  rc = sqlite3_open("theTestSqlite.db", &db);
	 
	  rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	   if( rc != SQLITE_OK )
		   AfxMessageBox(_T("不能创建表usersInfo")); 

	/*if (!fTest) 
	{ 
		AfxMessageBox(_T("打不开theTestSqlite.db")); 
		return; 
	} */
	//执行创建表语句 
	//fTest = sqlite.DirectStatement(_T("CREATE TABLE usersIN(name varchar(30), password varchar(20),st, Text)")); 
	//fTest = sqlite.DirectStatement(st);
	//if (!fTest) 
	//	AfxMessageBox(_T("不能创建表usersInfo")); 
	////新增数据 
 sqlite3_close(db);
	
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	
	TRACE("%s",NotUsed);
	for(int i=0;i<argc; i++){
		//TRACE("%s = %s\n", azColName[i],argv[i] ? argv[i] : "NULL");
		TRACE("%d,%s=%s\n",argc,azColName[i],argv[i]);
	}
	printf("\n");
	return 0;
}
