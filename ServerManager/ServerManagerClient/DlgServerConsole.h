#pragma once
#include "afxwin.h"


// CDlgServerConsole �Ի���

class CDlgServerConsole : public CDialog
{
	DECLARE_DYNAMIC(CDlgServerConsole)
protected:
	CEdit									m_edLog;
	CString									m_Command;

	UINT									m_ConnectionID;
	UINT									m_ServiceID;
	int										m_CharSet;
public:
	CDlgServerConsole(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgServerConsole();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void Open(UINT ConnectionID, UINT ServiceID, LPCTSTR ServerAddress, LPCTSTR ServiceName, int CharSet);
	void Close();
	afx_msg void OnBnClickedButtonExec();
	afx_msg void OnBnClickedButtonShowLog();
	afx_msg void OnBnClickedButtonHideLog();
	afx_msg void OnBnClickedButtonServerStatus();


	void OnLogMsg(UINT ConnectionID, UINT ServiceID, LPCTSTR szLogMsg);
	virtual void OnCancel();
	virtual void OnOK();
};
