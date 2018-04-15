#pragma once

#include <exception>
#include "CvvImage.h"
#include <queue>

// �ļ�����
enum FileType
{
	TYPE_UNKNOWN = 0,
	TYPE_IMAGE, 
	TYPE_VIDEO, 
};

class CFileReader
{
private:
	FileType m_nType;
	int m_nDims[3];
	int m_nMaxBuf;
	CvvImage m_Image;
	std::queue<const cv::Mat> m_Buffer;
	VideoCapture m_Cap;

	CRITICAL_SECTION m_cs;
	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }

public:
	CFileReader(void);
	~CFileReader(void);

	int dims(int n) const { return m_nDims[n]; }

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
	bool IsVideo() const { return TYPE_VIDEO == m_nType; }

	// ���Ŷ����Ƿ�Ϊ��
	bool IsEmpty();

	// ���ļ�
	bool Open(const char *path);

	// ����ͼ��
	void Draw(HDC hDc, CRect &rt);

	void Clear();

	void Destroy();

	// �Ƿ����ڻ�����
	bool IsBuffering();

	// ��ȡ��һ֡ͼ��
	cv::Mat Front();
};
