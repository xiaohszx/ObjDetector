#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CIPCConfigDlg �Ի���

class CIPCConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIPCConfigDlg)

public:
	CIPCConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIPCConfigDlg();

// �Ի�������
	enum { IDD = IDD_IPCAMERA_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CNetAddressCtrl m_CtrlAddress;
	CEdit m_CtrlPort;
	CEdit m_CtrlUser;
	CEdit m_CtrlPassword;
	CString m_strAddress;
	int m_nPort;
	CString m_strUser;
	CString m_strPassword;
};
