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

class IPCamVisitor
{
private:
	// ��½ID
	LONG m_user;
	// IPC�豸����Ϣ
	IPCamInfo m_cam;
	// ȡ��ͨ��
	LONG m_port;
	// ����
	BYTE *m_buf;
	// �����С
	int m_nLen;

public:
	IPCamVisitor();

	~IPCamVisitor(void);

	LONG LoginCamera(const IPCamInfo &info, HWND hWnd);

	void LogoutCamera();

	cv::Mat GetCapture();
};
