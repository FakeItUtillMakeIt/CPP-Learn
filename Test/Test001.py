from numpy.core.defchararray import array
from numpy.core.arrayprint import dtype_is_implied
import struct
import numpy as np

def hello(s):
    print("i run in python")
    print(s)
    print('----------------')
    a=np.array([[1,2,3],[4,5,6]])
    return a

def test_ret():
    a=np.array([[1,2,3],[4,5,6]])
    alist=[[1,2,3],[3,2,1]]
    blist=[[4,5,6],[6,5,4]]
    alist=np.asarray(alist,dtype='float')
    blist=np.asarray(blist,dtype='float')
    return [alist,blist]