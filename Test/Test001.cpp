
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include<iostream>
#include<Python.h>
//#include<numpy/arrayobject.h>
#include"numeric"
#include"numpy/arrayobject.h"

//下面这个库是C++和python协同库
//#include <numpy/arrayobject.h>
//#include <Python.h>
//显式调用python  g++ -I /usr/include/python3.6 inputArrayTest.cpp -L /usr/lib/python3.6/config***/ -l python3.6
//https://www.cnblogs.com/tevic/p/3645197.html
//https://blog.csdn.net/stu_csdn/article/details/69488385
//https://numpy.org/doc/stable/reference/c-api/types-and-structures.html?highlight=pyarrayobject#c.PyArrayObject

using namespace std;

void hello(){
    //Py_Initialize();
    
    PyObject* pModule=NULL;
    PyObject* pFunct=NULL;
    //PyObject* pArgs=NULL;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    pModule=PyImport_ImportModule("Test001");
    PyRun_SimpleString("print('-------------------')");
    pFunct=PyObject_GetAttrString(pModule,"hello");

    //创建参数
    PyObject* pArgs=PyTuple_New(1);
    PyTuple_SetItem(pArgs,0,Py_BuildValue("s","hello python,im c++"));
    PyObject_CallObject(pFunct,pArgs);
    //Py_Finalize();

}

int init_numpy(){
    
    import_array();
    return 0;
}

int main(){

    
    Py_Initialize();
    hello();
    init_numpy();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    PyObject* pModule=nullptr;
    PyObject* pDict=nullptr;

    pModule=PyImport_ImportModule("Test001");
    pDict=PyModule_GetDict(pModule);

    //返回numpy array list
    PyObject* pFuncOne=PyDict_GetItemString(pDict,"test_ret");
    PyObject* FuncOneRet=PyObject_CallObject(pFuncOne,nullptr);

    int index_i=0,index_k=0,index_m=0,index_n=0;

    if (PyList_Check(FuncOneRet))
    {
        /* code */
        int sizeofList=PyList_Size(FuncOneRet);

        for (index_i = 0; index_i < sizeofList; index_i++)
        {
            /* code */
            PyArrayObject* listItem=(PyArrayObject*)PyList_GetItem(FuncOneRet,index_i);
            int rows=PyArray_DIM(listItem,0);
            int columns=PyArray_DIM(listItem,1);
            void* data_p=PyArray_DATA(listItem);
            cout<<"the"<<index_i<<"th array is:"<<endl;
            for (index_m = 0; index_m < rows; index_m++)
            {
                /* code */
                for (index_n = 0; index_n < columns; index_n++)
                {
                    /* code */
                    int stride1=PyArray_STRIDE(listItem,0);
                    int stride2=PyArray_STRIDE(listItem,1);
                    
                    cout<<*(double*)(data_p+index_m*stride1+index_n*stride2)<<" ";

                }
                cout<<endl;
            }
            Py_DECREF(listItem);
            
            
        }

        
    }
    else{
        cout<<"not a list"<<endl;
    }
    
    //


    Py_Finalize();

    //hello();

    return 0;
}