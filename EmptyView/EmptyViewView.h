
// EmptyViewView.h : interface of the CEmptyViewView class
//


#pragma once
typedef int(*DrawLiveView)(int nLength, void* pView); 

class CEmptyViewView : public CView
{
protected: // create from serialization only
	CEmptyViewView();
	DECLARE_DYNCREATE(CEmptyViewView)

// Attributes
public:
	CEmptyViewDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CEmptyViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnShowLiveView(WPARAM,LPARAM);
public:
	afx_msg void OnPaint();
	virtual void OnInitialUpdate();
	afx_msg void OnNcPaint();

	static unsigned __stdcall startRTSPclient(void* pArguments);
	static int DrawView(int nLength, void* pView);

	DrawLiveView m_pfun;

private:
	IPicture* LoadJpeg();
	void ConnectRTSPServer(DrawLiveView pFun, void* pView);
	void ConnectRTPServer(DrawLiveView pFun, void* pView);
	void CreateAnimationControl();
	int MakeHead(unsigned char *p);
	void DrawPic(int nLength);

	unsigned char* DQT(unsigned char *p);
	unsigned char* SOI(unsigned char *p)
	{
		*p++ = 0xff;
		*p++ = 0xd8;            /* SOI */

		return p;
	};
	unsigned char* APP0(unsigned char *p)
	{
		*p++ = 0xff;
		*p++ = 0xe0;
		*p++ = 0x00;
		*p++ = 0x10;
		*p++ = 0x4a;
		*p++ = 0x46;
		*p++ = 0x49;
		*p++ = 0x46;
		*p++ = 0x00;
		*p++ = 0x01;
		*p++ = 0x01;
		*p++ = 0x01;
		*p++ = 0x00;
		*p++ = 0x60;
		*p++ = 0x00;
		*p++ = 0x60;
		*p++ = 0x00;
		*p++ = 0x00;

		return p;
	};
	
	unsigned char *MakeQuantHeader(unsigned char *p, unsigned char *qt, int tableNo)
	{
		*p++ = 0xff;
		*p++ = 0xdb;            /* DQT */
		*p++ = 0;               /* length msb */
		*p++ = 67;              /* length lsb */
		*p++ = tableNo;
		memcpy(p, qt, 64);
		return (p + 64);
	}
	unsigned char *MakeDRIHeader(unsigned char *p, unsigned short dri) 
	{
			*p++ = 0xff;
			*p++ = 0xdd;				/* DRI */
			*p++ = 0x00;				/* length msb */
			*p++ = 0x04;              /* length lsb */
			*p++ = 0x00;				/* dri msb */
			*p++ = 0x54;				/* dri lsb */
			return (p);
	}

	unsigned char *DRI(unsigned char* p)
	{
		int dri = 84;
		if (dri != 0)
			p = MakeDRIHeader(p, dri);

		return p;
	};
	unsigned char *SOF(unsigned char *p)
	{
		*p++ = 0xff;
		*p++ = 0xc0;				/* SOF */
		*p++ = 0x00;               /* length msb */
		*p++ = 0x11;				/* length lsb */
		*p++ = 0x08;               /* 8-bit precision */
		*p++ = 0x04;				/* height msb */
		*p++ = 0x00;				/* height lsb */
		*p++ = 0x05;				/* width msb */
		*p++ = 0x00;               /* wudth lsb */

		*p++ = 0x03;               /* number of components */
		*p++ = 0x01;               /* comp 0 */

		//if (type == 0)
		//*p++ = 0x21;			/* hsamp = 2, vsamp = 1 */
		//else
		*p++ = 0x22;			/* hsamp = 2, vsamp = 2 */

		*p++ = 0x00;               /* quant table 0 */
		*p++ = 0x02;               /* comp 1 */
		*p++ = 0x11;				/* hsamp = 1, vsamp = 1 */
		*p++ = 0x01;               /* quant table 1 */
		*p++ = 0x03;               /* comp 2 */
		*p++ = 0x11;				/* hsamp = 1, vsamp = 1 */
		*p++ = 0x01;               /* quant table 1 */

		return p;
	};
	unsigned char *DHT(unsigned char *p);
	unsigned char *SOS(unsigned char *p)
	{
		*p++ = 0xff;
		*p++ = 0xda;				/* SOS */
		*p++ = 0x00;               /* length msb */
		*p++ = 0x0c;				/* length lsb */
		*p++ = 0x03;               /* 3 components */
		*p++ = 0x01;               /* comp 0 */
		*p++ = 0x00;               /* huffman table 0 */
		*p++ = 0x02;               /* comp 1 */
		*p++ = 0x11;				/* huffman table 1 */
		*p++ = 0x03;               /* comp 2 */
		*p++ = 0x11;				/* huffman table 1 */
		*p++ = 0x00;               /* first DCT coeff */
		*p++ = 0x3f;				/* last DCT coeff */
		*p++ = 0x00;               /* sucessive approx. */	
		return p;
	};
	unsigned char *MakeHuffmanHeader(unsigned char *p, unsigned char *codelens, int ncodes,unsigned char *symbols, int nsymbols, int tableNo,int tableClass);
};

#ifndef _DEBUG  // debug version in EmptyViewView.cpp
inline CEmptyViewDoc* CEmptyViewView::GetDocument() const
   { return reinterpret_cast<CEmptyViewDoc*>(m_pDocument); }
#endif

