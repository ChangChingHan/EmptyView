
// EmptyViewView.cpp : implementation of the CEmptyViewView class
//

#include "stdafx.h"
#include "EmptyView.h"

#include "EmptyViewDoc.h"
#include "EmptyViewView.h"
#include "afxcmn.h"
#include <mmsystem.h>
//Library to link : Winmm.lib
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const UINT  MsgShowLiveView = RegisterWindowMessage(_T("MsgShowLiveView") );

int Qtbl_offset = 28;
int FrmPaylaod_offset = 156;
int Paylaod_offset = 24;
int allDataLength = 0;
int rtp_sockfd = 0;


struct threadParam
{
	CEmptyViewView* pView;
	DrawLiveView pFun;
};

unsigned char g_paylaodbuffer[80000] = {0}; 
unsigned char g_buffer[80000] = {0}; 

unsigned char g_Qtbl[128] = {0}; 
int g_nDataItr = 0;

HANDLE g_handle = NULL;

static const int jpeg_luma_quantizer[64] = {
	16, 11, 10, 16, 24, 40, 51, 61,
	12, 12, 14, 19, 26, 58, 60, 55,
	14, 13, 16, 24, 40, 57, 69, 56,
	14, 17, 22, 29, 51, 87, 80, 62,
	18, 22, 37, 56, 68, 109, 103, 77,
	24, 35, 55, 64, 81, 104, 113, 92,
	49, 64, 78, 87, 103, 121, 120, 101,
	72, 92, 95, 98, 112, 100, 103, 99
};

static const int jpeg_chroma_quantizer[64] = {
	17, 18, 24, 47, 99, 99, 99, 99,
	18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99,
	47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99
};

unsigned char lum_dc_codelens[] = {
	0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
};

unsigned char lum_dc_symbols[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};

unsigned char lum_ac_codelens[] = {
	0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d,
};

unsigned char lum_ac_symbols[] = {
	0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
	0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
	0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
	0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
	0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
	0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
	0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa,
};

unsigned char chm_dc_codelens[] = {
	0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
};

unsigned char chm_dc_symbols[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
};

unsigned char chm_ac_codelens[] = {
	0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77,
};

unsigned char chm_ac_symbols[] = {
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
	0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
	0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
	0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
	0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
	0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
	0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
	0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
	0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa,
};
// CEmptyViewView

IMPLEMENT_DYNCREATE(CEmptyViewView, CView)

BEGIN_MESSAGE_MAP(CEmptyViewView, CView)
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(MsgShowLiveView, OnShowLiveView)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CEmptyViewView construction/destruction

CEmptyViewView::CEmptyViewView()
{
	// TODO: add construction code here

}

CEmptyViewView::~CEmptyViewView()
{
}

BOOL CEmptyViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEmptyViewView drawing

void CEmptyViewView::OnDraw(CDC* pDC)
{
	CEmptyViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	TRACE(_T("Exception - bOutOfRange"));


	if (!pDoc)
		return;

	
	// TODO: add draw code for native data here
}


// CEmptyViewView diagnostics

#ifdef _DEBUG
void CEmptyViewView::AssertValid() const
{
	CView::AssertValid();
}

void CEmptyViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEmptyViewDoc* CEmptyViewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEmptyViewDoc)));
	return (CEmptyViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CEmptyViewView message handlers
IPicture* CEmptyViewView::LoadJpeg()
{
	//////////////////////////////////////////////////////////////////////////
	char datapath[50] = {0};
	FILE * pReadFile = NULL;
	IPicture* pPic;

	memset(datapath, 0x00, sizeof(datapath));
	sprintf(datapath, "C:\\Users\\Hank\\Desktop\\Data.jpg");
	pReadFile = fopen (datapath,"rb");
	if (pReadFile!=NULL)
	{
		fseek(pReadFile, 0, SEEK_END);   // non-portable
		int nDataSize = ftell (pReadFile);
		rewind (pReadFile);

		unsigned char* pBuff = new unsigned char [nDataSize];
		int nResult = fread (pBuff,sizeof(unsigned char),nDataSize,pReadFile);
		if (nResult == nDataSize)
		{
			
		}

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataSize);
		void* pData = GlobalLock(hGlobal);
		memcpy(pData, pBuff, nDataSize);
		GlobalUnlock(hGlobal);

		IStream* pStream = NULL;
		

		if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
		{
			HRESULT hr;
			if ((hr = OleLoadPicture (pStream, nDataSize, FALSE, IID_IPicture,
				(LPVOID *)&pPic)) == S_OK)

			pStream->Release();
		}

		fclose (pReadFile);
		delete [] pBuff;
	}

//////////////////////////////////////////////////////////////////////////
	//string mFile("C:\\Users\\Hank\\Desktop\\Data.jpg");
	//CString pFSize;
	//WCHAR wpath[MAX_PATH];
	//MultiByteToWideChar(CP_ACP, 0, mFile.c_str(), -1, wpath, MAX_PATH);

	//
	//OleLoadPicturePath(wpath, NULL, NULL, NULL, IID_IPicture,(LPVOID*)&pPic);

	if (pPic==NULL) return NULL;
	

	return pPic;
}

