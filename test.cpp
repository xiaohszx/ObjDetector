#include "config.h"
#include "python.h"

//////////////////////////////////////////////////////////////////////////
// Python ����
#define PYTHON_HOME L"D:/Anaconda3/envs/tfgpu"

// ���� numpy �е�ͷ�ļ�arrayobject.h
#include "..\Lib\site-packages\numpy\core\include\numpy\arrayobject.h"

// ��ʼ�� numpy ִ�л�������Ҫ�ǵ����
// python2.7��void�������ͣ�python3.0������int��������
int init_numpy(){

	import_array();
	return 0;
}

/************************************************************************
* @class pyCaller
* @brief python������: �����ڶ�ͼƬ����Ƶ����ʶ��
* @author Ԭ����, 2018-4-11
************************************************************************/
class pyCaller
{
private:
	PyObject* pModule;							// pythonģ��
	PyObject* pFunMap;							// �����б�

public:
	/**
	* @brief ����һ��pyCaller���󣬽���py�ű�������Ϊ�������
	*/
	pyCaller(const char * module_name)
	{
		clock_t t = clock();
		Py_SetPythonHome(PYTHON_HOME);
		Py_Initialize();
		init_numpy();
		pModule = PyImport_ImportModule(module_name);
		if (NULL == pModule)
			OUTPUT("PyImport_ImportModule failed.\n");
		t = clock() - t;
		OUTPUT("PyImport_ImportModule using %d ms.\n", t);
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
		if (pFunMap)
			Py_DECREF(pFunMap);
		Py_Finalize();
	}
	/**
	* @brief ʹ��ǰ����ָ�����Ƶĺ���
	*/
	bool ActivateFunc(const char * func_name)
	{
		return (pFunMap =  pModule ? PyObject_GetAttrString(pModule, func_name) : 0);
	}
	/**
	* @brief ����python�ű��е�ָ������
	*/
	void CallFunction(const char * func_name, PyObject *arg)
	{
		if (pFunMap && arg)
		{
			PyObject * pRetVal = PyEval_CallObject(pFunMap, arg);
			if (NULL == pRetVal)
				return;
			if (PyTuple_Check(pRetVal))
			{
				int SizeOfTuple = PyTuple_Size(pRetVal);
				for(int i = 0; i < SizeOfTuple; ++i)
				{
					PyArrayObject *ListItem = (PyArrayObject *)PyTuple_GetItem(pRetVal, i);
					int x1 = ListItem->dimensions[0], x2 = ListItem->dimensions[1], 
						x3 = ListItem->dimensions[2];
					int p[256], num = x1 * x2 * x3 ;
					memcpy(p, ListItem->data, num * sizeof(int));
					for (int j = 0; j < num; ++j)
						printf("%d, ", p[j]);
					printf("\n\n\n\n");
					for(int r = 0; r < x1; ++r)
					{
						printf("\n\n");
						for (int c = 0; c < x2; ++c)
						{
							for (int k = 0; k < x3; ++k)
								printf("%d\t", 
								*(int *)(ListItem->data + r * ListItem->strides[0] + 
								c * ListItem->strides[1] + k * ListItem->strides[2]));
							printf("\n");
						}
					}
				}
			}
		}
	}
};


int main(int argc, const char *argv[])
{
	const char *path = argc == 1 ? "image.jpg" : argv[1];

	pyCaller py("show_image");
	py.ActivateFunc("show_image");

	Mat im = imread(path);

	npy_intp dims[] = {im.rows, im.cols, im.step[1]}; // ����ά����Ϣ
	// ���ɰ��������ά�����PyObject����ʹ��PyArray_SimpleNewFromData����
	// ��һ������2��ʾά�ȣ��ڶ���Ϊά������Dims,����������ָ����������ͣ����ĸ�����Ϊ����
	PyObject *PyArray  = PyArray_SimpleNewFromData(3, dims, NPY_UBYTE, im.data); 
	// ͬ�������С��Python������������һ�µ�PyTuple����
	PyObject *ArgArray = PyTuple_New(1);
	PyTuple_SetItem(ArgArray, 0, PyArray); 
	// ���ú���������Numpy Array ����
	py.CallFunction("show_image", ArgArray);

	return getchar();
}
