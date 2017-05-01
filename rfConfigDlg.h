// rfConfigDlg.h : header file
//

#if !defined(AFX_RFCONFIGDLG_H__19C3EC86_A279_11D4_8CE2_0060674A702D__INCLUDED_)
#define AFX_RFCONFIGDLG_H__19C3EC86_A279_11D4_8CE2_0060674A702D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRfConfigDlg dialog

class CRfConfigDlg : public CDialog
{
// Construction
public:
	CRfConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRfConfigDlg)
	enum { IDD = IDD_RFCONFIG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRfConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRfConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RFCONFIGDLG_H__19C3EC86_A279_11D4_8CE2_0060674A702D__INCLUDED_)