void CEmptyViewView::OnPaint()
{
	if (allDataLength>0)
	{
		DrawPic(allDataLength);
	}
}

void CEmptyViewView::ConnectRTSPServer(DrawLiveView pFun, void* pView)
{
	sockaddr_in webserver;
	int nResult = 0, sockfd = 0, addr_len = sizeof(sockaddr_in);
	char receive_message[1024] = {0};
	char chAuthEncode[30] = {0};

	int nPort = 554;
	char chIP[] = "10.1.21.190";

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	webserver.sin_family=AF_INET;
	webserver.sin_port=htons(nPort);
	webserver.sin_addr.s_addr=inet_addr(chIP);

	string send_message;
	nResult = connect(sockfd,(sockaddr*)(&webserver),sizeof(sockaddr));
	for (int nIdx = 0; nIdx < 4; nIdx++)
	{
		if(nIdx == 0)
			send_message = "OPTIONS rtsp://10.1.21.190:554/rtpvideo1.sdp RTSP/1.0\r\nCSeq: 2\r\n\r\n";
		else if (nIdx == 1)
			send_message = "DESCRIBE rtsp://10.1.21.190:554/rtpvideo1.sdp RTSP/1.0\r\nCSeq: 3\r\nAccept: application/sdp\r\n\r\n";
		else if (nIdx == 2)
			send_message = "SETUP rtsp://10.1.21.190/rtpvideo1.sdp/track1 RTSP/1.0\r\nCSeq: 4\r\nTransport: RTP/AVP;unicast;client_port=55838-55839\r\n\r\n";
		//else if (nIdx == 3)
		//	send_message = "SETUP rtsp://10.1.21.190/rtpvideo1.sdp/track2 RTSP/1.0\r\nCSeq: 5\r\nTransport: RTP/AVP;unicast;client_port=55840-55841\r\nSession: 638451ED\r\n\r\n";
		else if (nIdx == 3)
			send_message = "PLAY rtsp://10.1.21.190/rtpvideo1.sdp/ RTSP/1.0\r\nCSeq: 6\r\nSession: 638451ED\r\nRange: npt=0.000-\r\n\r\n";

		memset(receive_message, 0x00, 1024);
		nResult = send(sockfd,send_message.c_str(),send_message.length(), 0);
		nResult = recvfrom(sockfd, receive_message,sizeof(receive_message), 0 , (sockaddr*)&webserver ,&addr_len);
	}
	// zack maybe IP Camera will not send stream when socket close.
	// zack closesocket(sockfd);

	//ConnectRTPServer(pFun, pView);
	//closesocket(sockfd);
}

void CEmptyViewView::ConnectRTPServer(DrawLiveView pFun,void* pView)
{
	CEmptyViewView* p = (CEmptyViewView*)pView;
	sockaddr_in rtpserver;
	int nResult = 0;
	rtp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	char receive_message[1490] = {0};
	//char receive_message[1490] = {0};

	int addr_len = sizeof(sockaddr_in);

	rtpserver.sin_family=AF_INET;
	// zack RTSP SETUP
	// send_message = "SETUP rtsp://10.1.21.34/rtpvideo1.sdp/track1 RTSP/1.0\r\nCSeq: 4\r\nTransport: RTP/AVP;unicast;client_port=55838-55839\r\n\r\n";
	// parse Response server port ??? to htons();
	// zack rtpserver.sin_port=htons(55838);
	rtpserver.sin_port=htons(55838);
	//rtpserver.sin_addr.s_addr=inet_addr("10.1.21.34");
	rtpserver.sin_addr.s_addr=INADDR_ANY;

	nResult = bind(rtp_sockfd,(sockaddr*)&rtpserver,sizeof(rtpserver));
	//nResult = connect(sockfd,(sockaddr*)(&rtpserver),sizeof(sockaddr));

	int nLength = 0, nPreLength = 0;
	bool Qtbl = false;

	memset(g_buffer, 0x00, 80000);
	memset(g_paylaodbuffer, 0x00, 80000);
}

