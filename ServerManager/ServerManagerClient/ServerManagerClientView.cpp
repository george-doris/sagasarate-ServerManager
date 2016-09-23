﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClientView.cpp                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClientView.cpp : CServerManagerClientView 类的实现
//

#include "stdafx.h"
#include "ServerManagerClientView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientView

IMPLEMENT_DYNCREATE(CServerManagerClientView, CFormView)

BEGIN_MESSAGE_MAP(CServerManagerClientView, CFormView)
	ON_WM_SIZE()
	
	ON_NOTIFY(NM_RCLICK, IDC_SERVICE_LIST, &CServerManagerClientView::OnNMRClickServiceList)
	ON_COMMAND(ID_STARTUP_SERVICE, &CServerManagerClientView::OnStartupService)
	ON_COMMAND(ID_SHUTDOWN_SERVCE, &CServerManagerClientView::OnShutdownServce)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SERVICE_LIST, &CServerManagerClientView::OnLvnColumnclickServiceList)
	ON_COMMAND(ID_POPUP_BROWSE_WORK_DIR, &CServerManagerClientView::OnPopupBrowseWorkDir)
	ON_COMMAND(ID_SHOW_HIDE_SERVICE, &CServerManagerClientView::OnShowHideService)
	ON_UPDATE_COMMAND_UI(ID_SHOW_HIDE_SERVICE, &CServerManagerClientView::OnUpdateShowHideService)
	ON_COMMAND(ID_SHUT_DOWN_SERVICE_FORCE, &CServerManagerClientView::OnShutDownServiceForce)
	ON_NOTIFY(NM_DBLCLK, IDC_SERVICE_LIST, &CServerManagerClientView::OnNMDblclkServiceList)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TYPE_TREE, &CServerManagerClientView::OnTvnSelchangedTypeTree)
	ON_COMMAND(ID_POPUP_ADD_SERVICE, &CServerManagerClientView::OnPopupAddService)
	ON_COMMAND(ID_POPUP_EDIT_SERVICE, &CServerManagerClientView::OnPopupEditService)
	ON_COMMAND(ID_POPUP_DEL_SERVICE, &CServerManagerClientView::OnPopupDelService)
END_MESSAGE_MAP()

// CServerManagerClientView 构造/析构

CServerManagerClientView::CServerManagerClientView()
	: CFormView(CServerManagerClientView::IDD)
{
	// TODO: 在此处添加构造代码
	m_ShowHideService=false;
	m_hAllServer = NULL;
	m_SelectedConnectionID = 0;
}

CServerManagerClientView::~CServerManagerClientView()
{
}

void CServerManagerClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE_TREE, m_tvServers);
	DDX_Control(pDX, IDC_SERVICE_LIST, m_lvServiceInfos);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edLog);
	DDX_Control(pDX, IDC_TASK_LIST, m_lvTask);
}

