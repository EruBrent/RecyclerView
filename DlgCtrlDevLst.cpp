// E:\CGhummer\Alarm\Alarm\View\Dlg\DlgCtrlDevLst.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCtrlDevLst.h"
#include "Resource.h"
#include "Model/StorageDef.h"

#include "json/jsoncpp.h"
#include "CSettingConfig.h"

#include "Global/UIDef.h"
#include "Global/CGProtocol.h"
#include "Global/CGlobalData.h"
#include "Global/CGApi.h"
#include "Global/CGlobalDlg.h"
#include "Network/ISocketProc.h"

// CDlgCtrlDevLst 对话框
static int nCount = 0;
extern CAlarmRecord *g_alarmRecord;
IMPLEMENT_DYNAMIC(CDlgCtrlDevLst, CXTPDialog)

CDlgCtrlDevLst::CDlgCtrlDevLst(CWnd* pParent /*=NULL*/)
	: CXTPDialog(CDlgCtrlDevLst::IDD, pParent)
{

}

CDlgCtrlDevLst::~CDlgCtrlDevLst()
{
}

void CDlgCtrlDevLst::DoDataExchange(CDataExchange* pDX)
{
	CXTPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_DEVLST, m_devLst);
}


BEGIN_MESSAGE_MAP(CDlgCtrlDevLst, CXTPDialog)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, IDC_LST_DEVLST, &CDlgCtrlDevLst::OnNMDblclkLstDevlst)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_DEVLST, &CDlgCtrlDevLst::OnLvnItemchangedLstDevlst)
END_MESSAGE_MAP()


// CDlgCtrlDevLst 消息处理程序

BOOL CDlgCtrlDevLst::OnInitDialog()
{
	CXTPDialog::OnInitDialog();

	CRect rc;
	GetClientRect(&rc);

	m_LightList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
	m_CurtainList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
	m_EleList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
	m_DefendList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);
	m_SceneList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20);

	CWinApp* pWinApp = AfxGetApp();
	ASSERT_VALID(pWinApp);

	m_LightList.Add (pWinApp->LoadIcon (IDI_ICON_LIGHT_OPEN));
	m_LightList.Add (pWinApp->LoadIcon (IDI_ICON_LIGHT_CLOSE));
	m_CurtainList.Add (pWinApp->LoadIcon (IDI_ICON_CURTAIN_OPEN));
	m_CurtainList.Add (pWinApp->LoadIcon (IDI_ICON_CURTAIN_CLOSE));
	m_EleList.Add (pWinApp->LoadIcon (IDI_ICON_ELE_OPEN));
	m_EleList.Add (pWinApp->LoadIcon (IDI_ICON_ELE_CLOSE));
	m_DefendList.Add (pWinApp->LoadIcon (IDI_ICON_DEFEND));
	m_DefendList.Add (pWinApp->LoadIcon (IDI_ICON_DEFEND_OLD));
	m_SceneList.Add (pWinApp->LoadIcon (IDI_ICON_SCENE));
	m_SceneList.Add (pWinApp->LoadIcon (IDI_ICON_SCENE_OLD));

	//m_devLst.SetImageList (&m_LightList, LVSIL_NORMAL);
	//m_devLst.InsertItem ( m_devLst.GetItemCount(), "灯光1", 0);
	//m_devLst.InsertItem ( m_devLst.GetItemCount(), "灯光2", 1);
	m_ImgListType.insert(make_pair(TYPE_LIGHT, &m_LightList));
	m_ImgListType.insert(make_pair(TYPE_CURTAIN, &m_CurtainList));
	m_ImgListType.insert(make_pair(TYPE_ELEC, &m_EleList));
	m_ImgListType.insert(make_pair(1, &m_DefendList));
	m_ImgListType.insert(make_pair(2, &m_DefendList));
	m_ImgListType.insert(make_pair(3, &m_DefendList));
	m_ImgListType.insert(make_pair(4, &m_SceneList));


	m_driverKey.push_back("device_array");
	m_driverKey.push_back("device_id");
	m_driverKey.push_back("device_name");
	m_driverKey.push_back("device_type");
	m_driverKey.push_back("device_state");

	m_defendKey.push_back("defense_array");
	m_defendKey.push_back("defense_id");
	m_defendKey.push_back("defense_name");
	m_defendKey.push_back("defense_atype");
	m_defendKey.push_back("defense_state");

	m_sceneKey.push_back("scene_array");
	m_sceneKey.push_back("scene_id");
	m_sceneKey.push_back("scene_name");


	

	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposDefault);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCtrlDevLst::OnSize(UINT nType, int cx, int cy)
{
	CXTPDialog::OnSize(nType, cx, cy);

	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposDefault);

	if (!m_devLst.GetSafeHwnd())
	{
		return;
	}

	CRect rcDevLst;
	m_devLst.GetClientRect(&rcDevLst);
	rcDevLst.right = rcDevLst.left+rcDevLst.Width();
	m_devLst.MoveWindow(&rcDevLst);
}