void CEmptyViewView::DrawPic(int nLength)
{
	FILE * pFile;
	pFile = fopen ("C:\\Users\\Hank\\Desktop\\Data.jpg","wb+");
	if (pFile!=NULL)
	{
		fwrite (g_buffer , sizeof(unsigned char), nLength, pFile);
		fclose (pFile);
	}

	CPaintDC dc(this); 

	//IPicture* pPic = LoadJpeg();
	IPicture* pPic;
	int nDataSize = nLength;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, g_buffer, nDataSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
	HRESULT hr;

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		if ((hr = OleLoadPicture (pStream, nDataSize, FALSE, IID_IPicture,
			(LPVOID *)&pPic)) == S_OK)

			pStream->Release();
	}

	long mWid,mHei;
	if (pPic)
	{
		pPic->get_Height(&mHei);
		pPic->get_Width(&mWid);

		pPic->Render (dc.m_hDC, 0, 0, 1920, 1080, 0,
			mHei, mWid, -mHei, NULL);
	}

	//Sleep(1000);
}

unsigned char *CEmptyViewView::MakeHuffmanHeader(unsigned char *p, unsigned char *codelens, int ncodes,
				  unsigned char *symbols, int nsymbols, int tableNo,
				  int tableClass)
{
	*p++ = 0xff;
	*p++ = 0xc4;            /* DHT */
	*p++ = 0;               /* length msb */
	*p++ = 3 + ncodes + nsymbols; /* length lsb */
	*p++ = (tableClass << 4) | tableNo;
	memcpy(p, codelens, ncodes);
	p += ncodes;
	memcpy(p, symbols, nsymbols);
	p += nsymbols;
	return (p);
}

unsigned char *CEmptyViewView::DHT(unsigned char *p)
{
	p = MakeHuffmanHeader(p, lum_dc_codelens,
		sizeof(lum_dc_codelens),
		lum_dc_symbols,
		sizeof(lum_dc_symbols), 0, 0);
	p = MakeHuffmanHeader(p, lum_ac_codelens,
		sizeof(lum_ac_codelens),
		lum_ac_symbols,
		sizeof(lum_ac_symbols), 0, 1);
	p = MakeHuffmanHeader(p, chm_dc_codelens,
		sizeof(chm_dc_codelens),
		chm_dc_symbols,
		sizeof(chm_dc_symbols), 1, 0);
	p = MakeHuffmanHeader(p, chm_ac_codelens,
		sizeof(chm_ac_codelens),
		chm_ac_symbols,
		sizeof(chm_ac_symbols), 1, 1);

	return p;
}

unsigned char * CEmptyViewView::DQT(unsigned char *p)
{
	unsigned char lqt[64] = {0};
	unsigned char cqt[64] = {0};
	memcpy(lqt, g_Qtbl, 64);
	memcpy(cqt, g_Qtbl+64, 64);

	p = MakeQuantHeader(p, lqt, 0);
	p = MakeQuantHeader(p, cqt, 1);
	return p;
}
int CEmptyViewView::MakeHead(unsigned char *p)
{
	unsigned char *start = p;
	p = SOI(p);
	p = APP0(p);
	p = SOF(p);
	p = DHT(p);
	p = DRI(p);
	p = DQT(p);
	p = SOS(p);
	return (p - start);
}
void CEmptyViewView::CreateAnimationControl()
{
	// Create the animation control. 
	//CAnimateCtrl cAnimCtrl;
	// 	if (cAnimCtrl.Create(WS_CHILD|WS_VISIBLE|ACS_CENTER, 
	// 		CRect(10,10,100,100), this, 1))
	// 	{
	// 		// Open the AVI file. 
	// 		if (cAnimCtrl.Open(_T("D:\\Lynn-Test\\EmptyView\\EmptyView\\res\\filecopy.avi")))
	// 		{
	// 			cAnimCtrl.Play(0, (UINT)-1, (UINT)-1);
	// 		}
	// 	}
}


