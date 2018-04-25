#pragma once
#include "CvvImage.h"
#include "afxwin.h"

// ��ʱ����
#define TIME_SHOW 1

// CResultDlg �Ի���
// ��ģ̬�Ի������Ծ�������չ��
class CResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResultDlg();

	// �Ի�������
	enum { IDD = IDD_RESULT_DLG };

	int m_nTimer;

	int m_nClassId;		// ��ǰ���ID

	HDC m_hPaintDC;
	CRect m_rtPaint;

	CvvImage m_Image;

	// չʾ���
	void ShowResult(const cv::Mat &m, int class_id, int counts, float scores);

	// ������
	inline void ClearResult() { if(m_nCounts) { m_nCounts = 0; m_fScores = 0; FillRect(m_hPaintDC, m_rtPaint, CBrush(RGB(200, 200, 200))); } }

	// ֹͣչʾ
	inline void StopShow() { if(TIME_SHOW == m_nTimer) { KillTimer(m_nTimer); m_nTimer = 0; } ShowWindow(SW_HIDE); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CStatic m_Picture;
	CComboBox m_ComboClass;
	CEdit m_EditCounts;
	CEdit m_EditScores;
	int m_nCounts;
	float m_fScores;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
