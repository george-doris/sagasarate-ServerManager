// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"



// CDlgLogin �Ի���

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_AccountName(_T(""))
	, m_Password(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_AccountName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
END_MESSAGE_MAP()


// CDlgLogin ��Ϣ�������
