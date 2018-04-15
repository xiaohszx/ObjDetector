
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

// OpenCV
#define USING_OPENCV 1

// Tensorflow
#define USING_TENSORFLOW 1

#ifdef _DEBUG
// ���û�� Visual Leak Detector ����ע�ʹ���
#if !USING_TENSORFLOW
#include "vld.h"
#endif
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#if USING_OPENCV

#include<opencv2/opencv.hpp>   
#include<opencv/cv.h> 
using namespace cv;

#define CV_VER "320"

#ifdef _DEBUG
#define CV_LIB_PATH "D:/opencv/opencv32/lib/Debug/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##"d.lib"
#else
#define CV_LIB_PATH "D:/opencv/opencv32/lib/Release/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##".lib"
#endif

#define USING_CV_LIB(LIB) CV_LIB_X(LIB, CV_VER)

#pragma comment(lib, USING_CV_LIB("core"))
#pragma comment(lib, USING_CV_LIB("highgui"))
#pragma comment(lib, USING_CV_LIB("imgproc"))
#pragma comment(lib, USING_CV_LIB("video"))
#pragma comment(lib, USING_CV_LIB("videoio"))
#pragma comment(lib, USING_CV_LIB("imgcodecs"))
#pragma comment(lib, USING_CV_LIB("photo"))

#endif


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

// �߾��ȵ�˯�ߺ���
#define Sleep_m(ms) { timeBeginPeriod(1); Sleep(ms); timeEndPeriod(1); }

// �Բ���n����������C�µȴ�T��(n�ǲ���������������1000)
#define WAIT_n(C, T, n) {assert(!(1000%(n)));int s=(1000*(T))/(n);do{Sleep(n);}while((C)&&(--s));}

// ������C����ʱ�ȴ�T��(����10ms)
#define WAIT(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 10); timeEndPeriod(1); }

// ������C����ʱ�ȴ�T��(����1ms)
#define WAIT_1(C, T) { timeBeginPeriod(1); WAIT_n(C, T, 1); timeEndPeriod(1); }
