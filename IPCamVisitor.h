#pragma once

#include "config.h"

#include "HC_SDK/HCNetSDK.h"
#include "HC_SDK/playm4.h"

#define WIDTH 2048

#define HEIGHT 1152

// IPC����
typedef struct IPCamInfo
{
	int port;
	char ip[64];
	char user[128];
	char pwd[128];
	IPCamInfo() { memset(this, 0, sizeof(IPCamInfo)); }
	IPCamInfo(const char *_ip, int _port)
	{
		port = _port;
		strcpy_s(ip, _ip);
		strcpy_s(user, "admin");
		strcpy_s(pwd, "admin");
	}
}IPCamInfo;


/************************************************************************
* @class IPCamVisitor
* @brief IPC������
************************************************************************/
class IPCamVisitor
{
private:
	// �����С
	int m_nLen;
	// ��½ID
	LONG m_user;
	// ȡ��ͨ��
	LONG m_port;

	// ͼ�񻺴�
	BYTE *m_buf;
	// RGB����
	BYTE *m_rgb;
	// ͼ������
	const BYTE *m_data;				// ָ��ͼ�񻺴�
	// ͼ������
	const BITMAPINFOHEADER *m_head;	// ָ��ͼ�񻺴�

	// IPC�豸����Ϣ
	IPCamInfo m_cam;

	// �����Ƿ��㹻
	bool IsBufferEnough(int w, int h) const 
	{
		return sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + w * h * 4 <= m_nLen;
	}

public:
	IPCamVisitor();

	~IPCamVisitor(void);

	void Create(int nBufferLen = WIDTH * HEIGHT * 4);

	void Release();

	LONG LoginCamera(const IPCamInfo &info, HWND hWnd);

	void LogoutCamera();

	cv::Mat GetCapture();
};
