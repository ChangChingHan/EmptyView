
// EmptyViewDoc.h : interface of the CEmptyViewDoc class
//


#pragma once


class CEmptyViewDoc : public CDocument
{
protected: // create from serialization only
	CEmptyViewDoc();
	DECLARE_DYNCREATE(CEmptyViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CEmptyViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


