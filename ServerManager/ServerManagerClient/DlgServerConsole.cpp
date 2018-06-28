// DlgServerConsole.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgServerConsole.h"



// CDlgServerConsole �Ի���

IMPLEMENT_DYNAMIC(CDlgServerConsole, CDialog)

CDlgServerConsole::CDlgServerConsole(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerConsole::IDD, pParent)
	, m_Command(_T(""))
{
	m_ConnectionID = 0;
	m_ServiceID = 0;
	m_CharSet = CP_ACP;
}

CDlgServerConsole::~CDlgServerConsole()
{
}

void CDlgServerConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edLog);
	DDX_Text(pDX, IDC_EDIT_CMD, m_Command);
}


BEGIN_MESSAGE_MAP(CDlgServerConsole, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CDlgServerConsole::OnBnClickedButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_LOG, &CDlgServerConsole::OnBnClickedButtonShowLog)
	ON_BN_CLICKED(IDC_BUTTON_HIDE_LOG, &CDlgServerConsole::OnBnClickedButtonHideLog)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_STATUS, &CDlgServerConsole::OnBnClickedButtonServerStatus)
END_MESSAGE_MAP()


// CDlgServerConsole ��Ϣ�������


BOOL CDlgServerConsole::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDlgServerConsole::Open(UINT ConnectionID, UINT ServiceID, LPCTSTR ServerAddress, LPCTSTR ServiceName, int CharSet)
{
	m_ConnectionID = ConnectionID;
	m_ServiceID = ServiceID;
	m_CharSet = CharSet;
	CEasyString Title;
	Title.Format(_T("%s(%s)"), ServerAddress, ServiceName);
	SetWindowText(Title);

	if (m_ConnectionID&&m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QueryEnableLogRecv(m_ServiceID, true);
		}
	}
	ShowWindow(SW_SHOW);
}
void CDlgServerConsole::Close()
{
	if (m_ConnectionID&&m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QueryEnableLogRecv(m_ServiceID, false);
		}
	}

	m_ConnectionID = 0;
	m_ServiceID = 0;
	ShowWindow(SW_HIDE);
}

void CDlgServerConsole::OnBnClickedButtonExec()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if ((!m_Command.IsEmpty()) && m_ConnectionID && m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QuerySendCommand(m_ServiceID, m_Command);
		}
	}
	m_Command.Empty();
	UpdateData(false);
}


void CDlgServerConsole::OnBnClickedButtonShowLog()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_ConnectionID&&m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QueryEnableLogRecv(m_ServiceID, true);
		}
	}
}


void CDlgServerConsole::OnBnClickedButtonHideLog()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_ConnectionID&&m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QueryEnableLogRecv(m_ServiceID, false);
		}
	}
}


void CDlgServerConsole::OnBnClickedButtonServerStatus()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CDlgServerConsole::OnLogMsg(UINT ConnectionID, UINT ServiceID, LPCTSTR szLogMsg)
{

	int s1, s2;
	int sm1, sm2;
	int SelLine;
	int EndLine;

	char Buffer[5001];

	if (m_CharSet == CP_UTF8)
	{
		UINT Len = UTF8ToAnsi(szLogMsg, strlen(szLogMsg), Buffer, 5000);
		Buffer[Len] = 0;
		szLogMsg = Buffer;
	}

	m_edLog.SetRedraw(false);

	m_edLog.GetSel(sm1, sm2);
	m_edLog.SetSel(0, -1);
	m_edLog.GetSel(s1, s2);
	SelLine = m_edLog.LineFromChar(sm1);
	EndLine = m_edLog.GetLineCount() - 1;
	if (s2 > MAX_CONTROL_PANEL_MSG_LEN)
	{
		m_edLog.SetSel(0, -1);
		m_edLog.Clear();
		s2 = 0;
	}
	m_edLog.SetSel(s2, s2);
	//m_edLog.ReplaceSel(CTime::GetCurrentTime().Format("%H:%M:%S :"));


	//m_edLog.SetSel(0,-1);
	//m_edLog.GetSel(s1,s2);
	//m_edLog.SetSel(s2,s2);
	m_edLog.ReplaceSel(szLogMsg);


	m_edLog.SetSel(0, -1);
	m_edLog.GetSel(s1, s2);
	m_edLog.SetSel(s2, s2);

	m_edLog.ReplaceSel("\r\n");



	m_edLog.SetRedraw(true);


	if (SelLine == EndLine)
		m_edLog.LineScroll(m_edLog.GetLineCount());
	else
		m_edLog.SetSel(sm1, sm2);
}

void CDlgServerConsole::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
	Close();
}


void CDlgServerConsole::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialog::OnOK();
	Close();
}
