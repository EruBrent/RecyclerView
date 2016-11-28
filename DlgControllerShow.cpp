// E:\work\server_alarm\Alarm\Alarm\View\Dlg\DlgControllerShow.cpp : implementation file
//

#include "stdafx.h"
#include "DlgControllerShow.h"
#include "Model/StorageDef.h"

#include "json/jsoncpp.h"
#include "CSettingConfig.h"

#include "Global/UIDef.h"
#include "Global/CGProtocol.h"
#include "Global/CGlobalData.h"
#include "Global/CGApi.h"
#include "Global/CGlobalDlg.h"
#include "Network/ISocketProc.h"

SHOWDLG(ControllerShow)

typedef struct st_DEVTYPE2DLG
{
	DEV_TYPE type;
	CDlgCtrlDevLst* pDlg;
}DEVTYPE2DLG;

// CDlgControllerShow dialog

IMPLEMENT_DYNAMIC(CDlgControllerShow, CXTPDialog)

CDlgControllerShow::CDlgControllerShow(CWnd* pParent /*=NULL*/)
	: CXTPDialog(CDlgControllerShow::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_nFilter = DEV_ALL;

	m_pDlgLightDev = NULL;
	m_pDlgCurtain = NULL;
	m_pDlgElectrical = NULL;
	m_pDlgDefense = NULL;
	m_pDlgScene = NULL;
}

CDlgControllerShow::~CDlgControllerShow()
{
	SAFE_DELETE(m_pDlgDefense);
	SAFE_DELETE(m_pDlgElectrical);
	SAFE_DELETE(m_pDlgScene);
	SAFE_DELETE(m_pDlgCurtain);
	SAFE_DELETE(m_pDlgLightDev);

	//KillTimer(111);
}

void CDlgControllerShow::DoDataExchange(CDataExchange* pDX)
{
	CXTPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgControllerShow, CXTPDialog)
	ON_BN_CLICKED(IDC_BTN_ALL, &CDlgControllerShow::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_LIGHT, &CDlgControllerShow::OnBnClickedBtnLight)
	ON_BN_CLICKED(IDC_BTN_CURTAIN, &CDlgControllerShow::OnBnClickedBtnCurtain)
	ON_BN_CLICKED(IDC_BTN_ELECTRICAL, &CDlgControllerShow::OnBnClickedBtnElectrical)
	ON_BN_CLICKED(IDC_BTN_DEFENSE, &CDlgControllerShow::OnBnClickedBtnDefense)
	ON_BN_CLICKED(IDC_BTN_SCENE, &CDlgControllerShow::OnBnClickedBtnScene)
	ON_MESSAGE(WM_HOST_SEL, &CDlgControllerShow::OnWmHostSel)
	ON_STN_CLICKED(IDC_STATIC_HOSTNAME, &CDlgControllerShow::OnStnClickedStaticHostname)
	ON_WM_SIZING()
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgControllerShow message handlers
CDlgCtrlDevLst* CDlgControllerShow::CreateALineDevLst(CWnd* pParent, int type)
{
	CDlgCtrlDevLst* pDlg = new CDlgCtrlDevLst(pParent);
	pDlg->SetControlType(type);
	pDlg->Create(CDlgCtrlDevLst::IDD, pParent);
	pDlg->ShowWindow(SW_HIDE);
	
	return pDlg;
}

void CDlgControllerShow::ShowDevLst()
{
	m_pDlgLightDev->ShowWindow(SW_HIDE);
	m_pDlgCurtain->ShowWindow(SW_HIDE);
	m_pDlgElectrical->ShowWindow(SW_HIDE);
	m_pDlgDefense->ShowWindow(SW_HIDE);
	m_pDlgScene->ShowWindow(SW_HIDE);
	switch(m_nFilter)
	{
		case DEV_DEFENSE:
			{
				m_pDlgDefense->ShowWindow(SW_SHOW);
			}
			break;
		case DEV_ELECTRICAL:
			{
				m_pDlgElectrical->ShowWindow(SW_SHOW);
			}
			break;
		case DEV_CURTAIN:
			{
				m_pDlgCurtain->ShowWindow(SW_SHOW);
			}
			break;
		case DEV_LIGHT:
			{
				m_pDlgLightDev->ShowWindow(SW_SHOW);
			}
			break;
		case DEV_SCENE:
			{	
				m_pDlgScene->ShowWindow(SW_SHOW);
			}
			break;
		case DEV_ALL:
			m_pDlgLightDev->ShowWindow(SW_SHOW);
			m_pDlgCurtain->ShowWindow(SW_SHOW);
			m_pDlgElectrical->ShowWindow(SW_SHOW);
			m_pDlgDefense->ShowWindow(SW_SHOW);
			m_pDlgScene->ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}
}

