介绍：
    接口以加速viterbi算法的运算速度为目的，采用python和C++嵌入编程，使用C++代替python中的循环及算法操作，将最终得到的结果通过调用python接口返回给python程序，从而进行下一步计算或操作。

Requipments:
    python库：
        1.Python3及以上
        2.Numpy
        3.Scipy
        4.H5py

    C++库：
        1.Python.h
        2.numpy/arrayobject.h（numpy库需要包含至系统路径或用户路径，若没包含，可以通过将python库复制至当前文件夹，然后直接引入该头文件亦可）

    编译环境：
        linux/windows

接口函数说明：
    当作为python接口引入C++运行时：
        1.python接口
            1.接口函数
                parse_python_viterbi_data_opration  调用此函数将获取python传入的参数，此时只需该函数返回需要传递的参数即可
            2.具体操作步骤
                通过形如 pModule=PyImport_ImportModule("Test002");
                        pFunct=PyObject_GetAttrString(pModule,"test_data");
                        pFunRet=PyObject_CallObject(pFunct,nullptr);//nullptr表示无参数传递至python
                        PyArg_ParseTuple(pFunRet,"OsO",&a,&b,&c);
                即可得到python返回（传入）的数据，这里pModule,pFunct,pFunRet均为PyObject类型指针
        2.C++接口
            1.g++编译为动态库
            2.使用CPython或Swig方式导入
            3.调用C++接口函数运行即可返回计算后的数据
            详见目录下python调用C++/pythonC接口说明文件
            4.C++中参数传递详见https://blog.csdn.net/stu_csdn/article/details/69488385#comments_13185047
                

 
编译：
    1.编译指令：
        g++ Test002copy2.py -I /usr/include/python3.6 -L /usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/ -l python3.6 -o Test002copy2 -g



