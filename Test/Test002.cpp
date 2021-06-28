

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include<iostream>
#include<Python.h>
//不能使用尖括号
#include"numpy/arrayobject.h"
#include<time.h>
#include"Eigen/Dense"
#include<math.h>
#include<malloc.h>


using namespace std;
//解析python传过来的元组数据

bool init_numpy(){
    
    import_array();
    //return 0;
}

void parase_python_data(){
    Py_Initialize();
    init_numpy();

    PyObject* pModule=NULL;
    PyObject* pFunct=NULL;
    PyObject* pFunRet=NULL;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    pModule=PyImport_ImportModule("Test002");
    pFunct=PyObject_GetAttrString(pModule,"test_data");
    //pFunct=PyObject_GetAttrString(pModule,"load_para");

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
    //第二个为传入类型，O代表PyObject类型
    PyArg_ParseTuple(pFunRet,"OsO",&a,&b,&c);

    cout<<b<<endl;

    int index_i=0,index_k=0,index_m=0,index_n=0;

    //0:row 1:col 2:thsd
    int a_rows=PyArray_DIM(a,0);
    int a_cols=PyArray_DIM(a,1);
    cout<<"a rows:"<<a_rows<<"cols:"<<a_cols<<endl;

    int c_rows=PyArray_DIM(c,0);
    int c_cols=PyArray_DIM(c,1);
    int c_thds=PyArray_DIM(c,2);
    int c_fous=PyArray_DIM(c,3);

    void* p_data=PyArray_DATA(a);
    int stride0=PyArray_STRIDE(a,0);
    int stride1=PyArray_STRIDE(a,1);

    cout<<"stride:\n"<<stride0<<"\n"<<stride1<<endl;

    for(int i=0;i<a_rows;i++){
        for(int j=0;j<a_cols;j++){
            
            cout<<*(double*)(p_data+i*stride0+j*stride1)<<" ";
        }
    }

    cout<<endl;

    cout<<"c rows:"<<c_rows<<"cols:"<<c_cols<<"thds:"<<c_thds<<"fords"<<c_fous<<endl;

}