void CDlgControllerShow::ADevSetPosition(DEV_TYPE type, CDlgCtrlDevLst* pDlg, int& nStartY)
{
	CRect rcCtrl;
	this->GetClientRect(&rcCtrl);
	if (m_nFilter & type)
	{
		CRect rcDev(0,0,0,0);
		pDlg->GetClientRect(&rcDev);
		int nHeight = rcDev.Height();
		rcDev.top = nStartY;
		rcDev.bottom = rcDev.top+nHeight;
		rcDev.right = rcDev.left+rcCtrl.Width();
		pDlg->MoveWindow(rcDev);

		nStartY += CTRL_DEV_VERTICAL_GAP+nHeight;
	}
}

void CDlgControllerShow::AdaptCtrlDevShow()
{
	int nStartY = CTRL_DEVLST_STARTPOS;

	//灯光
	ADevSetPosition(DEV_LIGHT, m_pDlgLightDev, nStartY);

	//窗帘 
	ADevSetPosition(DEV_CURTAIN, m_pDlgCurtain, nStartY);

	//电器
	ADevSetPosition(DEV_ELECTRICAL, m_pDlgElectrical, nStartY);

	//防区
	ADevSetPosition(DEV_DEFENSE, m_pDlgDefense, nStartY);

	//场景
	ADevSetPosition(DEV_SCENE, m_pDlgScene, nStartY);
}

void CDlgControllerShow::OnBnClickedBtnAll()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_ALL;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

void CDlgControllerShow::OnBnClickedBtnLight()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_LIGHT;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

void CDlgControllerShow::OnBnClickedBtnCurtain()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_CURTAIN;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

void CDlgControllerShow::OnBnClickedBtnElectrical()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_ELECTRICAL;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

void CDlgControllerShow::OnBnClickedBtnDefense()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_DEFENSE;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

void CDlgControllerShow::OnBnClickedBtnScene()
{
	if (m_IsConnection)
	{
		m_nFilter = DEV_SCENE;
		ShowDevLst();
		AdaptCtrlDevShow();
	}

}

