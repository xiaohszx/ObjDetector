
// objDetectorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "pyCaller.h"
#include "FileReader.h"

// ͼ���ע
struct Tips
{
	float score;
	cv::Rect rect;
	Tips() { memset(this, 0, sizeof(Tips)); }
	Tips(const cv::Rect &rt, float s) : score(s), rect(rt) { }
};

// ý��״̬
enum MediaState
{
	STATE_DONOTHING = 0,			// ����
	STATE_DETECTING,				// ���
	STATE_PLAYING,					// ����
	STATE_PAUSE, 
};

#define Thread_Start    1			// �߳�����
#define Thread_Stop		0			// �߳�ͣ��

// �߳�����ö��
enum Thread
{
	_InitPyCaller = 0, 
	_DetectVideo, 
	_PlayVideo,
	_Max
};

// CobjDetectorDlg �Ի���
class CobjDetectorDlg : public CDialogEx
{
// ����
public:
	CobjDetectorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OBJDETECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	MediaState m_nMediaState;		// ý��״̬

	char m_strFile[_MAX_PATH];		// �ļ�·��

	CFileReader m_reader;			// �ļ���ȡ��

	pyCaller *m_py;					// python������

	bool m_bExit;					// �Ƿ��˳�����

	bool m_bOK;						// pyCaller�Ƿ��λ

	CRect m_rtPaint;				// ��ͼ����

	HDC m_hPaintDC;					// ��ͼHDC

	int m_nThreadState[_Max];		// �߳�״̬

	// �Ƿ����ڼ��
	bool IsDetecting() const { return STATE_DETECTING == m_nMediaState; }

	// �Ƿ���æ
	bool IsBusy() const { return STATE_DETECTING == m_nMediaState || STATE_PLAYING == m_nMediaState; }

	// ��ʼ��python���û���
	static void InitPyCaller(LPVOID param);
	// �����Ƶ
	static void DetectVideo(LPVOID param);
	// ��ʾ�����
	static void PlayVideo(LPVOID param);
	// ����Ŀ����
	std::vector<Tips> DoDetect(cv::Mat &m);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_picCtrl;
	afx_msg void OnFileOpen();
	afx_msg void OnDestroy();
	afx_msg void OnObjDetect();
	afx_msg void OnFileClose();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateObjDetect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
	afx_msg void OnFileQuit();
	afx_msg void OnEditPlay();
	afx_msg void OnUpdateEditPlay(CCmdUI *pCmdUI);
	afx_msg void OnEditPause();
	afx_msg void OnUpdateEditPause(CCmdUI *pCmdUI);
	afx_msg void OnEditStop();
	afx_msg void OnUpdateEditStop(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
