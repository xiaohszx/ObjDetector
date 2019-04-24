#pragma once

#include <exception>
#include <queue>
#include "IPCamVisitor.h"

// �ļ�����
enum FileType
{
	TYPE_UNKNOWN = 0,		// δ֪
	TYPE_IMAGE,				// ͼ��
	TYPE_VIDEO,				// ��Ƶ
	TYPE_IPC,				// IPC������
	TYPE_CAMERA,			// ���������ͷ
};

// ͼ��ά��ö��
enum DIMS
{
	IMAGE_HEIGHT = 0,		// ��
	IMAGE_ROWS = 0,			// ��
	IMAGE_WIDTH = 1,		// ��
	IMAGE_COLS = 1,			// ��
	IMAGE_CHANNEL = 2,		// ͨ��(ÿ�����ֽ���)
};

#define BUF_MIN_LEN 8		// ��С����֡��

/************************************************************************
* @class CFileReader
* @brief �ļ�/�� ��ȡ��
************************************************************************/
class CFileReader
{
private:
	FileType m_nType;					// �ļ�����
	int m_nDims[3];						// ͼ��ά��
	int m_nImageBuf;					// Ŀ��ʶ�𻺴�֡��
	int m_nStreamBuf;					// ��Ƶ������֡��
	std::queue<cv::Mat> m_Buffer;		// �������
	std::queue<cv::Mat> m_Stream;		// IPC Stream����
	VideoCapture m_Cap;					// ��Ƶ��ȡ��
	IPCamVisitor m_IPC;					// IPC ��ȡ��

	CRITICAL_SECTION m_cs;
	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }

	// ��ʱץȡIPCͼ����߳�
	static void CaptureThread(LPVOID param);

	// �߳��Ƿ�����
	bool m_bThreadStart;

	// ��IPC Stream ���һ֡ͼ��
	bool PushStream(const cv::Mat &m);

	// ��IPC Stream ȡ��һ֡ͼ��
	cv::Mat PopStream();

	// û��ȡ����Ƶ����
	bool NoStream();

	// ������ͷ��ȡһ֡ͼ��
	cv::Mat ReadCamera() { cv::Mat m; m_Cap.read(m); return m; }

public:
	CFileReader(void);
	~CFileReader(void);

	// �����߳�
	void StartThread();

	// ������󻺴�֡��
	void SetBufferSize(int nSize); 

	// ��ȡͼ��ά��
	int dims(DIMS n) const { return m_nDims[n]; }

	// �򲥷Ŷ������һ֡ͼ��
	bool PushImage(const cv::Mat &m);

	// �Ӳ��Ŷ���ȡ��һ֡ͼ��
	cv::Mat PopImage();

	// ����ͼ��
	cv::Mat PlayVideo();

	// �Ƿ�Ϊͼ��
	bool IsImage() const { return TYPE_IMAGE == m_nType; }

	// �Ƿ�Ϊ��Ƶ
	bool IsVideo() const { return m_nType > TYPE_IMAGE; }

	// �Ƿ�Ϊ�����ļ�
	bool IsFile() const { return m_nType < TYPE_IPC; }

	// �Ƿ�Ϊ��Ƶ��
	bool IsStream() const { return m_nType > TYPE_VIDEO; }

	// IPC
	bool IsIPC() const { return TYPE_IPC == m_nType; }

	// �Ƿ�Ϊ��
	bool IsEmpty() const { return TYPE_UNKNOWN == m_nType; }

	// ���ļ�
	bool Open(const char *path);

	// ��IP�����
	bool OpenIPCamera(const IPCamInfo &cam, HWND hWnd);

	// ����
	void Clear();

	// ����
	void Destroy();

	// �Ƿ����ڻ�����
	bool IsBuffering();

	// ��ȡ��һ֡ͼ��
	cv::Mat Front();
};
