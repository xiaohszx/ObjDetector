#include "python.h"
#include <map>
#include <string>
#include "StrTransfer.h"

#pragma once

// ���ɼ��Ŀ�����
#define MAX_BOXES_NUM 100

// Py_DECREF ���³����������ʱ����
#define My_DECREF(p) 

#ifdef _AFX
#define OUTPUT TRACE
#else
#define OUTPUT printf
#endif

// ���� numpy �е�ͷ�ļ�arrayobject.h
#include "..\Lib\site-packages\numpy\core\include\numpy\arrayobject.h"
using namespace std;


enum 
{
	_boxes = 0, 
	_scores, 
	_classes, 
	_num_detections
};

/************************************************************************
* @class tfOutput
* @brief tensorflowģ������Ĳ����ṹ
************************************************************************/
class tfOutput
{
protected:
	int *ref;				// ���ü���
	int addref() const { return ++ (*ref); }
	int removeref() const { return -- (*ref); }
	void destroy()			// ����
	{
		if (ref && 0 == removeref())
		{
			SAFE_DELETE_ARRAY(ref);
			SAFE_DELETE_ARRAY(boxes);
			SAFE_DELETE_ARRAY(scores);
			SAFE_DELETE_ARRAY(classes);
			SAFE_DELETE_ARRAY(counts);
		}
	}
public:
	int n;					// ������(n)
	float *boxes;			// n�� 100 x 4 ����
	float *scores;			// n x 100 ����
	float *classes;			// n x 100 ����
	float *counts;			// n ά����(ÿ�����)

	/**
	* @brief ����һ��n��tensorflowģ�Ͳ���
	* Ĭ�Ϲ����Ϊ�ղ���
	*/
	tfOutput(int class_num = 0)
	{
		memset(this, 0, sizeof(tfOutput));
		n = class_num;
		if (n)
		{
			boxes = new float[n * MAX_BOXES_NUM * 4]();
			scores = new float[n * MAX_BOXES_NUM]();
			classes = new float[n * MAX_BOXES_NUM]();
			counts = new float[n]();
			ref = new int(1);
		}
	}
	~tfOutput()
	{
		destroy();
	}
	tfOutput(const tfOutput &o)
	{
		ref = o.ref;
		n = o.n;
		boxes = o.boxes;
		scores = o.scores;
		classes = o.classes;
		counts = o.counts;
		addref();
	}
	tfOutput operator = (const tfOutput &o)
	{
		// ����������
		destroy();
		// this��o����
		ref = o.ref;
		n = o.n;
		boxes = o.boxes;
		scores = o.scores;
		classes = o.classes;
		counts = o.counts;
		addref();
		return *this;
	}

	// ����ָ��Ԫ��
	inline float p(int r, int c, int i = 0) const
	{
		return boxes[i * (MAX_BOXES_NUM*4) + r * 4 + c];
	}

	// ��ӡ��i��ʶ����
	void PrintBoxes(int i = 0) const 
	{
		if (i < n)
		{
			const float *header = boxes + i * (MAX_BOXES_NUM*4);
			for (int i = 0; i < MAX_BOXES_NUM; ++i)
			{
				const float *row = header;
				OUTPUT("%f\t %f\t %f\t %f\n", *row++, *row++, *row++, *row++);
				header += 4;
			}
		}
	}
};

/************************************************************************
* @class pyCaller
* @brief python������: �����ڶ�ͼƬ����Ƶ����ʶ��
* @author Ԭ����, 2018-4-11
************************************************************************/
class pyCaller
{
private:
	PyObject* pModule;							// pythonģ��
	std::map<std::string, PyObject*> pFunMap;	// �����б�

	// ��ʼ�� numpy ִ�л�������Ҫ�ǵ����
	// python2.7��void�������ͣ�python3.0������int��������
	inline int init_numpy(){

		import_array();
		return 0;
	}

	// ����python���
	tfOutput ParseResult(PyObject *pRetVal);

public:
	/**
	* @brief ����һ��pyCaller���󣬽���py�ű�������Ϊ�������
	*/
	pyCaller(const char * module_name)
	{
		clock_t t = clock();
		Py_SetPythonHome(L"D:/Anaconda3/envs/tfgpu");
		Py_Initialize();
		init_numpy();
		pModule = PyImport_ImportModule(module_name);
		if (NULL == pModule)
			OUTPUT("PyImport_ImportModule failed.\n");
		t = clock() - t;
		char szOut[128];
		sprintf_s(szOut, "PyImport_ImportModule using %d ms.\n", t);
		OutputDebugStringA(szOut);
#ifndef _AFX
		printf(szOut);
#endif
		if (0 == Py_IsInitialized())
		{
			OUTPUT("Py_IsInitialized = 0.\n");
		}
	}

	/**
	* @brief ����ʼ��python�������ͷ�python������ڴ�
	*/
	~pyCaller()
	{
		if (pModule)
			Py_DECREF(pModule);
		for (std::map<std::string, PyObject*>::iterator p = pFunMap.begin(); 
			p != pFunMap.end(); ++p)
			if (p->second) Py_DECREF(p->second);
		Py_Finalize();
	}

	/**
	* @brief ʹ��ǰ����ָ�����Ƶĺ���
	*/
	bool ActivateFunc(const char * func_name)
	{
		bool bFind = false;
		std::string fun(func_name);
		for (std::map<std::string, PyObject*>::iterator p = pFunMap.begin(); 
			p != pFunMap.end(); ++p)
		{
			if (p->first == fun)
			{
				bFind = true;
				break;
			}
		}
		if (bFind)
			return true;

		PyObject *pFunc =  pModule ? PyObject_GetAttrString(pModule, func_name) : 0;
		pFunMap.insert(std::make_pair(func_name, pFunc));

		return pFunc;
	}

	/**
	* @brief ����python�ű��е�ָ������
	*/
	tfOutput CallFunction(const char * func_name, const char *arg)
	{
		tfOutput out;
		PyObject *pFunc = pFunMap[func_name];
		if (pFunc)
		{
			const char *utf8 = MByteToUtf8(arg);
			PyObject* pArg = Py_BuildValue("(s)", utf8);
			delete [] utf8;
			if (NULL == pArg)
				return out;
			PyObject* pRetVal = PyEval_CallObject(pFunc, pArg);
			if (NULL == pRetVal)
				return out;
			out = ParseResult(pRetVal);
		}
		return out;
	}
	/**
	* @brief ����python�ű��е�ָ������
	*/
	tfOutput CallFunction(const char * func_name, PyObject *arg)
	{
		tfOutput out;
		PyObject *pFunc = pFunMap[func_name];
		if (pFunc)
		{
 			PyObject* pRetVal = PyEval_CallObject(pFunc, arg);
			if (NULL == pRetVal)
				return out;
			out = ParseResult(pRetVal);
		}
		return out;
	}
};
