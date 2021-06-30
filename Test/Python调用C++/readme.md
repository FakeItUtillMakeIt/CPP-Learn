#1.安装必要linux必要依赖
apt install swig

#2.构建python与C/C++对应数据类型与包含头文件，文件格式形如C语言格式
#具体文件见align.i所示

#3.构建linux可调用的so动态库 
依次运行命令
swig -c++/c -python *.i
g++ -fPIC -shared align_wrap.cxx/c(swig生成的和python交互的接口) align.cpp/c(自己写的供python调用的c接口) -o _align.so(你的.so文件名) -I /usr/include/python版本(安装的python目录)

#4.将so动态库文件导入linux环境变量，方便python导入库文件
使用VI编辑器打开~/目录下的.bashrc用户变量文件
并在~/.bashrc用户变量文件末尾添加 export LD_LABRARY_PATH=$LD_LABRARY_PATH:[so动态库文件路径]
然后更新用户环境变量 source ~/.bashrc 

#5.运行上述命令后将在选定的文件目录生成align.py文件，align.py文件中引入了align.so库，将align.py文件放入用户的执行目录中就可调用align.cpp中的函数了
import align




