// DlgServerStatus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgServerStatus.h"



// CDlgServerStatus �Ի���

IMPLEMENT_DYNAMIC(CDlgServerStatus, CDialog)

CDlgServerStatus::CDlgServerStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerStatus::IDD, pParent)
{
	m_ConnectionID = 0;
	m_ServiceID = 0;
	m_CharSet = CP_ACP;
}

CDlgServerStatus::~CDlgServerStatus()
{
}

void CDlgServerStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgServerStatus, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgServerStatus ��Ϣ�������


BOOL CDlgServerStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lvList.InsertColumn(0, _T("��"), LVCFMT_LEFT, 200);
	m_lvList.InsertColumn(1, _T("ֵ"), LVCFMT_LEFT, 150);

	CRect ClientRect;
	GetClientRect(&ClientRect);

	m_lvList.MoveWindow(&ClientRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgServerStatus::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������

	if (::IsWindow(m_lvList.GetSafeHwnd()))
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);

		m_lvList.MoveWindow(&ClientRect);
	}

	
}

void CDlgServerStatus::Open(UINT ConnectionID, UINT ServiceID, LPCTSTR ServerAddress, LPCTSTR ServiceName, int CharSet)
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
			pConnection->QueryServerStatusFormat(m_ServiceID);
			pConnection->QueryAllServerStatus(m_ServiceID);
		}
	}
	SetTimer(9358093, 5000, NULL);
	ShowWindow(SW_SHOW);
	
}
void CDlgServerStatus::Close()
{
	m_ConnectionID = 0;
	m_ServiceID = 0;
	ShowWindow(SW_HIDE);
}

void CDlgServerStatus::SetStatusFormat(UINT ConnectionID, UINT ServiceID, const CSmartStruct& ServerStatusFormat)
{
	if (m_ConnectionID != ConnectionID || m_ServiceID != ServiceID)
		return;
	m_FormatInfoList.Clear();
	void * Pos = ServerStatusFormat.GetFirstMemberPosition();
	while (Pos)
	{
		WORD MemberID;
		CSmartStruct Info = ServerStatusFormat.GetNextMember(Pos, MemberID);
		if (MemberID == SC_SST_SSFIL_SERVER_STATUS_FORMAT_INFO)
		{
			SERVER_STATUS_FORMAT_INFO FormatInfo;
			FormatInfo.StatusID = Info.GetMember(SC_SST_SSFI_STATUS_ID);
			FormatInfo.FormatType = Info.GetMember(SC_SST_SSFI_FORMAT_TYPE);
			if (m_CharSet == CP_UTF8)
			{
				LPCSTR szName = Info.GetMember(SC_SST_SSFI_NAME);
				char Buffer[2001];
				UINT Len = UTF8ToAnsi(szName, strlen(szName), Buffer, 2000);
				Buffer[Len] = 0;
				FormatInfo.Name = Buffer;
			}
			else
			{
				FormatInfo.Name = (LPCSTR)Info.GetMember(SC_SST_SSFI_NAME);
			}

			if (FormatInfo.StatusID)
			{
				m_FormatInfoList.Add(FormatInfo);
			}
		}
	}
}
void CDlgServerStatus::FlushStatus(UINT ConnectionID, UINT ServiceID, const CSmartStruct& ServerStatus)
{
	if (m_ConnectionID != ConnectionID || m_ServiceID != ServiceID)
		return;

	m_lvList.DeleteAllItems();
	LPVOID Pos = ServerStatus.GetFirstMemberPosition();
	while (Pos)
	{
		WORD MemberID;
		CSmartValue Value = ServerStatus.GetNextMember(Pos, MemberID);
		CEasyString MemberIDStr(_T("δ֪����"));
		int FormatType = SSFT_DEFAULT;
		for (UINT i = 0; i < m_FormatInfoList.GetCount(); i++)
		{
			if (m_FormatInfoList[i].StatusID == MemberID)
			{
				FormatType = m_FormatInfoList[i].FormatType;
				MemberIDStr = m_FormatInfoList[i].Name;
				break;
			}
		}

		CEasyString ValueStr;
		switch (Value.GetType())
		{
		case CSmartValue::VT_CHAR:
			ValueStr.Format(_T("%d"), (char)Value);
			break;
		case CSmartValue::VT_UCHAR:
			ValueStr.Format(_T("%u"), (BYTE)Value);
			break;
		case CSmartValue::VT_SHORT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((short)Value, true);
			else
				ValueStr.Format(_T("%d"), (short)Value);
			break;
		case CSmartValue::VT_USHORT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((WORD)Value, true);
			else
				ValueStr.Format(_T("%u"), (WORD)Value);
			break;
		case CSmartValue::VT_INT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((int)Value, true);
			else
				ValueStr.Format(_T("%d"), (int)Value);
			break;
		case CSmartValue::VT_UINT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((UINT)Value, true);
			else
				ValueStr.Format(_T("%u"), (UINT)Value);
			break;
		case CSmartValue::VT_BIGINT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((INT64)Value, true);
			else if (FormatType == SSFT_VERSION)
			{
				UINT64_CONVERTER Version;
				Version.QuadPart = Value;
				ValueStr.Format(_T("%d.%d.%d.%d"),
					Version.Words[3], Version.Words[2], Version.Words[1], Version.Words[0]);
			}
			else
				ValueStr.Format(_T("%lld"), (INT64)Value);
			break;
		case CSmartValue::VT_UBIGINT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWords((UINT64)Value, true);
			else if (FormatType == SSFT_VERSION)
			{
				UINT64_CONVERTER Version;
				Version.QuadPart = Value;
				ValueStr.Format(_T("%d.%d.%d.%d"),
					Version.Words[3], Version.Words[2], Version.Words[1], Version.Words[0]);
			}
			else
				ValueStr.Format(_T("%llu"), (UINT64)Value);
			break;
		case CSmartValue::VT_FLOAT:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWordsFloat((float)Value, true);
			else if (FormatType == SSFT_PERCENT)
				ValueStr.Format(_T("%.2f%%"), (float)Value * 100);
			else
				ValueStr.Format(_T("%g"), (float)Value);
			break;
		case CSmartValue::VT_DOUBLE:
			if (FormatType == SSFT_FLOW)
				ValueStr = FormatNumberWordsFloat((float)Value, true);
			else if (FormatType == SSFT_PERCENT)
				ValueStr.Format(_T("%.2f%%"), (double)Value * 100);
			else
				ValueStr.Format(_T("%g"), (double)Value);
			break;
		case CSmartValue::VT_STRING:
			ValueStr = (LPCTSTR)Value;
			break;
		case CSmartValue::VT_USTRING:
			ValueStr = (LPCWSTR)Value;
			break;
		default:
			ValueStr = _T("δ֪��ʽ����");
		}
		int Item = m_lvList.InsertItem(m_lvList.GetItemCount(), MemberIDStr);
		m_lvList.SetItemText(Item, 1, ValueStr);
	}
}

void CDlgServerStatus::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���	
	CDialog::OnCancel();
	Close();
}


void CDlgServerStatus::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialog::OnOK();
	Close();
}


void CDlgServerStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_ConnectionID&&m_ServiceID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
		if (pConnection)
		{
			pConnection->QueryAllServerStatus(m_ServiceID);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