BOOL CDlgControllerShow::OnInitDialog()
{
	CXTPDialog::OnInitDialog();

	gCGDlg.m_pDlgCtlShow = this;
	gCGDlg.m_hCtlShow = GetSafeHwnd();

	// sub listctl
	m_pDlgLightDev = CreateALineDevLst(this, DEV_LIGHT);
	m_pDlgCurtain = CreateALineDevLst(this, DEV_CURTAIN);
	m_pDlgElectrical = CreateALineDevLst(this, DEV_ELECTRICAL);
	m_pDlgDefense = CreateALineDevLst(this, DEV_DEFENSE);
	m_pDlgScene = CreateALineDevLst(this, DEV_SCENE);

	// 初始化5个
	ShowDevLst();
	AdaptCtrlDevShow();

	RegisterUserMsg(WM_HOST_SEL, this->GetSafeHwnd());
	RegisterUserMsg(WM_SOCKETS_MSG, this->GetSafeHwnd());
	RegisterUserMsg(WM_FRMSIZE, this->GetSafeHwnd());

	m_dlgType.insert(make_pair(TYPE_LIGHT, m_pDlgLightDev));
	m_dlgType.insert(make_pair(TYPE_CURTAIN, m_pDlgCurtain));
	m_dlgType.insert(make_pair(TYPE_ELEC, m_pDlgElectrical));
	m_dlgType.insert(make_pair(1, m_pDlgDefense));
	m_dlgType.insert(make_pair(2, m_pDlgDefense));
	m_dlgType.insert(make_pair(3, m_pDlgDefense));
	m_dlgType.insert(make_pair(4, m_pDlgScene));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CDlgControllerShow::OnWmHostSel(WPARAM wParam, LPARAM lParam)
{
	gCGDlg.m_pDlgCtlShow = this;
	gCGDlg.m_hCtlShow = GetSafeHwnd();

	m_strHostName = *(string *)lParam;
	cout<<"CDlgControllerShow::OnWmHostSel() hostname: "<<m_strHostName<<endl;

	SetDlgItemText(IDC_STATIC_HOSTNAME, m_strHostName.c_str());
	HostInfo *pHost = gCGData.GetHostInfo(m_strHostName);   
	
	//m_curSockID = pHost->sockID;

	gSockMgr.GetMapSocket().find(pHost->sockID);
	MapSock::iterator iter = gSockMgr.GetMapSocket().find(pHost->sockID);
	if(iter != gSockMgr.GetMapSocket().end()){
		IsHide(TRUE);
		m_IsConnection = true;
	}
	else{
		IsHide(FALSE);
		m_IsConnection = false;
	}
	if (m_strHostName.length() > 0)
	{
		this->ParseHostInfo(m_strHostName);
	}
	return 0;
}



LRESULT CDlgControllerShow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SOCKETS_MSG:
		{
			cout<<"CDlgControllerShow::OnSocketMsg()"<<endl;
			CG_NET_MSG msg;
			memcpy(&msg, (void *)lParam, sizeof(CG_NET_MSG));

			CString str;
			//str.Format("state: %d sockid: %d\n", msg.type, msg.sockID);
			//gApi.DebugStr(str);

			switch (msg.type)
			{
			case NET_CONNECT:
				this->CppSockConnect(msg.sockID);
				break;
			case NET_DISCONNECT:
				this->CppSockDisConnect(msg.sockID);
				break;
			case NET_DELETE:
				this->CppSockDelete(msg.sockID);
				break;
			case NET_TIMEOUT:
				this->CppSockTimeout(msg.sockID);
				break;
			case NET_CONN_TIMEOUT:
				this->CppConnectTimeout(msg.sockID);
				break;
			case  NET_RECEIVE:
				//this->CppSockReceive(msg.buf, msg.len, msg.sockID);
				this->CppSockRecvJson(*msg.pVal, msg.sockID, msg.CMD);
				break;
			default:
				break;
			}
			return 0;
		}
	}
	return CXTPDialog::WindowProc(message, wParam, lParam);   
}

void CDlgControllerShow::ParseHostInfo(const string &strHostName)
{
	m_hostInfo.strHostName = "";
	// 
	HostInfo *pHost = gCGData.GetHostInfo(m_strHostName);
	if (pHost)
	{
		memcpy(&m_hostInfo, pHost, sizeof(HostInfo));
		if (m_pDlgLightDev && m_pDlgCurtain && m_pDlgElectrical && 
			m_pDlgElectrical && m_pDlgDefense && m_pDlgScene)
		{
			m_pDlgDefense->ParseHostInfo(pHost);
			m_pDlgElectrical->ParseHostInfo(pHost);
			m_pDlgCurtain->ParseHostInfo(pHost);
			m_pDlgLightDev->ParseHostInfo(pHost);
			m_pDlgScene->ParseHostInfo(pHost);
		}
	}

}

// sockets callback
void CDlgControllerShow::CppSockConnect(int sockID)
{
	cout<<"CDlgControllerShow::CppSockConnect() sockID = "<<sockID<<endl;
// 	m_curSockID = sockID;
// 	if (m_curSockID == sockID)
// 	{
// 		string strUserName = m_valHost[PROTO_CFG::INI::KEY_USER_NAME].asString();
// 		string strPwd = m_valHost[PROTO_CFG::INI::KEY_USER_PWD].asString();
// 		int iHostType = m_valHost[PROTO_CFG::INI::KEY_HOST_TYPE].asInt();
// 		gSocket.SendLogin(strUserName, strPwd, sockID, iHostType);
// 	}
}

void CDlgControllerShow::CppSockDisConnect(int sockID)
{
	m_curSockID = -1;
	cout<<"CDlgControllerShow::CppSockDisConnect() sockID = "<<sockID<<endl;
}

void CDlgControllerShow::CppSockDelete(int sockID)
{
	m_curSockID = -1;
	cout<<"CDlgControllerShow::CppSockDelete() sockID = "<<sockID<<endl;
}

