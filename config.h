// ��ע�⡿�뽫OpenCV�İ���Ŀ¼��python����Ŀ¼��ӵ���Ŀ���Ӱ���Ŀ¼
// ��python�Ŀ�Ŀ¼��ӵ���Ŀ�ĸ��ӿ�Ŀ¼
// Ȼ�����ʵ������޸ı��ļ���


//////////////////////////////////////////////////////////////////////////
// OpenCV ����
#define CV_VER "320"

#if _MSC_VER < 1900 // Visual Studio 2015���°汾��ʹ�������궨��������ӿ�

#ifdef _DEBUG
#define CV_LIB_PATH "D:/opencv/opencv32/x64/lib/Debug/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##"d.lib"
#else
#define CV_LIB_PATH "D:/opencv/opencv32/x64/lib/Release/"
#define CV_LIB_X(LIB, VER) CV_LIB_PATH##"opencv_"##LIB##VER##".lib"
#endif

#define USING_CV_LIB(LIB) CV_LIB_X(LIB, CV_VER) // ʹ��CV�Ŀ�


//////////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
using namespace cv;

#pragma comment(lib, USING_CV_LIB("core"))
#pragma comment(lib, USING_CV_LIB("highgui"))
#pragma comment(lib, USING_CV_LIB("imgproc"))
#pragma comment(lib, USING_CV_LIB("video"))
#pragma comment(lib, USING_CV_LIB("videoio"))
#pragma comment(lib, USING_CV_LIB("imgcodecs"))
#pragma comment(lib, USING_CV_LIB("photo"))

#else

#include "opencv2/opencv.hpp" 
#include "opencv/cv.h"
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "opencv_core320d.lib")
#pragma comment(lib, "opencv_highgui320d.lib")
#pragma comment(lib, "opencv_imgproc320d.lib")
#pragma comment(lib, "opencv_video320d.lib")
#pragma comment(lib, "opencv_videoio320d.lib")
#pragma comment(lib, "opencv_imgcodecs320d.lib")
#pragma comment(lib, "opencv_photo320d.lib")
#else
#pragma comment(lib, "opencv_core320.lib")
#pragma comment(lib, "opencv_highgui320.lib")
#pragma comment(lib, "opencv_imgproc320.lib")
#pragma comment(lib, "opencv_video320.lib")
#pragma comment(lib, "opencv_videoio320.lib")
#pragma comment(lib, "opencv_imgcodecs320.lib")
#pragma comment(lib, "opencv_photo320.lib")
#endif

#endif

#ifdef _AFX
#define OUTPUT TRACE
#else
#define OUTPUT printf
#endif


// �Ƿ�֧��IPC
#define SUPPORT_IPC 1

// �Ƿ�ʹ��⺯��Ϊstatic(�ܱ���Ƶ������map)
#define STATIC_DETECTING 1

// ������ʶ������ͼ�����������С�ߴ�
#define MIN_FACESIZE 64
