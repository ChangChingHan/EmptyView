
// EmptyView.h : main header file for the EmptyView application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "afxdb.h"

// CEmptyViewApp:
// See EmptyView.cpp for the implementation of this class
//
struct Camera_Grp
{

};

struct Camera_Grp_Camera
{

};

struct Camera
{

};

class CEmptyViewApp : public CWinApp
{
public:
	CEmptyViewApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL InitApplication();
	virtual int Run();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

private:
	void InitialDB();
	bool OpenDatadase();
	CDatabase m_DB;
	bool CheckDatabaseOnline();

	//device tree
	void GetDeviceTreeGroup(vector<Camera_Grp>& vGroup);
	void GetGroupItem(const Camera_Grp& cmrGroup, vector<Camera_Grp_Camera>& vGroupItm){};
	void GetDeviceChannel(const Camera_Grp_Camera& cmrGroup, vector<Camera>& vCamera){};
	void GetCameraGroupCamera();

	void GetCamera();

};

extern CEmptyViewApp theApp;
