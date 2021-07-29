
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


//传递多维数组
//https://blog.csdn.net/stu_csdn/article/details/69488385

//传递结构体
//
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
    Py_DECREF(pModule);
    Py_DECREF(pFunct);
    Py_DECREF(pArgs);

}

typedef struct heard_{
    int a;
    char b[11];
    int c;
}header;

bool init_numpy(){
    
    import_array();
    //return 0;
}
//做结构体传入暂时没解决
//做元组进行传入
void parase_python_struct(){
    Py_Initialize();
    init_numpy();

    PyObject* pModule=NULL;
    PyObject* pFunct=NULL;
    PyObject* pFunRet=NULL;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    pModule=PyImport_ImportModule("Test001");
    pFunct=PyObject_GetAttrString(pModule,"test_struct");

    pFunRet=PyObject_CallObject(pFunct,nullptr);
 

    PyArrayObject* a;
    char* b;
    PyArrayObject* c;
    char* pRetstruct;
    
    // PyArg_ParseTuple(pFunRet,"isi",&a,&b,&c);
    // cout<<"从python传入的：\n"<<a<<b<<c<<endl;
    // PyArrayObject* retarray;
    // PyArray_OutputConverter(pFunRet,&retarray);
    // cout<<PyArray_SHAPE(retarray);
    PyArg_ParseTuple(pFunRet,"OsO*",&a,&b,&c);

    cout<<b<<endl;

    int index_i=0,index_k=0,index_m=0,index_n=0;

    //0:row 1:col 2:thsd
    int a_rows=PyArray_DIM(a,0);
    int a_cols=PyArray_DIM(a,1);
    void* data= PyArray_DATA(a);
    int stride0=PyArray_STRIDE(a,0);
    int stride1=PyArray_STRIDE(a,1);
    *(double*)(data+0)=2;
    cout<<"a rows:"<<a_rows<<"cols:"<<a_cols<<endl;

    int c_rows=PyArray_DIM(c,0);
    int c_cols=PyArray_DIM(c,1);
    int c_thds=PyArray_DIM(c,2);
    int c_fous=PyArray_DIM(c,3);

    


    cout<<"c rows:"<<c_rows<<"cols:"<<c_cols<<"thds:"<<c_thds<<"fords"<<c_fous<<endl;

}



int main(){

    
    Py_Initialize();
    hello();
    parase_python_struct();
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
    
    PyObject* pFuncTwo=PyDict_GetItemString(pDict,"get_cpp_matrix");
    double arr[3]={1,2,3};
    PyObject* Arg1=Py_BuildValue("O",arr);
    PyObject_CallObject(pFuncTwo,Arg1);

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
                    *(double*)(data_p+0)=2;
                    cout<<*(double*)(data_p+index_m*stride1+index_n*stride2)<<" ";

                }
                cout<<endl;
            }
            //release
            
            Py_DECREF(listItem);
            
            
        }

       //PyRun_SimpleString("print()"); 
    }
    else{
        cout<<"not a list"<<endl;
    }
    //release
    Py_DECREF(pModule);
    Py_DECREF(pFuncOne);
    Py_DECREF(pDict);
    Py_DECREF(FuncOneRet);


    //执行下一步可能导致segment dumped
    //Py_Finalize();

    return 0;
}