BOOL CServerManagerClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CServerManagerClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_DialogItemSorter.SetParent(this);
	m_DialogItemSorter.SetSortType(0, CDialogItemSorter::SORT_HORIZONTAL | CDialogItemSorter::PARAM_ALIGN_SPACE | CDialogItemSorter::ENABLE_RESIZE_BAR, 5, 0);

	m_DialogItemSorter.AddDialogItem(0, 1,
		CDialogItemSorter::SORT_VERTICAL |
		CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT | CDialogItemSorter::ENABLE_RESIZE_BAR,
		0, CRect(0, 0, 500, 0), 5, 0);
	m_DialogItemSorter.AddDialogItem(0,2,
		CDialogItemSorter::SORT_VERTICAL|
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH | CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT | CDialogItemSorter::ENABLE_RESIZE_BAR,
		0,CRect(0,0,0,0),5,0);

	m_DialogItemSorter.AddDialogItem(1, 11,
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH | CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		IDC_TYPE_TREE, CRect(0, 0, 0, 0), 0, 0);

	m_DialogItemSorter.AddDialogItem(1, 12,
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH,
		IDC_TASK_LIST, CRect(0, 0, 0, 600), 0, 0);
	

	m_DialogItemSorter.AddDialogItem(2,21,		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH|CDialogItemSorter::RESIZE_FIT_PARENT_HEIGHT,
		IDC_SERVICE_LIST,CRect(0,0,0,0),0,0);

	m_DialogItemSorter.AddDialogItem(2,22,		
		CDialogItemSorter::RESIZE_FIT_PARENT_WIDTH,
		IDC_EDIT_LOG,CRect(0,0,0,600),0,0);



	m_hAllServer = m_tvServers.InsertItem(_T("所有"), NULL, NULL);
	m_tvServers.Expand(m_hAllServer, TVE_EXPAND);

	m_lvServiceInfos.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lvServiceInfos.InsertColumn(0,_T("服务器"),LVCFMT_LEFT,120);
	m_lvServiceInfos.InsertColumn(1,_T("服务名称"),LVCFMT_LEFT,200);
	m_lvServiceInfos.InsertColumn(2,_T("状态"),LVCFMT_CENTER,50);
	m_lvServiceInfos.InsertColumn(3,_T("CPU占用率"),LVCFMT_RIGHT,100);
	m_lvServiceInfos.InsertColumn(4,_T("内存占用"),LVCFMT_RIGHT,90);
	m_lvServiceInfos.InsertColumn(5,_T("虚拟内存占用"),LVCFMT_RIGHT,100);
	m_lvServiceInfos.InsertColumn(6,_T("外网流量(发送/接收)"),LVCFMT_RIGHT,130);
	m_lvServiceInfos.InsertColumn(7,_T("内网流量(发送/接收)"),LVCFMT_RIGHT,130);
	m_lvServiceInfos.InsertColumn(8,_T("可执行文件日期"),LVCFMT_CENTER,130);
	m_lvServiceInfos.InsertColumn(9,_T("可执行文件路径"),LVCFMT_LEFT,300);
	

	m_lvTask.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lvTask.InsertColumn(0, _T("类型"), LVCFMT_LEFT, 60);
	m_lvTask.InsertColumn(1, _T("进度"), LVCFMT_LEFT, 60);
	m_lvTask.InsertColumn(2, _T("源文件"), LVCFMT_LEFT, 300);
	m_lvTask.InsertColumn(3, _T("目标文件"), LVCFMT_LEFT, 300);


	m_DlgWorkDirBowser.Create(m_DlgWorkDirBowser.IDD, AfxGetMainWnd());
	m_DlgServiceEditor.Create(m_DlgServiceEditor.IDD, AfxGetMainWnd());

}


// CServerManagerClientView 诊断

#ifdef _DEBUG
void CServerManagerClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CServerManagerClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CServerManagerClientDoc* CServerManagerClientView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerManagerClientDoc)));
	return (CServerManagerClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CServerManagerClientView 消息处理程序

void CServerManagerClientView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_DialogItemSorter.DoSort();
}

void CServerManagerClientView::RefreshConnection()
{
	m_tvServers.DeleteAllItems();
	m_hAllServer = m_tvServers.InsertItem(_T("所有"), NULL, NULL);
	

	void * Pos = CServerManagerClientApp::GetInstance()->GetFirstServerConnectionPos();
	while (Pos)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetNextServerConnection(Pos);
		CEasyString Temp;
		Temp.Format(_T("%s(%s)"), (LPCTSTR)pConnection->GetServerAddress(), pConnection->IsConnected() ? _T("已连接") : _T("未连接"));
		HTREEITEM hItem = m_tvServers.InsertItem(Temp, m_hAllServer);
		m_tvServers.SetItemData(hItem, pConnection->GetID());
	}

	m_tvServers.Expand(m_hAllServer, TVE_EXPAND);
}

void CServerManagerClientView::PrintLogVL(LPCTSTR szFormat,va_list vl)
{
	TCHAR LogBuffer[1000];


	CEasyTime CurTime;
	CurTime.FetchLocalTime();


	_stprintf_s(LogBuffer,1000,_T("[%02d-%02d][%02d:%02d:%02d]:"),
		CurTime.Month(),CurTime.Day(),
		CurTime.Hour(),CurTime.Minute(),CurTime.Second());


	_vstprintf_s(LogBuffer+17,1000-17,szFormat, vl );
	

	int s1,s2;
	int sm1,sm2;
	int SelLine;
	int EndLine;

	m_edLog.SetRedraw(false);

	m_edLog.GetSel(sm1,sm2);
	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	SelLine=m_edLog.LineFromChar(sm1);
	EndLine=m_edLog.GetLineCount()-1;
	if(s2>MAX_LOG_MSG_LEN)
	{
		m_edLog.SetSel(0,-1);
		m_edLog.Clear();
		s2=0;
	}
	m_edLog.SetSel(s2,s2);	
	m_edLog.ReplaceSel(LogBuffer);


	m_edLog.SetSel(0,-1);
	m_edLog.GetSel(s1,s2);
	m_edLog.SetSel(s2,s2);	

	m_edLog.ReplaceSel(_T("\r\n"));



	m_edLog.SetRedraw(true);	


	if(SelLine==EndLine)
		m_edLog.LineScroll(m_edLog.GetLineCount());
	else
		m_edLog.SetSel(sm1,sm2);
}