BOOL CDlgCtrlDevLst::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS| WS_CLIPCHILDREN;

	return CXTPDialog::PreCreateWindow(cs);
}

void CDlgCtrlDevLst::OnSetFocus(CWnd* pOldWnd)
{
	CXTPDialog::OnSetFocus(pOldWnd);

	if (m_devLst.GetSafeHwnd())
	{
		m_devLst.SetFocus();
	}
}

void CDlgCtrlDevLst::OnNMDblclkLstDevlst(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int item = pNMItemActivate->iItem;
	if (item < 0)
		return;
	int devID = m_devLst.GetItemData(item);
	int state = this->GetDevState(devID);

	CString str;
	str.Format("item: %d devID: %d state: %d", item, devID, state);

	if (m_ctlType == DEV_LIGHT || m_ctlType == DEV_CURTAIN || m_ctlType == DEV_ELECTRICAL)
	{
		int devType;
		if (m_ctlType == DEV_LIGHT)
			devType = TYPE_LIGHT;
		else if (m_ctlType == DEV_CURTAIN)
			devType = TYPE_CURTAIN;
		else 
			devType = TYPE_ELEC;

		// send
		Json::Value valSend;
		valSend["cmd"] = 0x3000;
		valSend["cmd_act"] = 0x09;
		valSend["device_control"] = (state == DEV_ON) ? DEV_OFF : DEV_ON;
		valSend["device_id"] = devID;
		//valSend["device_code"] = "";
		valSend["device_type"] = devType;
		valSend["device_wiredevice"] = "";
		gUserJson.DebugJson(valSend);
		gSocket.SendJson(valSend, 0x3009, m_strHostName);
		string tempConten;
		string type = "操作日志";

		if(m_ctlType == DEV_LIGHT)
		{
			if(state == DEV_OFF)
			{
				tempConten = "打开了电灯";	
			}
			else
				tempConten = "关闭了电灯";
			
			g_alarmRecord->saveLog(tempConten,type);
		}
		else if(m_ctlType == DEV_CURTAIN)
		{
			if(state == DEV_OFF)
			{
				tempConten = "打开了窗帘";	
			}
			else
				tempConten = "关闭了窗帘";

			g_alarmRecord->saveLog(tempConten,type);
		}
		else
		{
			if(state == DEV_OFF)
			{
				tempConten = "打开了电器";	
			}
			else
				tempConten = "关闭了电器";

			g_alarmRecord->saveLog(tempConten,type);
		}


	}
	else if (m_ctlType == DEV_DEFENSE)
	{
		Json::Value valSend;
		valSend["cmd"] = 0x7000;
		valSend["cmd_act"] = 0x11;
		valSend["defense_id"] = devID;
		valSend["defense_state"] = (state == DEFENSE_ON) ? DEFENSE_OFF : DEFENSE_ON;
		gSocket.SendJson(valSend, 0x7011, m_strHostName);
		string tempContent;
		if(state = DEFENSE_ON)
			tempContent = "打开了防区";
		else
			tempContent = "关闭了防区";

		string type = "控制日志";
		tempContent.insert(0,g_alarmRecord->getLoginName());
		g_alarmRecord->saveLog(tempContent,type);
	}
	else if (m_ctlType == DEV_SCENE)
	{
		Json::Value valSend;
		valSend["cmd"] = 0x5000;
		valSend["cmd_act"] = 0x09;
		valSend["scene_id"] = devID;
		gSocket.SendJson(valSend, 0x5009, m_strHostName);

		string tempContent = "打开了场景";
		tempContent.insert(0,g_alarmRecord->getLoginName());
		string type = "控制日志";
		g_alarmRecord->saveLog(tempContent,type);
		
	}
}

