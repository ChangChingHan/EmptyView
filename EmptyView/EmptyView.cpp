
// EmptyView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "EmptyView.h"
#include "MainFrm.h"

#include "EmptyViewDoc.h"
#include "EmptyViewView.h"
#include "afxcmn.h"

//#include <mmsystem.h>
//Library to link : Winmm.lib
//#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const UINT  MsgForIPCam2DB =RegisterWindowMessage(_T("MsgForIPCam2DB") );

// CEmptyViewApp

BEGIN_MESSAGE_MAP(CEmptyViewApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CEmptyViewApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CEmptyViewApp construction

CEmptyViewApp::CEmptyViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CEmptyViewApp object

CEmptyViewApp theApp;


// CEmptyViewApp initialization

BOOL CEmptyViewApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(1);  // Load standard INI file options (including MRU)

	if (!AfxSocketInit())
	{
		return FALSE;
	}
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEmptyViewDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CEmptyViewView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CAnimateCtrl m_Animate;
	virtual BOOL OnInitDialog();
	afx_msg void OnAcnStartAnimate();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATE, m_Animate);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_ACN_START(IDC_ANIMATE, &CAboutDlg::OnAcnStartAnimate)
END_MESSAGE_MAP()

// App command to run the dialog
void CEmptyViewApp::OnAppAbout()
{
	static CString str(_T(""));

	//InitialDB();
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	#pragma region Test Code
//FILE *pFile = NULL;
	//int nFileSize = 0;
	//char *buffer = NULL;
	//pFile = fopen( "C:\\database\\etrocenter.reg", "ab+" );
	//if (pFile)
	//{	
	//	fseek(pFile, 0, SEEK_END);
	//	nFileSize = ftell(pFile);
	//	rewind(pFile);

	//	buffer = new char[nFileSize];
	//	memset(buffer, 0x00, nFileSize);

	//	for(int n = 0; n < nFileSize; n++)
	//	{
	//		fread(buffer+n, 1, 1, pFile);
	//	}

	//	fseek(pFile, nFileSize, SEEK_SET);
	//	fwrite(buffer, 1, nFileSize, pFile);

	//	fclose(pFile);
	//}

	

// 	char str[128];
// 	sprintf(str, "regedit.exe /e C:\\database\\%s.reg \"HKEY_CURRENT_USER\\Software\\Etrovision Technology\\ETROCENTER\"", W2A(strFileName), W2A(REG_EXT_NAME));
// 	system(str);
	// 	sprintf(str, "regedit.exe /e C:\\database\\%s.reg \"HKEY_LOCAL_MACHINE\\Software\\Etrovision Technology\"", W2A(strFileName), W2A(REG_EXT_NAME));
	// 	system(str);



// 
// 	vector<pair<CString, vector<int> > >  nodeitemByGroup;
// 
// 	::SendMessage(NULL, MsgForIPCam2DB, NULL, NULL);

// 	char str[128];
// 	sprintf(str, "SQLCMD -E -S (local)\\SQLEXPRESS  -Q %s", "\"BACKUP DATABASE etrocenter TO DISK='D:\\etrocenter.bak'\"");
// 	//sprintf(str, "ipconfig");
// 	system(str);

	/*kill task*/
	/*
	long lProcessID = 0;
	lProcessID = GetCurrentProcessId();
	char str[128];
	sprintf(str, "taskkill /pid %d", lProcessID);
	system(str);

	AfxGetMainWnd()->SendMessage(WM_CLOSE, NULL, NULL);

	HANDLE handle = GetCurrentProcess();
	TerminateProcess(handle, 0);
	*/

	/*FILE *pbakFile = NULL, *pTestFile = NULL;
	unsigned char* Bakbuffer = NULL;
	long lSizeBak = 0;
	size_t result;

	pbakFile = fopen( "C:\\database\\etrocenter.bak", "ab+" );
	//pbakFile = fopen( "C:\\database\\rewrite.bak", "w" );  //°ß¼g
	pTestFile = fopen( "C:\\database\\rewrite.bak", "w" );  //°ß¼g

	if( pbakFile != NULL)
	{
		fseek (pbakFile , 0 , SEEK_END);
		lSizeBak = ftell (pbakFile);
		rewind (pbakFile);

		Bakbuffer = new unsigned char[4];
		memset (Bakbuffer,NULL,4);
		byte buffer;
		//memset (buffer,NULL,1);
		long lCursor = 0, lIndx = 0;

		if (Bakbuffer != NULL)
		{
			while( lCursor < lSizeBak )
			{
				result = fread (&buffer,1,1,pbakFile);
				fwrite( &buffer, 1, 1, pTestFile );

				lCursor++;

				int c = 0, d = 0;
				if ((c = ftell (pTestFile)) != (d = ftell (pbakFile)))
				{
					fseek (pbakFile , lCursor , SEEK_SET);
					fseek (pTestFile , lCursor , SEEK_SET);
				}
			}
		}

		fclose (pbakFile);
		fclose (pTestFile);
	}

	

// 	if( pbakFile != NULL)
// 	{
// 
// 		for (int nIdx = 0; nIdx < lSizeBak/2; nIdx++)
// 		{
// 			if(nIdx >= 6816)
// 			{
// 				fwrite( Bakbuffer+nIdx, sizeof(char), 1, pTestFile );
// 			}
// 			else
// 				fwrite( Bakbuffer+nIdx, sizeof(char), 1, pbakFile );
// 		}
// 		fwrite( &lSizeBak, sizeof(long), 1, pbakFile );
// 		if (Bakbuffer)
// 		{
// 			fwrite( Bakbuffer, sizeof(char), lSizeBak, pbakFile );
// 		}

// 		fclose( pTestFile );
// 		fclose( pbakFile );
// 		delete [] Bakbuffer;
//	}*/
#pragma endregion Test Code

	str = _T("123");

	
// 	vector<Camera_Grp> vGroup;
// 	vector<Camera_Grp_Camera> vGroupItm;
// 	vector<Camera> vCamera;
// 
// 	GetDeviceTreeGroup(vGroup);
// 	GetCameraGroupCamera();
// 	GetCamera();
// 	
// 	m_DB.Close();
}