void CDlgControllerShow::CppSockTimeout(int sockID)
{
	m_curSockID = -1;
	cout<<"CDlgControllerShow::CppSockTimeout() sockID = "<<sockID<<endl;
}

void CDlgControllerShow::CppConnectTimeout(int sockID)
{
	m_curSockID = -1;
	cout<<"CDlgControllerShow::CppConnectTimeout() sockID = "<<sockID<<endl;
}

void CDlgControllerShow::CppSockReceive(const char *buf, int len, int sockID)
{
	cout<<"CDlgControllerShow::CppSockReceive() sockID = "<<sockID<<endl;
}

void CDlgControllerShow::CppSockRecvJson(const Json::Value &valRecv, int sockID, int CMD)
{
	cout<<"CDlgControllerShow::CppSockRecvJson"<<endl;

	gUserJson.DebugJson(valRecv);
	int cmd = CMD;
	int ret = valRecv["ret"].asInt();
	
	HostInfo *pHost = gCGData.SockID2HostInfo(sockID);
	//HostInfo *pHost = gCGData.SockID2HostInfoInDevControl(sockID);  

	//HostInfo *pHost = gCGData.GetHostInfo(m_strHostName); 
	if (!pHost || ret != 1)
	{
		return;
	}
	////请求设备信息返回
	if ( cmd == 0x300b || cmd ==0x3002  /*||cmd ==0x8212 ?修改秘钥返回*/ )    
	{
		gUserJson.DebugJson(valRecv);
		int device_id = valRecv["device_id"].asInt();
		int device_state = valRecv["device_state"].asInt();
		int device_code = valRecv["device_code"].asInt();
		int device_type = valRecv["device_type"].asInt();
		Json::Value &valDev = pHost->jDev;
		valDev["device_array"].isArray();
		int nSize = valDev["device_array"].size();

		for (int i = 0; i < nSize; i++)
		{
			Json::Value &dev = valDev["device_array"][i];
			int devID = dev["device_id"].asInt();
			int devType = dev["device_type"].asInt();
			string strName = dev["device_name"].asString();
			int devState = dev["device_state"].asInt();
			if(devID == device_id && devType == device_type)
			{
				dev["device_state"] = device_state;

				m_pDlgDefense->ParseHostInfo(pHost);
				m_pDlgElectrical->ParseHostInfo(pHost);
				m_pDlgCurtain->ParseHostInfo(pHost);
				m_pDlgLightDev->ParseHostInfo(pHost);
				m_pDlgScene->ParseHostInfo(pHost);
				
				return;
			}
		}
	}
	////state Update,,NDbControl the DevsState	
	if ( cmd == 0x830b ) //33547 ||dev||defense||scene's state &Update..
	{
		int device_id = valRecv["device_id"].asInt();
		int device_state = valRecv["device_state"].asInt();
		int device_code = valRecv["device_code"].asInt();
		int device_type = valRecv["device_type"].asInt();  //if:-1-->new Dev..
		if (device_type == TYPE_LIGHT ||TYPE_CURTAIN== device_type|| TYPE_ELEC == device_type)
		{

			Json::Value &valDev = pHost->jDev;
			valDev["device_array"].isArray();
			int nSize = valDev["device_array"].size();
			for (int i = 0; i < nSize; i++)
			{
				Json::Value &dev = valDev["device_array"][i];
				int devID = dev["device_id"].asInt();
				int devType = dev["device_type"].asInt();
				string strName = dev["device_name"].asString();
				int devState = dev["device_state"].asInt();

				if(devID == device_id)
				{
					dev["device_state"] = device_state;

					switch(device_type)
					{
						case TYPE_LIGHT:
							m_pDlgLightDev->ParseLight(valDev);	
							break;
						case TYPE_CURTAIN:
							m_pDlgCurtain->ParseCurtain(valDev);
							break;
						case TYPE_ELEC:
							m_pDlgElectrical->ParseElec(valDev);
							break;
						default: break;
					}
					return;
				}
			}
		} //如果是防区
		else if (device_type==TYPE_INFRARED_SENSOR ||TYPE_WILL2INFRARED==device_type ||TYPE_FINGER_LOCK == device_type
			||TYPE_DOOR_MAG == device_type ||TYPE_SOUND_CTL == device_type ||TYPE_SMOKE_SENSOR==device_type 
			||TYPE_GAS_SENSOR == device_type ||TYPE_WIND_RAIN_SENSOR==device_type ||TYPE_TEMP_SENSOR==device_type
			||TYPE_GLASS_SENSOR==device_type ||TYPE_HELP_KEY==device_type ||TYPE_MACHINE_HAND==device_type
			||TYPE_ALARM_BELL==device_type ||TYPE_DIMMER_LIGHT==device_type
			)
		{
			Json::Value &valDefend = pHost->jDefend;
			gUserJson.DebugJson(valDefend);
			if (valDefend["defense_array"].isArray())
			{
				int iCount = valDefend["defense_array"].size();
				for (int i = 0; i < iCount; i++)
				{
					Json::Value &dev = valDefend["defense_array"][i];
					string def_Name = dev["defense_name"].asString();
					int devID = dev["defense_id"].asInt();
					int devState = dev["defense_state"].asInt();
					int defenseAType = dev["defense_atype"].asInt(); 
					int defenseDType = dev["defense_dtype"].asInt();
					if (devID == device_id)
					{
						dev["defense_state"] = device_state;

						m_pDlgDefense->ParseDefense(dev);

						return;
					}
				}
			}
		}
		else{   
			//scene's state  & all Infos Update.
			Json::Value &valScene = pHost->jScene;
			gUserJson.DebugJson(valScene);
			if (valScene["scene_array"].isArray())
			{
				int iSceneCount = valScene["scene_array"].size();
				for (int jIndexScene = 0; jIndexScene<iSceneCount; jIndexScene++)
				{
					Json::Value &valCurScene = valScene["scene_array"][jIndexScene];
					int curSceneId = valCurScene["scene_id"].asInt();
					if (device_id == curSceneId)
					{
						//.....	
						m_pDlgLightDev->ParseHostInfo(pHost);	
						m_pDlgCurtain->ParseHostInfo(pHost);
						m_pDlgElectrical->ParseHostInfo(pHost);
						m_pDlgDefense->ParseHostInfo(pHost);
						m_pDlgScene->ParseHostInfo(pHost);
						return;
					}
				}
			}
		}
	}
	//广播删除终端设备
	else if (cmd == 0x8138 || cmd==0x8308 ||cmd==0x3008) 
	{
		int delCurDevId = valRecv["device_id"].asInt();
		int curRet = valRecv["ret"].asInt(); 
		
		Json::Value &valDev = pHost->jDev;
		valDev["device_array"].isArray();
		int nSize = valDev["device_array"].size();

		for (int i = 0; i < nSize; i++)
		{
			Json::Value &curDevVal = valDev["device_array"][i];
			int curDelDevId = curDevVal["device_id"].asInt();
			int curDelDevType = curDevVal["device_type"].asInt();
			if (curDelDevId == delCurDevId )
			{
				valDev["device_array"].remove(i);
				switch(curDelDevType)
				{
					case TYPE_LIGHT:
							m_pDlgLightDev->ParseLight(valDev);	
							break;
					case TYPE_CURTAIN:
							m_pDlgCurtain->ParseCurtain(valDev);
							break;
					case TYPE_ELEC:
							m_pDlgElectrical->ParseElec(valDev);
							break;
					default: break;
				}
			}
		}
		return;
	}
	//广播添加设备,添加设备返回
	else if (cmd==0x8304 || cmd== 0x3004 )	//addDev//8304
	{
		int curAddDevId = valRecv["device_id"].asInt();
		int curAddDevType = valRecv["device_type"].asInt();
		string curAddDevName = valRecv["device_name"].asString(); 
		int curAddDevState = valRecv["device_state"].asInt();
		int curRet = valRecv["ret"].asInt(); 
		
		Json::Value &valDev = pHost->jDev;
		//int nSize = valDev["device_array"].size();
		valDev["device_array"].append(valRecv);
		//gUserJson.DebugJson(valDev);
		
		switch(curAddDevType)
		{
			case TYPE_LIGHT: 
				m_pDlgLightDev->ParseLight(valDev);		
				//m_pDlgLightDev->ParseLight(pHost->jDev);		
				/*if (curAddDevState == DEV_ON)
				{
					m_pDlgLightDev->m_LightList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
					m_pDlgLightDev->m_LightList.Add (AfxGetApp()->LoadIcon (IDI_ICON_LIGHT_OPEN));
				}else
				{
					m_pDlgLightDev->m_LightList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
					m_pDlgLightDev->m_LightList.Add (AfxGetApp()->LoadIcon (IDI_ICON_LIGHT_CLOSE));
				}*/	
				/*m_pDlgLightDev->m_LightList.Add(AfxGetApp()->LoadIcon(IDI_ICON_LIGHT_OPEN));
				
				m_pDlgLightDev->m_devLst.SetImageList(&m_pDlgLightDev->m_LightList, LVSIL_NORMAL);
				m_pDlgLightDev->m_devLst.InsertItem(0, curAddDevName.c_str(), curAddDevState);
				m_pDlgLightDev->m_devLst.SetItemData(0, curAddDevId);
				*/
				break;
			case TYPE_CURTAIN:
				m_pDlgCurtain->ParseCurtain(valDev);
				break;
			case TYPE_ELEC:
				m_pDlgElectrical->ParseElec(valDev);
				break;
			default: break;
		}
		return;
	}
	//广播修改设备,修改设备返回
	else if (cmd==0x8306 ||cmd== 0x3006)	//edit/modify Dev
	{
		int curEditDevId = valRecv["device_id"].asInt();
		int curEditDevType = valRecv["device_type"].asInt();
		string curEditDevName = valRecv["device_name"].asString();
		int curRet = valRecv["ret"].asInt(); 
		
		Json::Value &valDev = pHost->jDev;
		valDev["device_array"].isArray();
		int nSize = valDev["device_array"].size();
		for (int i = 0; i < nSize; i++)
		{
			Json::Value &curEditVal = valDev["device_array"][i];
			int curDevId = curEditVal["device_id"].asInt();
			int curDevType = curEditVal["device_type"].asInt();
			int curDevState = curEditVal["device_state"].asInt();
			if (curEditDevId == curDevId && curEditDevType == curDevType)
			{
				gUserJson.DebugJson(valDev);
				gUserJson.DebugJson(valRecv);
				curEditVal["device_state"] = curDevState;
				curEditVal["device_name"] = curEditDevName;
				//int indexItem = 0;
				switch(curEditDevType)
				{
					case TYPE_LIGHT:
						m_pDlgLightDev->ParseLight(valDev);
						break;
					case TYPE_CURTAIN:
						m_pDlgCurtain->ParseCurtain(valDev);
						break;
					case TYPE_ELEC:
						m_pDlgElectrical->ParseElec(valDev);
						break;
					default: break;
				}
			}
		}
		return;
	}

	//// defense设置防区撤布防返回,广播设置防区撤布防,请求防区信息返回
	else if (cmd == 0x7012 || cmd == 0x8712 ||cmd== 0x7002) //some||all Open or Close(.防区撤布防..)
	{
		int defense_id = valRecv["defense_id"].asInt();
		int defense_state = valRecv["defense_state"].asInt();

		Json::Value &valDev = pHost->jDefend;
		gUserJson.DebugJson(valDev);
		gUserJson.DebugJson(valRecv);

		valDev["defense_array"].isArray();
		int iCount = valDev["defense_array"].size();
		for (int i = 0; i < iCount; i++)
		{
			Json::Value &dev = valDev["defense_array"][i];
			string def_Name = dev["defense_name"].asString();
			int devID = dev["defense_id"].asInt();
			int devState = dev["defense_state"].asInt();
			int devType = dev["defense_atype"].asInt();
	
			int indexItem =0;
			if (devID == defense_id)
			{
				dev["defense_state"] = defense_state;
				m_pDlgDefense->ParseDefense(dev);
				return;
			}
		}
	}
		
	else if ( cmd== 0x7006|| cmd == 0x8706) //modify the defense 修改防区返回,广播修改防区
	{
		int defense_id = valRecv["defense_id"].asInt();
		int defense_state = valRecv["defense_state"].asInt();
		string defense_name = valRecv["defense_name"].asString();
		int defenseType = valRecv["defense_atype"].asInt(); 
		int defenseDTpe = valRecv["defense_dtype"].asInt();	

		Json::Value &valDev = pHost->jDefend;
		gUserJson.DebugJson(valDev);
		gUserJson.DebugJson(valRecv);

		valDev["defense_array"].isArray();
		int iCount = valDev["defense_array"].size();
		for (int i = 0; i < iCount; i++)
		{
			Json::Value &dev = valDev["defense_array"][i];
			string def_Name = dev["defense_name"].asString();
			int devID = dev["defense_id"].asInt();
			int devState = dev["defense_state"].asInt();
			int defenseAType = dev["defense_atype"].asInt(); 
			int defenseDType = dev["defense_dtype"].asInt();
			//int indexItem =0;
			if (devID == defense_id)
			{
				dev["defense_state"] = defense_state;
				dev["defense_name"] = defense_name;
				dev["defense_atype"] = defenseType;
				dev["defense_dtype"] = defenseDTpe;

				m_pDlgDefense->ParseDefense(dev);
				
				return;
			}
		}
	}

	//else if (cmd == 0x700a || cmd == 0x8710)   // all Open or Close 
	//{
	//	int recvCount = valRecv["defense_count"].asInt(); //数组长度..
	//	int curRet = valRecv["ret"].asInt();
	//	if(valRecv["defense_array"].isArray())  //...? difference between theUp
	//	{ 
	//		int recvSize = valRecv["defense_array"].size();
	//		for (int jDef = 0; jDef< recvCount; jDef++)
	//		{
	//			Json::Value valDefendArray = valRecv["defense_array"][jDef]; 
	//			int curDefenseId = valDefendArray["defense_id"].asInt();
	//			int curDefenseState = valDefendArray["defense_state"].asInt();
	//			
	//			Json::Value &pDefend = pHost->jDefend; //configExist..
	//			string defName = pDefend["defense_name"].asString();
	//			if(find(pDefend.begin(), pDefend.end(), curDefenseId) != pDefend.end()){
	//				int indexItem = 0;
	//				indexItem = m_pDlgLightDev->m_devLst.GetItemData(curDefenseId);
	//				m_pDlgLightDev->m_devLst.DeleteItem(indexItem);  //Delete,,first,,

	//				m_pDlgLightDev->m_devLst.SetImageList(&m_pDlgLightDev->m_DefendList, LVSIL_NORMAL);
	//				m_pDlgLightDev->m_devLst.InsertItem(indexItem, defName.c_str(), curDefenseState );
	//				m_pDlgLightDev->m_devLst.SetItemData(indexItem, curDefenseId);
	//			}
	//		}
	//	}//[self parseDefenseData:gData.jDefend];
	//	
	//}

	else if (cmd== 0x7004 || cmd== 0x8704)	//add..defense //添加防区信息返回,广播添加防区
	{
		int defense_id = valRecv["defense_id"].asInt();
		string defenseName = valRecv["defense_name"].asString();//defense_name/aType/dType..
		int defAType = valRecv["defense_atype"].asInt();
		int defDype = valRecv["defense_dtype"].asInt();//devType..
		
		Json::Value &defVal = pHost->jDefend;
		gUserJson.DebugJson(defVal);
		gUserJson.DebugJson(valRecv);
		defVal["defense_array"].append(valRecv);

		gUserJson.DebugJson(defVal);

		m_pDlgDefense->ParseDefense(defVal);
	}

	else if (cmd==0x8708 || cmd==0x7008)  //del defense..the associated devs update?!! 广播删除防区,删除防区返回
	{
		int defense_id = valRecv["defense_id"].asInt();
		int curRet = valRecv["ret"].asInt();

		Json::Value &valDefend = pHost->jDefend;
		if (valDefend["defense_array"].isArray())
		{
			int iCount = valDefend["defense_array"].size();
			for (int i = 0; i < iCount; i++)
			{
				Json::Value &dev = valDefend["defense_array"][i];
				string def_Name = dev["defense_name"].asString();
				int devID = dev["defense_id"].asInt();
				int devState = dev["defense_state"].asInt();
				int defenseAType = dev["defense_atype"].asInt(); 
				int defenseDType = dev["defense_dtype"].asInt();

				if (devID == defense_id) 
				{
					valDefend["defense_array"].remove(i);
					m_pDlgDefense->ParseDefense(valDefend);

					return;
				}
			 }
		}	
	}


	//// scense 启动场景返回,请求场景信息返回
	else if (cmd == 0x500a ||cmd== 0x5002)
	{
		gUserJson.DebugJson(valRecv);
		cout<<"<<<OK：成功启动场景！"<<endl;
	}
	else if (cmd == 0x5004 || cmd == 0x8504)//add.. 添加场景信息返回,广播添加场景
	{
		int addSceneId = valRecv["scene_id"].asInt();
		string addSceneName = valRecv["scene_name"].asString();

		Json::Value &valScene = pHost->jScene;

		gUserJson.DebugJson(valScene);

		valScene["scene_array"].append(valRecv);

		m_pDlgScene->ParseScene(valScene);
		//int scene_id = valRecv["scene_id"].asInt();
		//string scene_name = valRecv["scene_name"].asString();
		//Json::Value userRoot = valRecv["scene_usermap"];
	
		//int dev_act = 0;
		//if (valRecv["scene_device"].isArray())     //scene union device list..
		//{
		//	int iSize = valRecv["scene_device"].size();
		//	for (int  j=0; j<iSize; j++ )
		//	{
		//		Json::Value valSceneDevice = valRecv["scene_device"][j]; 
		//		int dev_id = valSceneDevice["dev_id"].asInt();
		//		int dev_type = valSceneDevice["dev_type"].asInt();//0:控制设备 1:防区..
		//		dev_act = valSceneDevice["dev_act"].asInt();
		//	}
		//}
	}

	else if (cmd == 0x5006 || cmd == 0x8506)//edit/modify scene.. 修改场景返回,广播修改场景 
	{
		m_pDlgScene->ParseHostInfo(pHost);
		int editSceneId = valRecv["scene_id"].asInt();
		int curRet = valRecv["ret"].asInt();
		string editSceneName = valRecv["scene_name"].asString();
		
		Json::Value &valScene = pHost->jScene;
		int sceneCount = valScene["scene_count"].asInt(); //房间数..
		if (valScene["scene_array"].isArray())
		{
			int sceneArrSize = valScene["scene_array"].size();
			for (int iScene = 0; iScene< sceneArrSize; iScene++)
			{
				Json::Value &curScene = valScene["scene_array"][iScene];
				int curSceneId = curScene["scene_id"].asInt();
				string curSceneName = curScene["scene_name"].asString();
				if (curSceneId == editSceneId)
				{
					curSceneName = editSceneName;

					m_pDlgScene->ParseScene(valScene);
				}
			}
		}
	}

	else if (cmd== 0x5008 || cmd == 0x8508)//del scene..
	{
		int delSceneId = valRecv["scene_id"].asInt();
		int curRet = valRecv["ret"].asInt();
	
		if (!curRet){
			return;
		}
		//m_pDlgScene->ParseHostInfo(pHost);
		Json::Value &valScene = pHost->jScene;
		int sceneCount = valScene["scene_count"].asInt(); //房间数..
		if (valScene["scene_array"].isArray())
		{
			int sceneSize = valScene["scene_array"].size();
			for (int indexScene=0; indexScene<sceneSize; indexScene++)
			{
				Json::Value curScene = valScene["scene_array"][indexScene];
				int curSceneId = curScene["scene_id"].asInt();
				if (curSceneId == delSceneId)
				{
					valScene["scene_array"].remove(indexScene);
					m_pDlgScene->ParseScene(valScene);
				}
			}	
		}
	}

}


map<int, CDlgCtrlDevLst*>& CDlgControllerShow::getDlgType()
{
	return m_dlgType;
}

void CDlgControllerShow::OnStnClickedStaticHostname()
{
	// TODO: Add your control notification handler code here
}

void CDlgControllerShow::OnSizing(UINT fwSide, LPRECT pRect)
{
	CXTPDialog::OnSizing(fwSide, pRect);

	
}


void CDlgControllerShow::IsHide(int value)
{
	m_pDlgLightDev->ShowWindow(value);
	m_pDlgCurtain->ShowWindow(value);
	m_pDlgElectrical->ShowWindow(value);
	m_pDlgDefense->ShowWindow(value);
	m_pDlgScene->ShowWindow(value);
}

BOOL CDlgControllerShow::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam )   
		return TRUE ;  
	return CXTPDialog::PreTranslateMessage(pMsg);
}


void CDlgControllerShow::AddDriveInfo(Json::Value& val, int cmd)
{

}