void parse_python_viterbi_data_opration(){
    Py_Initialize();
    init_numpy();

    PyObject* pModule=NULL;
    PyObject* pFunct=NULL;
    PyObject* pFunRet=NULL;

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    //导入python模块
    pModule=PyImport_ImportModule("Test002");
    //使用python函数
    pFunct=PyObject_GetAttrString(pModule,"parse_python_viterbi_data_opration");
    pFunRet=PyObject_CallObject(pFunct,nullptr);
 

    PyArrayObject* perm_matrix;
    PyArrayObject* alignment_matrix;
    PyArrayObject* traceback_matrix_rows;
    PyArrayObject* traceback_matrix_cols;
    PyArrayObject* num_candidate_matches;
    PyArrayObject* candidate_matches;
    PyArrayObject* p_bad;
    PyArrayObject* p_good;
    PyArrayObject* p_vals;
    int max_sequential_bad;
    
    
    //第二个为传入类型，O代表PyObject类型
    PyArg_ParseTuple(pFunRet,"OOOOOOOOOi",&perm_matrix,&alignment_matrix,&traceback_matrix_rows,&traceback_matrix_cols,
                    &num_candidate_matches,&candidate_matches,&p_bad,&p_good,&p_vals,&max_sequential_bad);


    int index_i=0,index_k=0,index_m=0,index_n=0;

    //0:row 1:col 2:thsd
    //perm_matrix 为二维矩阵
    int perm_rows=PyArray_DIM(perm_matrix,0);
    int perm_cols=PyArray_DIM(perm_matrix,1);
    void* perm_data=PyArray_DATA(perm_matrix);
    int perm_stride0=PyArray_STRIDE(perm_matrix,0);
    int perm_stride1=PyArray_STRIDE(perm_matrix,1);
    cout<<"perm_matrix rows:\n"<<perm_rows<<"\t"<<"cols:"<<perm_cols<<endl;
    cout<<"perm_matrix stride:\n"<<perm_stride0<<"\t"<<perm_stride1<<endl;

    //alignment_matrix为二c维矩阵
    int alignment_rows=PyArray_DIM(alignment_matrix,0);
    int alignment_cols=PyArray_DIM(alignment_matrix,1);
    void* alignment_data=PyArray_DATA(alignment_matrix);
    int alignment_stride0=PyArray_STRIDE(alignment_matrix,0);
    int alignment_stride1=PyArray_STRIDE(alignment_matrix,1);
    cout<<"alignment_matrix rows:\n"<<alignment_rows<<"\t"<<"cols:"<<alignment_cols<<endl;
    cout<<"alignment_matrix stride:\n"<<alignment_stride0<<"\t"<<alignment_stride1<<endl;

    //traceback_matrix_rows为二维向量
    int traceback_mr_rows=PyArray_DIM(traceback_matrix_rows,0);
    int traceback_mr_cols=PyArray_DIM(traceback_matrix_rows,1);
    void* traceback_mr_data=PyArray_DATA(traceback_matrix_rows);
    int traceback_mr_stride0=PyArray_STRIDE(traceback_matrix_rows,0);
    int traceback_mr_stride1=PyArray_STRIDE(traceback_matrix_rows,1);
    cout<<"traceback_matrix_rows rows:\n"<<traceback_mr_rows<<"\t"<<"cols:"<<traceback_mr_cols<<endl;
    cout<<"traceback_matrix_rows stride:\n"<<traceback_mr_stride0<<"\t"<<traceback_mr_stride1<<endl;

    //traceback_matrix_cols为二维向量
    int traceback_mc_rows=PyArray_DIM(traceback_matrix_cols,0);
    int traceback_mc_cols=PyArray_DIM(traceback_matrix_cols,1);
    void* traceback_mc_data=PyArray_DATA(traceback_matrix_cols);
    int traceback_mc_stride0=PyArray_STRIDE(traceback_matrix_cols,0);
    int traceback_mc_stride1=PyArray_STRIDE(traceback_matrix_cols,1);
    cout<<"traceback_matrix_cols rows:\n"<<traceback_mc_rows<<"\t"<<"cols:"<<traceback_mc_cols<<endl;
    cout<<"traceback_matrix_cols stride:\n"<<traceback_mr_stride0<<"\t"<<traceback_mc_stride1<<endl;

    //num_candidate_matches为一维向量
    int num_candidate_matches_cols=PyArray_DIM(num_candidate_matches,0);
    void* num_candidate_matches_data=PyArray_DATA(num_candidate_matches);
    int num_candidate_matches_stride0=PyArray_STRIDE(num_candidate_matches,0);
    cout<<"num_candidate_matches cols:\n"<<num_candidate_matches_cols<<endl;
    cout<<"num_candidate_matches stride:\n"<<num_candidate_matches_stride0<<endl;
   

    //candidate_matches为二维向量 数据类型为BOOL
    int candidate_matches_rows=PyArray_DIM(candidate_matches,0);
    int candidate_matches_cols=PyArray_DIM(candidate_matches,1);
    void* candidate_matches_data=PyArray_DATA(candidate_matches);
    int candidate_matches_stride0=PyArray_STRIDE(candidate_matches,0);
    int candidate_matches_stride1=PyArray_STRIDE(candidate_matches,1);
    cout<<"candidate_matches rows:\n"<<candidate_matches_rows<<"\t"<<"cols:"<<candidate_matches_cols<<endl;
    cout<<"candidate_matches stride:\n"<<candidate_matches_stride0<<"\t"<<candidate_matches_stride1<<endl;

    //p_bad为一维向量
    int p_bad_cols=PyArray_DIM(p_bad,0);
    void* p_bad_data=PyArray_DATA(p_bad);
    int p_bad_stride0=PyArray_STRIDE(p_bad,0);
    cout<<"p_bad cols:\n"<<p_bad_cols<<endl;
    cout<<"p_bad stride:\n"<<p_bad_stride0<<endl;
    
    //p_good为一维向量
    int p_good_cols=PyArray_DIM(p_good,0);
    void* p_good_data=PyArray_DATA(p_good);
    int p_good_stride0=PyArray_STRIDE(p_good,0);
    cout<<"p_good rows:\n"<<p_good_cols<<endl;
    cout<<"p_good stride:\n"<<p_good_stride0<<endl;
    
    //p_vals为三维向量
    int p_vals_rows=PyArray_DIM(p_vals,0);
    int p_vals_cols=PyArray_DIM(p_vals,1);
    int p_vals_thrds=PyArray_DIM(p_vals,2);
    void* p_vals_data=PyArray_DATA(p_vals);
    int p_vals_stride0=PyArray_STRIDE(p_vals,0);
    int p_vals_stride1=PyArray_STRIDE(p_vals,1);
    int p_vals_stride2=PyArray_STRIDE(p_vals,2);
    cout<<"p_vals rows:\n"<<p_vals_rows<<"\t"<<"cols:"<<p_vals_cols<<"\t"<<"thrds:"<<p_vals_thrds<<endl;
    cout<<"p_vals stride:\n"<<p_vals_stride0<<"\t"<<p_vals_stride1<<"\t"<<p_vals_stride2<<endl;

    cout<<"max_sequenctial_bad:\n"<<max_sequential_bad<<endl;
    
    cout<<"perm data:\t"<<*(int*)(perm_data+0)<<endl;;
    cout<<"alignment data"<<"\t"<<*(int*)(alignment_data+0)<<endl;
    cout<<"traceback matrix row data"<<"\t"<<*(int*)(traceback_mr_data+0)<<endl;
    cout<<"traceback matrix column data"<<"\t"<<*(int*)(traceback_mc_data+0)<<endl;
    cout<<"num candidate matches data"<<"\t"<<*(int*)(num_candidate_matches_data+0)<<endl;
    cout<<"candidate matches data"<<"\t"<<*(bool*)(candidate_matches_data+0)<<endl;
    cout<<"p_bad data"<<"\t"<<*(double*)(p_bad_data+0)<<endl;
    cout<<"p_good data"<<"\t"<<*(double*)(p_good_data+0)<<endl;
    cout<<"p_vals data"<<"\t"<<*(double*)(p_vals_data+0)<<endl;
    //打印内容
    // for(int i=0;i<a_rows;i++){
    //     for(int j=0;j<a_cols;j++){
            
    //         cout<<*(double*)(p_data+i*stride0+j*stride1)<<" ";
    //     }
    // }

    // cout<<endl;

    // cout<<"c rows:"<<c_rows<<"cols:"<<c_cols<<"thds:"<<c_thds<<"fords"<<c_fous<<endl;
    time_t start,end;
    
    time(&start);
    //下面完成viterbi主要算法
    for(int cl=1;cl<alignment_rows;cl++){

        double* this_row_best_scores=new double[*(int*)(num_candidate_matches_data+cl*num_candidate_matches_stride0)];
        double* this_row_best_row_from=new double[*(int*)(num_candidate_matches_data+cl*num_candidate_matches_stride0)];
        double* this_row_best_col_from=new double[*(int*)(num_candidate_matches_data+cl*num_candidate_matches_stride0)];
        for(int index=0;index<*(int*)(num_candidate_matches_data+cl*num_candidate_matches_stride0);index++){
            this_row_best_scores[index]=-1*INFINITY;
            this_row_best_col_from[index]=0;
            this_row_best_row_from[index]=0;
        }

        for(int cfrom=0;cfrom<min(max_sequential_bad,cl-1);cfrom++){
            //获取candidate中cfrom行
            bool* tmp1=new bool[candidate_matches_cols];
            bool* tmp2=new bool[candidate_matches_cols];
            int tmp1_sum=0,tmp2_sum=0;
            for (int i = 0; i < candidate_matches_cols; i++)
            {
                /* code */
                tmp1[i]=*(bool*)(candidate_matches_data+(cl-cfrom-1)*candidate_matches_stride0+i*candidate_matches_stride1);
                tmp2[i]=*(bool*)(candidate_matches_data+(cl)*candidate_matches_stride0+i*candidate_matches_stride1);
                if(tmp1[i]==true)tmp1_sum++;
                if(tmp2[i]==true)tmp2_sum++;
            }
            int total_num;
            //搜索perm_matrix中tmp1和tmp2行和列均为1的个数
            //创建二维数组tran_perm并给相应值
            //以下tran_perms，p_vals_copy，transition_matrix均可以合并至一个循环结构中
            int** tran_perms;
            tran_perms=new int*[tmp1_sum];
            for (int i = 0; i < tmp1_sum; i++)
            {
                /* code */
                tran_perms[i]=new int[tmp2_sum];
                for (int j = 0; j < tmp2_sum; j++)
                {
                    /* code */
                    //tran_perms = perms_matrix[tmp1,:][:,tmp2]
                    tran_perms[i][j]=*(int*)(perm_data+tmp1[i]*perm_stride0+tmp2[j]*perm_stride1);
                    
                }
                
            }
            cout<<"tran_perms:"<<tran_perms[0][0]<<endl;
            //
            int row_n=*(int*)(num_candidate_matches_data+(cl-cfrom-1)*num_candidate_matches_stride0);
            int col_n=*(int*)(num_candidate_matches_data+(cl)*num_candidate_matches_stride0);
            
            double** p_vals_copy;
            p_vals_copy=new double*[tmp1_sum];
            for (int i = 0; i < tmp1_sum; i++)
            {
                /* code */
                p_vals_copy[i]=new double[tmp2_sum];
                for (int j = 0; j < tmp2_sum; j++)
                {
                    /* code */
                    double sum_p_bad=0;
                    for (int k = cl-cfrom; k <cl-1+1 ; k++)
                    {
                        /* code */
                        sum_p_bad+=*(double*)(p_bad_data+k*p_bad_stride0);
                    }
                    //p_bad_copy赋值
                    p_vals_copy[i][j]=*(double*)(p_vals_data+i*p_vals_stride0+j*p_vals_stride1+((int)(tran_perms[i][j]))*p_vals_stride2)+sum_p_bad;
                    
                }
                
            }
            cout<<"p_vals_copy:"<<p_vals_copy[0][0]<<endl;
            //transition_sub_matrix=p_vals_copy.reshape(row_n,coln)
            double** transition_matrix;
            transition_matrix=new double*[tmp1_sum];
            for (int i = 0; i < tmp1_sum; i++)
            {
                /* code */
                transition_matrix[i]=new double[tmp2_sum];
                for (int j = 0; j < tmp2_sum; j++)
                {
                    /* code */
                    transition_matrix[i][j]=p_vals_copy[i][j];
                    transition_matrix[i][j]=transition_matrix[i][j]+*(double*)(p_good_data+(cl-cfrom-1)*p_good_stride0);
                }
                
            }
            cout<<"transition_matrix:"<<transition_matrix[0][0]<<endl;
            //score
            //candidate_matches[cl-cfrom-1,:]
            double* tmp=new double[tmp1_sum];
            int index=0;
            for (int k = 0; k < candidate_matches_cols; k++)
            {
                /* code */
                if(true==*(bool*)(candidate_matches_data+(cl-cfrom-1)*candidate_matches_stride0+k*candidate_matches_stride1)){
                    tmp[index++]=*(double*)(alignment_data+(cl-cfrom-1)*alignment_stride0+k*alignment_stride1);
                }
            }
            cout<<index<<endl;
            cout<<"tmp:"<<tmp[0]<<endl;
            cout<<"tmp1_sum:"<<tmp1_sum<<endl;
            double** score;
            score=new double*[tmp1_sum];
            for (int i = 0; i < tmp1_sum; i++)
            {
                
                /* code */
                score[i]=new double[tmp2_sum];
                for (int j = 0; j < tmp2_sum; j++)
                {
                    /* code */
                
                    score[i][j]=transition_matrix[i][j]+tmp[i];
                }
                
            }
            cout<<"score:"<<score[0][0]<<endl;
            //from_score,where_fromm 
            //每列的最大值和最大值行索引
            double* from_score=new double[tmp2_sum];
            int* where_score=new int[tmp2_sum];
            bool* better_scores=new bool[tmp2_sum];
            for (int i = 0; i < tmp2_sum; i++)
            {
                /* code */
                double maxscore=-INFINITY;
                int maxindex=0; 
                for (int j = 0; j < tmp1_sum; j++)
                {
                    /* code */
                    //比较每一列tmp1_sum行寻找最大值和对应索引
                    if(score[j][i]>maxscore){
                        maxscore=score[j][i];
                        maxindex=i;
                    }
                    else{
                        continue;
                    }
                }
                from_score[i]=maxscore;
                where_score[i]=maxindex;
                if(from_score[i]>this_row_best_scores[i]){
                    better_scores[i]=true;
                }
            }
            cout<<"from score:"<<from_score[0]<<"where score:"<<where_score[0]<<"better score:"<<better_scores[0]<<endl;
            //from_indices
            int* from_indeces=new int[tmp1_sum];
            int index_f=0;
            for (int j = 0; j < candidate_matches_cols; j++)
            {
                /* code */
                if(*(bool*)(candidate_matches_data+(cl-cfrom-1)*candidate_matches_stride0+j*candidate_matches_stride1)==true){
                    from_indeces[index_f++]=j;
                }
            }
            cout<<"from indices:"<<from_indeces[0]<<endl;
            //this_row_best_col_from
            //this_row_best_row_from
            //thiw_row_best_scores
            for (int i = 0; i < tmp2_sum; i++)
            {
                /* code */
                if(better_scores[i]==true){
                    this_row_best_col_from[i]=from_indeces[where_score[i]];
                    this_row_best_row_from[i]=cl-cfrom-1;
                    this_row_best_scores[i]=from_score[i];
                }
                
            }
            
            
        }
        
        //alignment_matrix，traceback_matrix_row,traceback_matrix_col更新
        for (int i = 0; i < candidate_matches_cols; i++)
        {
            /* code */
            if(true==*(bool*)(candidate_matches_data+cl*candidate_matches_stride0+i*candidate_matches_stride1)){
                *(double*)(alignment_data+cl*alignment_stride0+i*alignment_stride1)+=this_row_best_scores[i];
                *(int*)(traceback_mr_data+cl*traceback_mr_stride0+i*traceback_mr_stride1)=this_row_best_row_from[i];
                *(int*)(traceback_mc_data+cl*traceback_mc_stride0+i*traceback_mc_stride1)=this_row_best_col_from[i];
            }  
        }
        cout<<"alignment data:"<<endl;
    }

    //返回至python
    //PyObject* topythonFun=PyObject_GetAttrString(pModule,"parse_cpp_return_data");
     
    cout<<"输送数据至Python"<<endl;
    time(&end);
    cout<<"计算总耗时："<<double(end-start)<<"s"<<endl;
}


int main(){
    parase_python_data();
    time_t start;
    time(&start);
    parse_python_viterbi_data_opration();
    time_t end;
    time(&end);
    
    cout<<(double)(end-start)<<"s"<<endl;


    return 0;
}