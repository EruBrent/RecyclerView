#pragma once
#include "DlgCtrlDevLst.h"
#include "Model/StorageDef.h"
// CDlgControllerShow dialog

#include "Global/CGlobalData.h"

class CDlgControllerShow : public CXTPDialog
{
	DECLARE_DYNAMIC(CDlgControllerShow)

public:
	CDlgControllerShow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgControllerShow();

// Dialog Data
	enum { IDD = IDD_DLG_ContrllerShow };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnLight();
	afx_msg void OnBnClickedBtnCurtain();
	afx_msg void OnBnClickedBtnElectrical();
	afx_msg void OnBnClickedBtnDefense();
	afx_msg void OnBnClickedBtnScene();
	virtual BOOL OnInitDialog();
	void ShowDevLst();
	CDlgCtrlDevLst* CreateALineDevLst(CWnd* pParent, int type);
	void AdaptCtrlDevShow();
	void ADevSetPosition(DEV_TYPE type, CDlgCtrlDevLst* pDlg, int& nStartX);

public:
	LRESULT OnWmHostSel(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void IsHide(int value);
	void ParseHostInfo(const string &strHostName);

	// sockets callback
	void CppSockConnect(int sockID);
	void CppSockDisConnect(int sockID);
	void CppSockDelete(int sockID);
	void CppSockTimeout(int sockID);
	void CppConnectTimeout(int sockID);
	void CppSockReceive(const char *buf, int len, int sockID);
	void CppSockRecvJson(const Json::Value &valRecv, int sockID, int CMD);

private:
	CDlgCtrlDevLst* m_pDlgLightDev;
	CDlgCtrlDevLst* m_pDlgCurtain;
	CDlgCtrlDevLst* m_pDlgElectrical;
	CDlgCtrlDevLst* m_pDlgDefense;
	CDlgCtrlDevLst* m_pDlgScene;
	bool m_IsConnection;
	int m_nFilter;
	HostInfo m_hostInfo;
	int m_curSockID;
	string m_strHostName;
	map<int, CDlgCtrlDevLst*> m_dlgType;
	map<int, CDlgCtrlDevLst*>& getDlgType();
public:
	afx_msg void OnStnClickedStaticHostname();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AddDriveInfo(Json::Value& val, int cmd);
};
