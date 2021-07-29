from numpy.core.defchararray import array
from numpy.core.arrayprint import dtype_is_implied
import struct
import numpy as np




test_string="wolaiceshiyixia"

def hello(s):
    print("i run in python")
    print(s)
    print('----------------')
    a=np.array([[1,2,3],[4,5,6]])
    return a

def get_cpp_matrix(matrix):
    print(matrix)
    matrix=np.asarray(matrix,dtype="double")
    print(matrix)

def test_ret():
    print(test_string)
    a=np.array([[1,2,3],[4,5,6]])
    alist=[[1,2,3],[3,2,1]]
    blist=[[4,5,6],[6,5,4]]
    alist=np.asarray(alist,dtype='float')
    blist=np.asarray(blist,dtype='float')
    return [alist,blist]

def test_struct():
    a=np.asarray([[1,2,3],[3,2,1]],dtype="double")
    """
    1,2
    2,1
    2,0
    0,2

    1,1
    1,1
    """
    c=np.array([[[[2,3],[1,3]],[[1,1],[1,1]],[[2,3],[2,1]]],[[[2,3],[1,3]],[[1,1],[1,1]],[[2,3],[2,1]]]])
    #s=struct.pack("i11si",a,"hello",c)
    return a,"hello,c",c