void CDlgCtrlDevLst::SetControlType(int type)
{
	m_ctlType = type;
}

void CDlgCtrlDevLst::ResetListCtl()
{
	CWinApp* pWinApp = AfxGetApp();
	ASSERT_VALID(pWinApp);
	m_devLst.DeleteAllItems();
}

int  CDlgCtrlDevLst::GetDevState(int devID)
{
	HostInfo *pHost = gCGData.GetHostInfo(m_strHostName);
	if (!pHost)
	{
		return -1;
	}
	
	// find
	if (m_ctlType == DEV_LIGHT || m_ctlType == DEV_CURTAIN || m_ctlType == DEV_ELECTRICAL)
	{
		const Json::Value &valDev = pHost->jDev;

		valDev["device_array"].isArray();
		int nSize = valDev["device_array"].size();

		int item = 0;
		for (int i = 0; i < nSize; i++)
		{
			const Json::Value &dev = valDev["device_array"][i];
			int device_id = dev["device_id"].asInt();
			int device_type = dev["device_type"].asInt();
			string device_name = dev["device_name"].asString();
			int device_state = dev["device_state"].asInt();
			if(devID == device_id)
			{
				return device_state;
			}
		}
	}

	if (m_ctlType == DEV_DEFENSE)
	{
		const Json::Value &valDev = pHost->jDefend;
		valDev["defense_array"].isArray();
		int nSize = valDev["defense_array"].size();

		int item = 0;
		for (int i = 0; i < nSize; i++)
		{
			Json::Value dev = valDev["defense_array"][i];
			int defense_id = dev["defense_id"].asInt();
			int devType = dev["defense_atype"].asInt();
			string strName = dev["defense_name"].asString();
			int devState = dev["defense_state"].asInt();
			// 室内防区
			if(devType == 0 && defense_id == devID)
			{
				return devState;
			}
			// 室外
			if(devType == 1 && defense_id == devID)
			{
				return devState;
			}
			// 永久
			if(devType == 2 && defense_id == devID)
			{
				return DEV_OFF;
			}
		}
	}
	if (m_ctlType == DEV_SCENE)
	{
		return DEV_OFF;
	}

	return -1;
}

void CDlgCtrlDevLst::ParseHostInfo(const HostInfo *pHost)
{
	m_hostInfo = *pHost;
	m_strHostName = pHost->strHostName; //..
	//m_strHostName = m_hostInfo.strHostName; ////??..

	gUserJson.DebugJson(pHost->jDev);

	if (m_ctlType == DEV_LIGHT)
		this->ParseLight(pHost->jDev);
	if (m_ctlType == DEV_CURTAIN)
		this->ParseCurtain(pHost->jDev);
	if (m_ctlType == DEV_ELECTRICAL)
		this->ParseElec(pHost->jDev);
	if (m_ctlType == DEV_DEFENSE)
		this->ParseDefense(pHost->jDefend);
	if (m_ctlType == DEV_SCENE)
		this->ParseScene(pHost->jScene);
}