void CServerManagerClientView::PrintLog(LPCTSTR szFormat,...)
{
	va_list vl;

	va_start(vl,szFormat);
	PrintLogVL(szFormat,vl);
	va_end(vl);
}

void CServerManagerClientView::SetServiceInfo(CServerConnection * pConnection, const SERVICE_INFO& ServiceInfo)
{
	if ((!m_ShowHideService) && ServiceInfo.ServiceID == 0)
		return;
	if (m_SelectedConnectionID != 0 && m_SelectedConnectionID != pConnection->GetID())
		return;
	int Item=-1;
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT Param = m_lvServiceInfos.GetItemData(i);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		if (pConnection->GetID() == ConID && ServiceInfo.ServiceID == SrvID)
		{
			Item=i;
		}
	}
	if(Item<0)
	{
		Item = m_lvServiceInfos.InsertItem(m_lvServiceInfos.GetItemCount(), pConnection->GetServerAddress());
		UINT Param = ((pConnection->GetID() & 0xFFFF) << 16) | (ServiceInfo.ServiceID & 0xFFFF);
		m_lvServiceInfos.SetItemData(Item, Param);
	}
	if(Item>=0)
	{
		CEasyString Temp;
		m_lvServiceInfos.SetItemText(Item, 0, pConnection->GetServerAddress());
		Temp.Format(_T("[%u]%s"), ServiceInfo.ServiceID, (LPCTSTR)ServiceInfo.Name);
		m_lvServiceInfos.SetItemText(Item,1,Temp);
		switch (ServiceInfo.Status)
		{
		case SERVICE_STATUS_NONE:
			Temp = _T("无");
			break;
		case SERVICE_STATUS_STOP:
			Temp = _T("已停止");
			break;
		case SERVICE_STATUS_RUNNING:			
			Temp = _T("运行中");
			break;
		case SERVICE_STATUS_SHUTDOWNNING:
			Temp = _T("关闭中");
			break;
		case SERVICE_STATUS_STARTUPPING:
			Temp = _T("启动中");
			break;
		default:
			Temp = _T("未知");
			break;
		}		
		m_lvServiceInfos.SetItemText(Item,2,Temp);
		Temp.Format(_T("%.5g%%"), ServiceInfo.CPUUsed * 100);
		m_lvServiceInfos.SetItemText(Item,3,Temp);
		Temp = FormatNumberWords(ServiceInfo.MemoryUsed);
		m_lvServiceInfos.SetItemText(Item,4,Temp);
		Temp = FormatNumberWords(ServiceInfo.VirtualMemoryUsed);
		m_lvServiceInfos.SetItemText(Item,5,Temp);		
		if (ServiceInfo.ImageFileTime == -1)
		{
			Temp=_T("无");
		}
		else
		{
			CEasyTime Time(ServiceInfo.ImageFileTime);
			Time.Format(Temp,_T("%Y-%m-%d %H:%M:%S"));
		}		
		m_lvServiceInfos.SetItemText(Item,8,Temp);
		m_lvServiceInfos.SetItemText(Item, 9, ServiceInfo.ImageFilePath);

	}
}

void CServerManagerClientView::SetInternetAdapterInfo(UINT ConnectionID, float SendFlux, float RecvFlux)
{
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT Param = m_lvServiceInfos.GetItemData(i);
		UINT ConID = Param >> 16;
		if (ConnectionID == ConID)
		{
			CEasyString Temp;
			Temp.Format(_T("%s/%s"),
				(LPCTSTR)FormatNumberWordsFloat(SendFlux,true),
				(LPCTSTR)FormatNumberWordsFloat(RecvFlux,true));
			m_lvServiceInfos.SetItemText(i,6,Temp);
		}
	}
}

void CServerManagerClientView::SetIntranetAdapterInfo(UINT ConnectionID, float SendFlux, float RecvFlux)
{
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{
		UINT Param = m_lvServiceInfos.GetItemData(i);
		UINT ConID = Param >> 16;
		if (ConnectionID == ConID)
		{
			CEasyString Temp;
			Temp.Format(_T("%s/%s"),
				(LPCTSTR)FormatNumberWordsFloat(SendFlux,true),
				(LPCTSTR)FormatNumberWordsFloat(RecvFlux,true));
			m_lvServiceInfos.SetItemText(i,7,Temp);
		}
	}
}

