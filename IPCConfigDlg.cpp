// IPCConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "objDetector.h"
#include "IPCConfigDlg.h"
#include "afxdialogex.h"


// CIPCConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CIPCConfigDlg, CDialogEx)

CIPCConfigDlg::CIPCConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIPCConfigDlg::IDD, pParent)
	, m_strAddress(_T("192.168.12.153"))
	, m_nPort(8000)
	, m_strUser(_T("admin"))
	, m_strPassword(_T("admin123"))
{

}

CIPCConfigDlg::~CIPCConfigDlg()
{
}

void CIPCConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPC_ADDRESS, m_CtrlAddress);
	DDX_Control(pDX, IDC_IPC_PORT, m_CtrlPort);
	DDX_Control(pDX, IDC_IPC_USER, m_CtrlUser);
	DDX_Control(pDX, IDC_IPC_PASSWORD, m_CtrlPassword);
	DDX_Text(pDX, IDC_IPC_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_IPC_PORT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 1, 32767);
	DDX_Text(pDX, IDC_IPC_USER, m_strUser);
	DDX_Text(pDX, IDC_IPC_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CIPCConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPCConfigDlg ��Ϣ�������
