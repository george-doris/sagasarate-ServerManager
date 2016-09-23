// DlgListSelector.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgListSelector.h"




// CDlgListSelector �Ի���

IMPLEMENT_DYNAMIC(CDlgListSelector, CDialog)

CDlgListSelector::CDlgListSelector(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgListSelector::IDD, pParent)
{
	m_PrevSelectAllState = 0;
}

CDlgListSelector::~CDlgListSelector()
{
}

void CDlgListSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_lvList);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_cbSelectAll);
}


BEGIN_MESSAGE_MAP(CDlgListSelector, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILES, &CDlgListSelector::OnLvnItemchangedListFiles)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, &CDlgListSelector::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


// CDlgListSelector ��Ϣ�������


BOOL CDlgListSelector::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetWindowText(m_Title);

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	m_lvList.InsertColumn(0, _T("�ļ���"), LVCFMT_LEFT, 300);

	UINT SelectedCount = 0;
	for (UINT i = 0; i < m_ItemList.GetCount(); i++)
	{
		bool IsChecked = m_ItemList[i].IsSelected;
		int Item = m_lvList.InsertItem(i, (LPCTSTR)m_ItemList[i].Item);
		if (IsChecked)
		{
			SelectedCount++;
			m_lvList.SetCheck(Item, TRUE);
		}
		else
		{
			m_lvList.SetCheck(Item, FALSE);
		}
	}
	
	//if (SelectedCount == 0)
	//{
	//	m_cbSelectAll.SetCheck(BST_UNCHECKED);
	//}
	//else if (SelectedCount == m_ItemList.GetCount())
	//{
	//	m_cbSelectAll.SetCheck(BST_CHECKED);
	//}
	//else
	//{
	//	m_cbSelectAll.SetCheck(BST_INDETERMINATE);
	//}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CDlgListSelector::UpdateCheckStatus()
{
	UINT SelectedCount = 0;
	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if (m_lvList.GetCheck(i))
		{
			SelectedCount++;
		}
	}
	if (SelectedCount == 0)
	{
		m_cbSelectAll.SetCheck(BST_UNCHECKED);
	}
	else if (SelectedCount == m_ItemList.GetCount())
	{
		m_cbSelectAll.SetCheck(BST_CHECKED);
	}
	else
	{
		m_cbSelectAll.SetCheck(BST_INDETERMINATE);
	}
	m_PrevSelectAllState = m_cbSelectAll.GetCheck();
}

void CDlgListSelector::OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UINT OldCheckStatus = pNMLV->uOldState >> 12;
	UINT NewCheckStatus = pNMLV->uNewState >> 12;

	if (OldCheckStatus != NewCheckStatus && pNMLV->iItem < m_ItemList.GetCount())
	{
		m_ItemList[pNMLV->iItem].IsSelected = NewCheckStatus == 2;
		UpdateCheckStatus();
	}
}


void CDlgListSelector::OnBnClickedCheckSelectAll()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_PrevSelectAllState == BST_CHECKED || m_PrevSelectAllState == BST_INDETERMINATE)
	{
		for (int i = 0; i < m_lvList.GetItemCount(); i++)
		{
			m_lvList.SetCheck(i, BST_UNCHECKED);
		}
		m_cbSelectAll.SetCheck(BST_UNCHECKED);
	}
	else
	{
		for (int i = 0; i < m_lvList.GetItemCount(); i++)
		{
			m_lvList.SetCheck(i, BST_CHECKED);
		}
	}
	m_PrevSelectAllState = m_cbSelectAll.GetCheck();
}