void CServerManagerClientView::ConnectNotify(CServerConnection * pConnection)
{
	HTREEITEM hItem = m_tvServers.GetChildItem(m_hAllServer);
	while (hItem)
	{
		UINT ConnectionID = (UINT)m_tvServers.GetItemData(hItem);
		if (ConnectionID == pConnection->GetID())
		{
			CEasyString Temp;
			Temp.Format(_T("%s(%s)"), (LPCTSTR)pConnection->GetServerAddress(), pConnection->IsConnected() ? _T("已连接") : _T("未连接"));
			m_tvServers.SetItemText(hItem, Temp);
			break;
		}
		hItem = m_tvServers.GetNextSiblingItem(hItem);
	}
}

void CServerManagerClientView::DisconnectNotify(CServerConnection * pConnection)
{
	HTREEITEM hItem = m_tvServers.GetChildItem(m_hAllServer);
	while (hItem)
	{
		UINT ConnectionID = (UINT)m_tvServers.GetItemData(hItem);
		if (ConnectionID == pConnection->GetID())
		{
			CEasyString Temp;
			Temp.Format(_T("%s(%s)"), (LPCTSTR)pConnection->GetServerAddress(), pConnection->IsConnected() ? _T("已连接") : _T("未连接"));
			m_tvServers.SetItemText(hItem, Temp);
			break;
		}
		hItem = m_tvServers.GetNextSiblingItem(hItem);
	}
	for(int i=0;i<m_lvServiceInfos.GetItemCount();i++)
	{		
		UINT Param = m_lvServiceInfos.GetItemData(i);
		UINT ConID = Param >> 16;
		if (pConnection->GetID() == ConID)
		{
			m_lvServiceInfos.SetItemText(i,2,_T("已断开"));
		}
	}
}





void CServerManagerClientView::OnAddFileTask(UINT ID, CFileTransferQueue::FILE_TRANSFER_TYPE Type, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath)
{
	CString Temp;
	switch (Type)
	{
	case CFileTransferQueue::FILE_TRANSFER_TYPE_DOWNLOAD:
		Temp = _T("下载");
		break;
	case CFileTransferQueue::FILE_TRANSFER_TYPE_UPLOAD:
		Temp = _T("上传");
		break;
	case CFileTransferQueue::FILE_TRANSFER_TYPE_DELETE:
		Temp = _T("删除");
		break;
	case CFileTransferQueue::FILE_TRANSFER_TYPE_CREATE_DIR:
		Temp = _T("建目录");
		break;
	case CFileTransferQueue::FILE_TRANSFER_TYPE_CHANGE_FILE_MODE:
		Temp = _T("改属性");
		break;
	default:
		Temp = _T("未知");
		break;
	}
	int Item = m_lvTask.InsertItem(m_lvTask.GetItemCount(), Temp);
	m_lvTask.SetItemText(Item, 1, _T("0%"));
	m_lvTask.SetItemText(Item, 2, SourceFilePath);
	m_lvTask.SetItemText(Item, 3, TargetFilePath);
	m_lvTask.SetItemData(Item, ID);
}
void CServerManagerClientView::OnDeleteFileTask(UINT ID)
{
	for (int i = 0; i < m_lvTask.GetItemCount(); i++)
	{
		if ((UINT)m_lvTask.GetItemData(i) == ID)
		{
			m_lvTask.DeleteItem(i);
			break;
		}
	}
}
void CServerManagerClientView::OnFileTaskUpdate(UINT ID, float Progress)
{
	for (int i = 0; i < m_lvTask.GetItemCount(); i++)
	{
		if ((UINT)m_lvTask.GetItemData(i) == ID)
		{
			CString Temp;
			Temp.Format(_T("%0.2f%%"), Progress * 100);
			m_lvTask.SetItemText(i, 1, Temp);
			break;
		}
	}
}

void CServerManagerClientView::OnDeleteAllFileTask()
{
	m_lvTask.DeleteAllItems();
}

void CServerManagerClientView::OnNMRClickServiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_SERVICE_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
	*pResult = 0;
}

void CServerManagerClientView::OnStartupService()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		CServerManagerClientApp::GetInstance()->StartupService(ConID, SrvID);
	}
}

void CServerManagerClientView::OnShutdownServce()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		CServerManagerClientApp::GetInstance()->ShutdownService(ConID, SrvID, false);
	}
}

void CServerManagerClientView::OnShutDownServiceForce()
{
	// TODO:  在此添加命令处理程序代码
	POSITION Pos = m_lvServiceInfos.GetFirstSelectedItemPosition();

	while (Pos)
	{
		int Item = m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		CServerManagerClientApp::GetInstance()->ShutdownService(ConID, SrvID, true);
	}
}

