����һ��MFC(C++)����Python��TensorFlow����Visual Studio��Ŀ��

#CallPython
����TensorFlow��һ��ͼ�����Ŀ��������ӡ�

#pbjDetector
һ�����ڶԻ���ĵ���TensorFlow����Ŀ��������ӡ�

#ShowImage
һ��C++����Python����numpy array�����ӡ�

#����ǰ������
0����ѵ���õ�ģ�ͣ�frozen_inference_graph.pb��
1����װpython3.5 64λ����װOpenCV 64λ
2��pip install numpy
3��pip install pillow
4��pip install matplotlib
5��pip install tensorflow
6�����������������

#����ע������

1��Ϊ��Ŀ���Python�ĸ��Ӱ���Ŀ¼����Ŀ¼������pythonXX.lib�ı��ݣ���������ΪpythonXX_d.lib;

1�����ݸ��˼�����������openCV������stdafx.h���������ӵ�λ������OpenCV�汾��·����

2��error LNK2019: �޷��������ⲿ���� __imp___Py_NegativeRefcount��__imp___Py_RefTotal���������

    ע�͵�object.h�ļ���56�� #define Py_TRACE_REFS
	
3�����ݸ������ã��޸�Py_SetPythonHome��python��Ŀ¼���꣺PYTHON_HOME����

4����Ҫ�������õ� *.py �ļ�����������Ŀ¼��

5����Ҫ����Ŀ�����ģ���ļ���������Ӧ�� *.py �ļ�����д��

�����⣬����ϵ��yuan_yuanxiang@163.com

															2018-4-16

															Ԭ���� ע
