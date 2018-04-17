#pragma once

#include <exception>
#include "CvvImage.h"
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

class CFileReader
{
private:
	FileType m_nType;					// �ļ�����
	int m_nDims[3];						// ͼ��ά��
	int m_nMaxBuf;						// ����֡��
	CvvImage m_Image;					// ��ͼͼ��
	std::queue<const cv::Mat> m_Buffer;	// �������
	VideoCapture m_Cap;					// ��Ƶ��ȡ��
	IPCamVisitor m_IPC;					// IPC
	HWND m_hWnd;

	CRITICAL_SECTION m_cs;
	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }

public:
	CFileReader(void);
	~CFileReader(void);

	// ��ȡͼ��ά��
	int dims(int n) const { return m_nDims[n]; }

	// ��ȡ��󻺴�֡��
	int getMaxBuf() const { return m_nMaxBuf; }

	// �򲥷Ŷ������һ֡ͼ��
	bool PushImage(const cv::Mat &m);

	// �Ӳ��Ŷ���ȡ��һ֡ͼ��
	int PopImage();

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

	// ���Ŷ����Ƿ�Ϊ��
	bool IsEmpty();

	// ���ļ�
	bool Open(const char *path);

	// ��IP�����
	bool OpenIPCamera(const IPCamInfo &cam, HWND hWnd);

	// ����ͼ��
	void Draw(HDC hDc, CRect &rt);

	// ����
	void Clear();

	// ����
	void Destroy();

	// �Ƿ����ڻ�����
	bool IsBuffering();

	// ��ȡ��һ֡ͼ��
	cv::Mat Front();
};