unsigned __stdcall CEmptyViewView::startRTSPclient(void* pArguments)
{
	threadParam *pParam = (threadParam*)pArguments;
	CEmptyViewView* pView = pParam->pView;
	DrawLiveView pFun = pParam->pFun;

	pParam->pView->ConnectRTSPServer(pParam->pFun, pParam->pView);
	pParam->pView->ConnectRTPServer(pParam->pFun, pParam->pView);

	int nLength = 0, nPreLength = 0;
	bool Qtbl = false;
	char receive_message[1490] = {0};
	sockaddr_in rtpserver;
	int addr_len = sizeof(sockaddr_in);

	while(1)
	{
		nPreLength = nLength;
		nLength = recvfrom(rtp_sockfd, receive_message,sizeof(receive_message), 0 , (sockaddr*)&rtpserver ,&addr_len);

		int nQLength = 0;
		if (g_nDataItr == 0)
		{
			memcpy(&nQLength, receive_message + (Qtbl_offset - 2), 2);
			if (nQLength == 32768)
			{
				Qtbl = true;
				memcpy(g_Qtbl, receive_message + Qtbl_offset, 128);
				memcpy(g_paylaodbuffer, receive_message + FrmPaylaod_offset, nLength-FrmPaylaod_offset);
				g_nDataItr += (nLength-FrmPaylaod_offset);
			}
			else
				continue;
		}
		else
		{
			if (Qtbl)
			{
				memcpy((g_paylaodbuffer+g_nDataItr), (receive_message+Paylaod_offset), (nLength-Paylaod_offset));
				g_nDataItr += (nLength-Paylaod_offset);
			}
		}

		if (nLength < nPreLength)
		{
			Qtbl = false;

			int nPayloadLength = g_nDataItr;
			g_nDataItr = 0;

			g_nDataItr = pView->MakeHead(g_buffer);
			memcpy(g_buffer+g_nDataItr, g_paylaodbuffer, nPayloadLength);
			allDataLength = g_nDataItr+nPayloadLength;

			//pFun(allDataLength, pView);
			//DrawPic(allDataLength);

			pView->SendMessage(MsgShowLiveView, allDataLength, 0);
			g_nDataItr = 0;
			//memset(g_buffer, 0x00, 80000);
			//memset(g_paylaodbuffer, 0x00, 80000);
			//break;
			Sleep(1500);
		}
	}
	closesocket(rtp_sockfd);

	return 0;
}

int CEmptyViewView::DrawView(int nLength, void* pView)
{
	CEmptyViewView* pEmptyView = (CEmptyViewView*)pView;

	FILE * pFile;
	pFile = fopen ("C:\\Users\\Hank\\Desktop\\Data.jpg","wb+");
	if (pFile!=NULL)
	{
		fwrite (g_buffer , sizeof(unsigned char), nLength, pFile);
		fclose (pFile);
	}

	CPaintDC dc(pEmptyView); 

	//IPicture* pPic = LoadJpeg();
	IPicture* pPic;
	int nDataSize = nLength;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataSize);
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, g_buffer, nDataSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
	HRESULT hr;

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		if ((hr = OleLoadPicture (pStream, nDataSize, FALSE, IID_IPicture,
			(LPVOID *)&pPic)) == S_OK)

			pStream->Release();
	}

	long mWid,mHei;
	if (pPic)
	{
		pPic->get_Height(&mHei);
		pPic->get_Width(&mWid);

		pPic->Render (dc.m_hDC, 0, 0, 1920, 1080, 0,
			mHei, mWid, -mHei, NULL);
	}
	return 0;
}

void CEmptyViewView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

 	threadParam s_threadparam;
 	s_threadparam.pFun = DrawView;
 	s_threadparam.pView = this;
 	g_handle = (HANDLE)::_beginthreadex(NULL, 0, startRTSPclient, &s_threadparam, 0, NULL);


	//ConnectRTSPServer(m_pfun, this);

	//CreateAnimationControl();

	// TODO: Add your specialized code here and/or call the base class
}

LRESULT CEmptyViewView::OnShowLiveView(WPARAM wPa, LPARAM lPa)
{
	DrawPic((int)wPa);
	return 0;
}
void CEmptyViewView::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CView::OnNcPaint() for painting messages
}
