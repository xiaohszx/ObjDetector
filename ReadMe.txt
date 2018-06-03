这是一个MFC(C++)调用Python（TensorFlow）的Visual Studio项目。

#CallPython
调用TensorFlow对一张图像进行目标检测的例子。

#objDetector
一个基于对话框的调用TensorFlow进行目标检测的例子。

#编译前置条件
0、已训练好的模型（frozen_inference_graph.pb）
1、安装python3.5 64位，安装OpenCV 64位
2、pip install numpy
3、pip install pillow
4、pip install matplotlib
5、pip install tensorflow
6、其他包视情况而定

#编译注意事项

1、为项目添加Python的附加包含目录及库目录，复制pythonXX.lib的备份，并重命名为pythonXX_d.lib;

1、根据个人计算机，编译好openCV，并在stdafx.h或其他链接的位置配置OpenCV版本及路径；

2、error LNK2019: 无法解析的外部符号 __imp___Py_NegativeRefcount、__imp___Py_RefTotal解决方案：

    注释掉object.h文件第56行 #define Py_TRACE_REFS
	
3、根据个人设置，修改Py_SetPythonHome中python的目录（宏：PYTHON_HOME）；

4、需要将被调用的 *.py 文件拷贝到生成目录；

5、需要给定目标检测的模型文件，并在相应的 *.py 文件中填写。

#补充说明 2018.6.3

本程序测试所用的模型：Faster R-CNN with Inception v2（faster_rcnn_inception_v2）

有问题，请联系：yuan_yuanxiang@163.com

															2018-4-16

															袁沅祥 注
