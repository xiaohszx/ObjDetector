#pragma once
#include "afxwin.h"


// CSettingsDlg �Ի���

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingsDlg();

// �Ի�������
	enum { IDD = IDD_SETTINGS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ThreshSave;
	CEdit m_ThreshShow;
	CEdit m_BufferSize;
	CEdit m_DetectStep;
	double m_fThreshSave;
	double m_fThreshShow;
	int m_nBufferSize;
	int m_nDetectStep;
};
