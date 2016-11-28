#pragma once
#include "Resource.h"

#include "IUserJsonData.h"
#include "Global/CGlobalData.h"

// CDlgCtrlDevLst 对话框
template<class BASE_CLASS>
class CNoFlickerWnd : public BASE_CLASS
{
public:
	CNoFlickerWnd()
	{
		m_bSkipEraseBkgnd = FALSE;
	}

	// This member function is called to set the background fill
	// color for the flicker free control.
	//
	inline void SetBackColor(
		// An RGB value.
		COLORREF crBack) {m_crBack = crBack;}

	// This member function is called to retrieve the background fill color
	// for the flicker free control. Returns an RGB value.
	//
	inline COLORREF GetBackColor() {return m_crBack;}

protected:

	// An RGB value.
	COLORREF m_crBack;
	BOOL m_bSkipEraseBkgnd;

	// This method provides a CE procedure (WindowProc) for a CWnd object.
	// It dispatches messages through the window message map. The return value
	// depends on the message.
	//
	virtual LRESULT WindowProc(
		// Specifies the Windows message to be processed.
		UINT message,
		// Provides additional information used in processing the message. The
		// parameter value depends on the message.
		WPARAM wParam,
		// Provides additional information used in processing the message. The
		// parameter value depends on the message.
		LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				CPaintDC dc(this);

				// Get the client rect, and paint to a memory device context.  This
				// will help reduce screen flicker.  Pass the memory device context
				// to the default window procedure to do default painting.

				CRect r;
				GetClientRect(&r);
				CXTPBufferDC memDC(dc, r);
				memDC.FillSolidRect(r, GetSysColor(COLOR_WINDOW));

				return BASE_CLASS::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
			}

		case WM_WINDOWPOSCHANGING:
			m_bSkipEraseBkgnd = TRUE;
			break;

		case WM_WINDOWPOSCHANGED:
			m_bSkipEraseBkgnd = FALSE;
			break;

		case WM_ERASEBKGND:
			if (m_bSkipEraseBkgnd)
			{
				return TRUE;
			}
			break;
		}

		return BASE_CLASS::WindowProc(message, wParam, lParam);
	}
};

class CDlgCtrlDevLst : public CXTPDialog
{
	DECLARE_DYNAMIC(CDlgCtrlDevLst)

public:
	CDlgCtrlDevLst(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCtrlDevLst();

// 对话框数据
	enum { IDD = IDD_DLG_CtrlDevLst };
private:
	CImageList m_LightList;
	CImageList m_DefendList;
	CImageList m_SceneList;
	CImageList m_CurtainList;
	CImageList m_EleList;
	int m_ctlType;
	string m_strHostName;
	HostInfo m_hostInfo;
	map<int, CImageList*> m_ImgListType;
	
	vector<string> m_driverKey;
	vector<string> m_defendKey;
	vector<string> m_sceneKey;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowDevLst();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNMDblclkLstDevlst(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	CListCtrl m_devLst;

public:
	void SetControlType(int type);
	void ResetListCtl();
	int  GetDevState(int devID);

	void ParseHostInfo(const HostInfo *pHost);
	void ParseLight(const Json::Value &val);
	void ParseCurtain(const Json::Value &val);
	void ParseElec(const Json::Value &val);
	void ParseDefense(const Json::Value &val);
	void ParseScene(const Json::Value &val);
	afx_msg void OnLvnItemchangedLstDevlst(NMHDR *pNMHDR, LRESULT *pResult);

	CListCtrl& getListCtrl();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void addItem(CListCtrl& listCtrl,CImageList *imgList, int item, string ItemName, int state, int id);
	void ParseAddData(const Json::Value &val, map<int, CDlgCtrlDevLst*> dlg);
};