void CDlgCtrlDevLst::ParseLight(const Json::Value &val)
{
	//this->ResetListCtl();

	Json::Value valDev = val;
	valDev["device_array"].isArray();
	int nSize = valDev["device_array"].size();

	int item = 0;
	for (int i = 0; i < nSize; i++)
	{
		Json::Value dev = valDev["device_array"][i];
		int devID = dev["device_id"].asInt();
		int devType = dev["device_type"].asInt();
		string strName = dev["device_name"].asString();
		int devState = dev["device_state"].asInt();
		if(devType == TYPE_LIGHT)
		{
			//m_LightList.Add (pWinApp->LoadIcon (IDI_ICON_LIGHT_OPEN));
			m_devLst.SetImageList (&m_LightList, LVSIL_NORMAL);
			int state = (devState == DEV_ON) ? 0 : 1;
			m_devLst.InsertItem (item, strName.c_str(), state);
			m_devLst.SetItemData(item, devID);
			//m_devLst.SetItemState(item, state, 0);

			item++;
		}
	}
}

void CDlgCtrlDevLst::ParseCurtain(const Json::Value &val)
{
	//this->ResetListCtl();

	Json::Value valDev = val;
	valDev["device_array"].isArray();
	int nSize = valDev["device_array"].size();

	int item = 0;
	for (int i = 0; i < nSize; i++)
	{
		Json::Value dev = valDev["device_array"][i];
		int devID = dev["device_id"].asInt();
		int devType = dev["device_type"].asInt();
		string strName = dev["device_name"].asString();
		int devState = dev["device_state"].asInt();
		if(devType == TYPE_CURTAIN)
		{
			//m_LightList.Add (pWinApp->LoadIcon (IDI_ICON_LIGHT_OPEN));
			m_devLst.SetImageList (&m_CurtainList, LVSIL_NORMAL);
			int state = (devState == DEV_ON) ? 0 : 1;
			m_devLst.InsertItem ( item, strName.c_str(), state);
			m_devLst.SetItemData(item, devID);
			//m_devLst.SetItemState(item, state, 0);

			item++;
		}
	}
}

void CDlgCtrlDevLst::ParseElec(const Json::Value &val)
{
	//this->ResetListCtl();

	Json::Value valDev = val;
	valDev["device_array"].isArray();
	int nSize = valDev["device_array"].size();

	int item = 0;
	for (int i = 0; i < nSize; i++)
	{
		Json::Value dev = valDev["device_array"][i];
		int devID = dev["device_id"].asInt();
		int devType = dev["device_type"].asInt();
		string strName = dev["device_name"].asString();
		int devState = dev["device_state"].asInt();
		if(devType == TYPE_ELEC)
		{
			//m_LightList.Add (pWinApp->LoadIcon (IDI_ICON_LIGHT_OPEN));
			m_devLst.SetImageList (&m_EleList, LVSIL_NORMAL);
			int state = (devState == DEV_ON) ? 0 : 1;
			m_devLst.InsertItem ( item, strName.c_str(), state);
			m_devLst.SetItemData(item, devID);
			//m_devLst.SetItemState(item, state, 0);

			item++;
		}
	}
}

void CDlgCtrlDevLst::ParseDefense(const Json::Value &val)
{
	//this->ResetListCtl();

	Json::Value valDev = val;
	valDev["defense_array"].isArray();
	int nSize = valDev["defense_array"].size();
	gUserJson.DebugJson(valDev);

	int item = 0;
	for (int i = 0; i < nSize; i++)
	{
		Json::Value dev = valDev["defense_array"][i];
		int devID = dev["defense_id"].asInt();
		int devType = dev["defense_atype"].asInt();
		string strName = dev["defense_name"].asString();
		int devState = dev["defense_state"].asInt();
		int state = (devState == DEFENSE_ON) ? 0 : 1;

		// 室内防区
		if(devType == 0)
		{
			addItem(&m_DefendList, item, strName, state, devID);

			item++;
		}
		// 室外
		if(devType == 1)
		{
			addItem(&m_DefendList, item, strName, state, devID);
			item++;
		}
		// 永久
		if(devType == 2)
		{
			addItem(&m_DefendList, item, strName, state, devID);
			item++;
		}
	}
}