void CServerManagerClientView::OnLvnColumnclickServiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_SortParam.pListCtrl=&m_lvServiceInfos;
	m_SortParam.SortColumn=pNMLV->iSubItem;

	m_lvServiceInfos.SortItemsEx(&CServerManagerClientView::ItemSortCallBack,(DWORD_PTR)&m_SortParam);
}


int CServerManagerClientView::ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SORT_PARAM * pSortParam=(SORT_PARAM *)lParamSort;

	CString Text1=pSortParam->pListCtrl->GetItemText(lParam1,pSortParam->SortColumn);
	CString Text2=pSortParam->pListCtrl->GetItemText(lParam2,pSortParam->SortColumn);

	return Text1.CompareNoCase(Text2);
}
void CServerManagerClientView::OnPopupBrowseWorkDir()
{
	// TODO: 在此添加命令处理程序代码
	POSITION Pos=m_lvServiceInfos.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		m_DlgWorkDirBowser.ShowWindow(SW_SHOW);
		m_DlgWorkDirBowser.SetServerInfo(this, ConID, SrvID);
		m_DlgWorkDirBowser.Browse(_T(""));		
	}
}

void CServerManagerClientView::OnShowHideService()
{
	// TODO: 在此添加命令处理程序代码
	if(m_ShowHideService)
		m_ShowHideService=false;
	else
		m_ShowHideService=true;
	m_lvServiceInfos.DeleteAllItems();
}

void CServerManagerClientView::OnUpdateShowHideService(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_ShowHideService);
}





void CServerManagerClientView::OnNMDblclkServiceList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	OnPopupBrowseWorkDir();
}


void CServerManagerClientView::OnTvnSelchangedTypeTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	m_SelectedConnectionID = pNMTreeView->itemNew.lParam;
	m_lvServiceInfos.DeleteAllItems();
	if (m_SelectedConnectionID)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_SelectedConnectionID);
		if (pConnection)
		{
			const CEasyArray<SERVICE_INFO>& ServiceList = pConnection->GetServiceList();
			for (UINT i = 0; i < ServiceList.GetCount(); i++)
			{
				SetServiceInfo(pConnection, ServiceList[i]);
			}
		}
	}
	else
	{
		void * Pos = CServerManagerClientApp::GetInstance()->GetFirstServerConnectionPos();
		while (Pos)
		{
			CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetNextServerConnection(Pos);
			const CEasyArray<SERVICE_INFO>& ServiceList = pConnection->GetServiceList();
			for (UINT i = 0; i < ServiceList.GetCount(); i++)
			{
				SetServiceInfo(pConnection, ServiceList[i]);
			}
		}
	}
}


void CServerManagerClientView::OnPopupAddService()
{
	// TODO:  在此添加命令处理程序代码
	HTREEITEM hItem = m_tvServers.GetSelectedItem();
	if (hItem)
	{
		UINT ConnectionID = (UINT)m_tvServers.GetItemData(hItem);
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ConnectionID);
		if (pConnection)
		{
			if (m_DlgServiceEditor.Init(pConnection, 0))
			{
				m_DlgServiceEditor.ShowWindow(SW_SHOW);
				return;
			}			
		}
	}
	AfxMessageBox(_T("请选择一个服务器"));
}


void CServerManagerClientView::OnPopupEditService()
{
	// TODO:  在此添加命令处理程序代码
	POSITION Pos = m_lvServiceInfos.GetFirstSelectedItemPosition();

	if (Pos)
	{
		int Item = m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ConID);
		if (pConnection)
		{
			if (m_DlgServiceEditor.Init(pConnection, SrvID))
			{
				m_DlgServiceEditor.ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("请选择一个服务"));
	}
}


void CServerManagerClientView::OnPopupDelService()
{
	// TODO:  在此添加命令处理程序代码
	POSITION Pos = m_lvServiceInfos.GetFirstSelectedItemPosition();

	if (Pos)
	{
		int Item = m_lvServiceInfos.GetNextSelectedItem(Pos);
		UINT Param = m_lvServiceInfos.GetItemData(Item);
		UINT ConID = Param >> 16;
		UINT SrvID = Param & 0xFFFF;
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ConID);
		if (pConnection)
		{
			const SERVICE_INFO * pServiceInfo = pConnection->GetServiceInfo(SrvID);
			if (pServiceInfo)
			{
				if (AfxMessageBoxEx(MB_YESNO, 0, _T("是否要删除[%s]上的服务[%s]?"),
					(LPCTSTR)pConnection->GetServerAddress(), (LPCTSTR)pServiceInfo->Name) == IDYES)
				{
					pConnection->QueryDelService(pServiceInfo->ServiceID);
				}
			}
		}
	}
	else
	{
		AfxMessageBox(_T("请选择一个服务"));
	}
}
