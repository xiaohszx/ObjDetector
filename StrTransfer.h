/************************************************************************/
/* �ַ�����ʽת��                                                       */
/************************************************************************/

#pragma once

#include <wtypes.h>

#define SAFE_DELETE_ARRAY(p) if(p) { delete [] (p); (p) = 0; } 

//	���ֽ�ת���ɿ��ֽ�
WCHAR* MByteToWChar(const char *pszStr);

//	���ֽ�ת����UTF8
char* WCharToUtf8(const WCHAR *pcwszStr);

//	���ֽ�ת����UTF8
char* MByteToUtf8(const char *pszStr);