void CDlgCtrlDevLst::ParseScene(const Json::Value &val)
{

	Json::Value valDev = val;
	valDev["scene_array"].isArray();
	int nSize = valDev["scene_array"].size();

	int item = 0;
	for (int i = 0; i < nSize; i++)
	{
		Json::Value dev = valDev["scene_array"][i];
		int devID = dev["scene_id"].asInt();
		//int devType = dev["device_type"].asInt();
		string strName = dev["scene_name"].asString();
		//int devState = dev["device_state"].asInt();

		Json::Value valTemp = valDev["scene_exp"];
		int iIcon = 0;
		if (valTemp != ""&& valTemp != Json::nullValue)
		{
			iIcon = valTemp["scene_logo"].asInt();
		}
		m_devLst.SetImageList (&m_SceneList, LVSIL_NORMAL);
		m_devLst.InsertItem ( item, strName.c_str(), 0); // on
		m_devLst.SetItemData(item, devID);

		item++;
	}
}
void CDlgCtrlDevLst::OnLvnItemchangedLstDevlst(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}



//void CDlgCtrlDevLst::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: Add your message handler code here and/or call default
//	if (nIDEvent == 11)
//	{
//		//this->ParseHostInfo(&m_hostInfo);
//	}
//
//	CXTPDialog::OnTimer(nIDEvent);
//}

BOOL CDlgCtrlDevLst::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam )   
		return TRUE ;  
	return CXTPDialog::PreTranslateMessage(pMsg);
}


void CDlgCtrlDevLst::addItem(CListCtrl &listCtrl, CImageList *imgList, int item, string ItemName, int state, int id)
{
	listCtrl.SetImageList(imgList, LVSIL_NORMAL);
	listCtrl.InsertItem(item, ItemName.c_str(), state);
	listCtrl.SetItemData(item, id);
}

void CDlgCtrlDevLst::ParseAddData(const Json::Value &val,map<int,CDlgCtrlDevLst*> dlg)
{
	CImageList* imgList;
	bool isSence = false; //判断当前数据是不是场景
	vector<string> test;
	vector<string> &t_vect = test;

	Json::Value::Members member(val.getMemberNames());
	for (Json::Value::Members::iterator it = member.begin();
	it != member.end(); ++it)
	{
		if (*it == "device_array" || *it == "defense_array" || *it == "scene_array")
		{
			if (*it == "device_array")
			{
				t_vect = m_driverKey;
			}
			else if(*it == "defense_array")
			{
				t_vect = m_defendKey;
			}
			else
			{
				t_vect = m_sceneKey;
				isSence = true;
				imgList = &m_SceneList;
			}
		}
	}


	val[t_vect[0].c_str()].isArray();
	int nSize = val[t_vect[0].c_str()].size();
	for (int i = 0; i < nSize; i++)
	{
		CDlgCtrlDevLst* t_dlg;
		Json::Value dev = val[t_vect[0].c_str()][i];
		int devID = dev[t_vect[1].c_str()].asInt();
		string strName = dev[t_vect[2].c_str()].asString();
		if (!isSence) 
		{
			int devType = dev[t_vect[3].c_str()].asInt();
			map<int, CImageList*>::iterator itor = m_ImgListType.find(devType);
			imgList = itor->second;
			map<int, CDlgCtrlDevLst*>::iterator iter = dlg.find(devType);
			t_dlg = iter->second;
			int devState = dev[t_vect[4].c_str()].asInt();
			int state = (devState == DEV_ON) ? 0 : 1;
			
			addItem(t_dlg->getListCtrl(),imgList, imgList->GetImageCount(), strName, state, devID);
		}
		else
		{
			addItem(t_dlg->getListCtrl(),imgList, imgList->GetImageCount(), strName, 0, devID);
		}

		
	}
}

CListCtrl& CDlgCtrlDevLst::getListCtrl()
{
	return m_devLst;
}