void CEmptyViewApp::GetCamera()
{
	bool lb_ret;
	CString str;
	CDBVariant varValue;	
	CString strSql(_T(""));

	strSql.Format(_T("select * from ec_camera where cameraid = '%d'"), 47);

	CRecordset m_rs(&m_DB);
	m_rs.Open(CRecordset::dynamic,strSql);

	while (!m_rs.IsEOF()) 
	{ 
		m_rs.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("cameraname"), str);
		m_rs.GetFieldValue(_T("ipaddress"), str);
		m_rs.GetFieldValue(_T("httpport"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("mac_address"), str);
		m_rs.GetFieldValue(_T("username"), str);
		m_rs.GetFieldValue(_T("password"), str);
		m_rs.GetFieldValue(_T("model_code"), str);
		m_rs.GetFieldValue(_T("video_format"), str);
		m_rs.GetFieldValue(_T("stream_url"), str);

		m_rs.MoveNext();  
	} 

	m_rs.Close();
}

void CEmptyViewApp::GetCameraGroupCamera()
{
	bool lb_ret;
	CString s_desc, s_id, s_default;
	int ngroupid = 0;
	CDBVariant varValue;	
	CString strSql(_T(""));

	//select * from ec_camera_group_camera where cameragroupid = '%d' order by cameragroupid
	//select * from ec_camera_group_camera where cameragroupid = '%d'

	strSql.Format(_T("select * from ec_camera_group_camera where cameragroupid = '%d"), 2);

	CRecordset m_rs(&m_DB);
	m_rs.Open(CRecordset::dynaset,strSql);

	while (!m_rs.IsEOF()) 
	{ 
		m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("cameraid"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("camera_idx"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("numb_"), varValue, SQL_C_SSHORT);
		

		m_rs.MoveNext();  
	} 
	m_rs.Close();
}

void CEmptyViewApp::GetDeviceTreeGroup(vector<Camera_Grp>& vGroup)
{
	bool lb_ret;
	CString s_desc, s_id, s_default;
	int ngroupid = 0;
	CDBVariant varValue;
	DWORD dwTreeType = 0;


	CRecordset m_rs(&m_DB);
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_camera_group order by cameragroupid"));
	m_rs.MoveLast();
	m_rs.MoveFirst();


	while (!m_rs.IsEOF()) { 

		m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		ngroupid = varValue.m_iVal;
		m_rs.GetFieldValue(_T("camera_group_desc"), s_desc);
		m_rs.GetFieldValue(_T("category_code"), varValue, SQL_C_SSHORT);
		m_rs.GetFieldValue(_T("default_"), s_default);


		m_rs.MoveNext();  

	} 

	m_rs.Close();
}

bool CEmptyViewApp::CheckDatabaseOnline()
{
	BOOL bRet = FALSE;
	CString csQuery; 
	csQuery.Format(_T("SELECT state_desc FROM sys.databases WHERE name = '%s'"), _T("Etrocenter"));
	CString csTmp;
	CRecordset recordSet(&m_DB);		
	CDBVariant var;
	recordSet.Open(CRecordset::snapshot, csQuery);
	recordSet.GetFieldValue(_T("state_desc"), csTmp);
	bRet = (csTmp == "ONLINE") ? true : false;
	recordSet.Close();

	return bRet;
}

bool CEmptyViewApp::OpenDatadase()
{
	CString csDBOpenString;
	csDBOpenString.Format(_T("DSN=%s;UID=%s;PWD=%s;LANGUAGE=us_english"), _T("Etrocenter"), _T("sa"), _T(""));
	BOOL bRet = m_DB.OpenEx(csDBOpenString, CDatabase::noOdbcDialog);

	return bRet;
}

void CEmptyViewApp::InitialDB()
{
	try
	{
		BOOL bRet = OpenDatadase();
		if (bRet)
		{
			int nRetryDatabaseStartup(4);
			bool bResult = false;
			while (--nRetryDatabaseStartup > 0 || !bResult)
			{
				bResult = CheckDatabaseOnline();
				if (bResult)
					nRetryDatabaseStartup = 0;
			}
		}
	}
	catch(CDBException * p)
	{
		m_DB.Close();
		p->Delete();
	}
}
// CEmptyViewApp message handlers



BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	
// 	if (m_Animate.Open(IDR_RUN))
// 	{
// 		m_Animate.Play(0, (UINT)-1, (UINT)-1);
// 	}

// 	m_Animate.Open(_T("D:\\Lynn-Test\\EmptyView\\EmptyView\\res\\Run.avi"));
// 	m_Animate.CenterWindow();
// 	m_Animate.Play(0,-1,-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnAcnStartAnimate()
{
	// TODO: Add your control notification handler code here
}

BOOL CEmptyViewApp::InitApplication()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::InitApplication();
}

int CEmptyViewApp::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::Run();
}

BOOL CEmptyViewApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CEmptyViewApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CEmptyViewApp::ProcessMessageFilter(int code, LPMSG lpMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::ProcessMessageFilter(code, lpMsg);
}
