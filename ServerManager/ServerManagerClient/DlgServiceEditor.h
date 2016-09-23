#pragma once
#include "afxwin.h"


// CDlgServiceEditor �Ի���

class CDlgServiceEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgServiceEditor)
protected:
	CServerConnection *		m_pConnection;
	SERVICE_INFO			m_ServiceInfo;
	CComboBox				m_cbServiceType;
	bool					m_IsAddNew;
public:
	CDlgServiceEditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgServiceEditor();

// �Ի�������
	enum { IDD = IDD_DIALOG_SERVICE_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool Init(CServerConnection * pConnection, UINT ServiceID);
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
};
