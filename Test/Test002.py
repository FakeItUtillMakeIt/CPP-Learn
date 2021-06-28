
from logging import PercentStyle
import numpy as np 
import os 
import json,time
from collections import defaultdict
from scipy.io import loadmat,savemat
import h5py 
import basic
import struct




def load_para():
    read = loadmat('/mnt/raid/lla/nanopore/result/test/cen/section12/2021_05_17_0005.abf_630.56_689.28_9.mat',simplify_cells=True)
    m=0.5923228
    b=-0.136433
    p_step = 0.93 #0.93
    p_skip = 0.07 #0.07
    p_ext = 0.2
    p_bad = 0.05 #0.01
    scorecutoff = -3 #-3
    p=[p_step]+[p_skip*np.power(p_ext,i) for i in range(11)]
    p = np.array(p)
    if m == None or b == None:
        if refseq == None:
            refseq = basic.random_seq()
        caldata,e,k = basic.levelPredPipelineDC(refseq,n=n)
        m,b=basic.calibrate_by_iqr(read['read']['x_tf'][0],caldata)
        print(m,b)
        read['read']['calibration_scale'] = m 
        read['read']['calibration_offset'] = b 
    else:
        read['read']['calibration_scale'] = m 
        read['read']['calibration_offset'] = b
    read['read']['startlevel'] = 0
    read['read']['endlevel'] = len(read['read']['x_tf'][0])
    x = read['read']['x_tf'][0][(read['read']['startlevel']):(read['read']['endlevel'])]*m + b
    K = read['read']['k_tf'][(read['read']['startlevel']):(read['read']['endlevel'])]
    K = np.array([1/np.linalg.inv(i)[0,0] for i in K])
    #x,K = x[100:170],K[100:170]
    P=np.array([np.tile(i,(len(x),4)) for i in p])
    P = P.transpose(1,2,0)
    Pbad = p_bad*np.ones(len(x))
    p = P.copy()
    p_bad = Pbad.copy()
    score_cutoff = -1
    start = time.time()
    pore_model_6mer_constant_voltage = h5py.File('/mnt/raid/lla/hectobio/variable-voltage-sequencing-master/pore_model_6mer_constant_voltage.mat','r')
    map_  = pore_model_6mer_constant_voltage['map']
    transition_info = h5py.File('/mnt/raid/lla/hectobio/variable-voltage-sequencing-master/transition_info_hel308_6mer.mat','r')
    transition_info = transition_info['transition_info']
    p_combos=transition_info['p_combos'][()].T
    baseascii = map_['name'][()].T

    p_ind_given_backstep = 0.975
    p_backstep = 0.025
    verbose = False
    did_backstep = np.zeros((len(x), 1))
    prior_given_backstep = 0.5 * np.ones((1, len(map_['mean'][()])))
    prior_given_nobackstep = prior_given_backstep
    n = p_combos.shape[1]
    max_sequential_bad = p.shape[1] - 1
    p_vals = p.copy() #(85,4,12)
    for cB in range(p_vals.shape[1]):
        total_probability = np.sum(p_vals[:, cB, :], 1)
        total_probability = np.tile(total_probability,(12,1,1)).transpose(2,1,0)
        p_vals[:, cB, :] = (p_vals[:, cB, :] / total_probability)[0]
    #transition_info['p_combos'][()].T，从matlab转过来需要转置

    p_combos = p_combos.reshape(p_combos.shape[0],p_combos.shape[1],1,1)
    p_vals = np.log(np.sum(p_vals.transpose(2,1,0)*p_combos, 1))
    p_vals = p_vals.transpose(2,1,0)
    #print(p_vals.shape)
    #find the penalty to be applied for each bad level.
    p_good = np.log(1 - p_bad)
    p_bad = np.log(p_bad)
    x2=map_['mean'][()].flatten()#从h5py读的是一个嵌套列表，每个值需要单独再取，所以用了[0]
    map_stiffness = map_['stiffness'][()].T[0]
    K2=np.array([pore_model_6mer_constant_voltage[i][()][0][0] for i in map_stiffness])
    #start = time.time()
    alignment_matrix = basic.smatrixACpar(x, K, x2, K2)+prior_given_backstep*(did_backstep) + prior_given_nobackstep
    start = time.time()

    candidate_matches = alignment_matrix > (np.max(alignment_matrix,1)+score_cutoff).reshape(-1,1)
    num_candidate_matches = np.sum(candidate_matches,1)
    alignment_matrix[~candidate_matches] = -1*np.inf


    traceback_matrix_rows = np.zeros(alignment_matrix.shape,dtype='int64')
    traceback_matrix_cols = np.zeros(alignment_matrix.shape,dtype='int64')
    perms_matrix=transition_info['perms_matrix'][()].T.astype(int)
    step_size_matrix=transition_info['step_size_matrix'][()].T.astype(int)
    end = time.time()
    print('loading necessary information cost {} s'.format(end-start))

    # print(perms_matrix.shape,
    # alignment_matrix.shape,
    # traceback_matrix_rows.shape,
    # traceback_matrix_cols.shape,
    # num_candidate_matches.shape,
    # candidate_matches.shape,max_sequential_bad)

    input_cpp_struct=struct.pack(perms_matrix,alignment_matrix,traceback_matrix_rows,traceback_matrix_cols,num_candidate_matches,candidate_matches,p_bad,p_good,p_vals,max_sequential_bad)
    return input_cpp_struct

def parse_python_viterbi_data_opration():
    read = loadmat('/mnt/raid/lla/nanopore/result/test/cen/section12/2021_05_17_0005.abf_630.56_689.28_9.mat',simplify_cells=True)
    m=0.5923228
    b=-0.136433
    p_step = 0.93 #0.93
    p_skip = 0.07 #0.07
    p_ext = 0.2
    p_bad = 0.05 #0.01
    scorecutoff = -3 #-3
    p=[p_step]+[p_skip*np.power(p_ext,i) for i in range(11)]
    p = np.array(p)
    if m == None or b == None:
        if refseq == None:
            refseq = basic.random_seq()
        caldata,e,k = basic.levelPredPipelineDC(refseq,n=n)
        m,b=basic.calibrate_by_iqr(read['read']['x_tf'][0],caldata)
        print(m,b)
        read['read']['calibration_scale'] = m 
        read['read']['calibration_offset'] = b 
    else:
        read['read']['calibration_scale'] = m 
        read['read']['calibration_offset'] = b
    read['read']['startlevel'] = 0
    read['read']['endlevel'] = len(read['read']['x_tf'][0])
    x = read['read']['x_tf'][0][(read['read']['startlevel']):(read['read']['endlevel'])]*m + b
    K = read['read']['k_tf'][(read['read']['startlevel']):(read['read']['endlevel'])]
    K = np.array([1/np.linalg.inv(i)[0,0] for i in K])
    #x,K = x[100:170],K[100:170]
    P=np.array([np.tile(i,(len(x),4)) for i in p])
    P = P.transpose(1,2,0)
    Pbad = p_bad*np.ones(len(x))
    p = P.copy()
    p_bad = Pbad.copy()
    score_cutoff = -1
    start = time.time()
    pore_model_6mer_constant_voltage = h5py.File('/mnt/raid/lla/hectobio/variable-voltage-sequencing-master/pore_model_6mer_constant_voltage.mat','r')
    map_  = pore_model_6mer_constant_voltage['map']
    transition_info = h5py.File('/mnt/raid/lla/hectobio/variable-voltage-sequencing-master/transition_info_hel308_6mer.mat','r')
    transition_info = transition_info['transition_info']
    p_combos=transition_info['p_combos'][()].T
    baseascii = map_['name'][()].T

    p_ind_given_backstep = 0.975
    p_backstep = 0.025
    verbose = False
    did_backstep = np.zeros((len(x), 1))
    prior_given_backstep = 0.5 * np.ones((1, len(map_['mean'][()])))
    prior_given_nobackstep = prior_given_backstep
    n = p_combos.shape[1]
    max_sequential_bad = p.shape[1] - 1
    p_vals = p.copy() #(85,4,12)
    for cB in range(p_vals.shape[1]):
        total_probability = np.sum(p_vals[:, cB, :], 1)
        total_probability = np.tile(total_probability,(12,1,1)).transpose(2,1,0)
        p_vals[:, cB, :] = (p_vals[:, cB, :] / total_probability)[0]
    #transition_info['p_combos'][()].T，从matlab转过来需要转置

    p_combos = p_combos.reshape(p_combos.shape[0],p_combos.shape[1],1,1)
    p_vals = np.log(np.sum(p_vals.transpose(2,1,0)*p_combos, 1))
    p_vals = p_vals.transpose(2,1,0)
    #print(p_vals.shape)
    #find the penalty to be applied for each bad level.
    p_good = np.log(1 - p_bad)
    p_bad = np.log(p_bad)
    x2=map_['mean'][()].flatten()#从h5py读的是一个嵌套列表，每个值需要单独再取，所以用了[0]
    map_stiffness = map_['stiffness'][()].T[0]
    K2=np.array([pore_model_6mer_constant_voltage[i][()][0][0] for i in map_stiffness])
    #start = time.time()
    alignment_matrix = basic.smatrixACpar(x, K, x2, K2)+prior_given_backstep*(did_backstep) + prior_given_nobackstep
    start = time.time()

    candidate_matches = alignment_matrix > (np.max(alignment_matrix,1)+score_cutoff).reshape(-1,1)
    num_candidate_matches = np.sum(candidate_matches,1)
    alignment_matrix[~candidate_matches] = -1*np.inf


    traceback_matrix_rows = np.zeros(alignment_matrix.shape,dtype='int64')
    traceback_matrix_cols = np.zeros(alignment_matrix.shape,dtype='int64')
    perms_matrix=transition_info['perms_matrix'][()].T.astype(int)
    step_size_matrix=transition_info['step_size_matrix'][()].T.astype(int)
    end = time.time()
    print('loading necessary information cost {} s'.format(end-start))

    # print(perms_matrix.shape,
    # alignment_matrix.shape,
    # traceback_matrix_rows.shape,
    # traceback_matrix_cols.shape,
    # num_candidate_matches.shape,
    # candidate_matches.shape,max_sequential_bad)

    #input_cpp_struct=struct.pack(perms_matrix,alignment_matrix,traceback_matrix_rows,traceback_matrix_cols,num_candidate_matches,candidate_matches,p_bad,p_good,p_vals,max_sequential_bad)
    return np.asarray(perms_matrix,dtype=("int")),np.asarray(alignment_matrix,dtype="double"),np.asarray(traceback_matrix_rows,dtype="int"),\
                np.asarray(traceback_matrix_cols,dtype="int"),np.asarray(num_candidate_matches,dtype="int"),np.asarray(candidate_matches,dtype="bool"),np.asarray(p_bad,dtype="double"),np.asarray(p_good,dtype=("double")),np.asarray(p_vals,dtype="double"),max_sequential_bad

def parse_cpp_return_data(alignment_matrix,traceback_matrix_rows,traceback_matrix_cols):
    print("alignment_matrix shape is:",alignment_matrix.shape)
    print("traceback_matrix_rows shape is:",traceback_matrix_rows.shape)
    print("traceback_matrix_cols shape is:",traceback_matrix_cols.shape)

    pass


def viterbi(perms_matrix,alignment_matrix,traceback_matrix_rows,traceback_matrix_cols,num_candidate_matches,candidate_matches,p_bad,p_good,p_vals,max_sequential_bad):
    for cl in range(1,alignment_matrix.shape[0]):
        #print('cl',cl)
        this_row_best_scores = np.array([-1*np.inf]*num_candidate_matches[cl])
        this_row_best_row_from = np.zeros((num_candidate_matches[cl]))
        this_row_best_col_from = np.zeros((num_candidate_matches[cl]))

        for cfrom in range(min(max_sequential_bad, cl-1)+1):
            #print('cfrom',cfrom)
            tmp1=candidate_matches[cl-cfrom-1,:]
            print(sum(tmp1))
            tmp2=candidate_matches[cl,:]
            print(sum(tmp2))
            # tmp3=perms_matrix[tmp1,:]
            # tran_perms=tmp3[:,tmp2]
            tran_perms = perms_matrix[tmp1,:][:,tmp2]
            #tran_perms = perms_matrix[candidate_matches[cl-cfrom-1,:],:][:,candidate_matches[cl,:]]
            #tran_perms = tran_perms_nb(perms_matrix,candidate_matches[cl-cfrom-1,:],candidate_matches[cl,:])
            #tran_perms = perm.get_perms(perms_matrix,candidate_matches[cl-cfrom-1,:],candidate_matches[cl,:])
            row_n,col_n = num_candidate_matches[cl-cfrom-1],num_candidate_matches[cl]
            p_vals_copy = p_vals[cl,cfrom,tran_perms-1]+np.sum(p_bad[(cl - cfrom):cl-1+1]) #matlab中数组是左闭右闭
            transition_submatrix = p_vals_copy.reshape(row_n,col_n)
            transition_submatrix = transition_submatrix+ p_good[cl - cfrom - 1]
            score=transition_submatrix + alignment_matrix[cl-cfrom-1,candidate_matches[cl-cfrom-1,:]].reshape(-1,1)
            from_score, where_from = np.max(score, 0),np.argmax(score, 0)
            better_scores = from_score > this_row_best_scores
            from_indices = np.where(candidate_matches[cl-cfrom-1,:] == True)[0]
            """
            0:k1 k2 k3
            1:k4 k5 k6
            score: k1k4 k1k5 k1k6
                   k2k4 k2k5 k2k6
                   k3k4 k3k5 k3k6
            from score: k2k4,k3k5,k1k6
            where from:1,2,0
            from_indices:k1 k2 k3
            from_indices[where_from[better_scores]]最后返回最佳kmer，比如是0行的k2(1),k3(2)
            this_row_best_col_from：里面存储着当前每个kmer跟上面哪个kmer乘积最大，例如k4最佳，那么k2和k4乘积最大,保存的索引1
            """
            #update the best-one trackers with this information
            this_row_best_col_from[better_scores] = from_indices[where_from[better_scores]]
            this_row_best_row_from[better_scores] = cl - cfrom - 1
            this_row_best_scores[better_scores] = from_score[better_scores]
        alignment_matrix[cl,candidate_matches[cl,:]] = this_row_best_scores + alignment_matrix[cl,candidate_matches[cl,:]]
        traceback_matrix_rows[cl,candidate_matches[cl,:]] = this_row_best_row_from
        traceback_matrix_cols[cl,candidate_matches[cl,:]] = this_row_best_col_from
    return alignment_matrix,traceback_matrix_rows,traceback_matrix_cols


def test_data():
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



if __name__ == '__main__':
    start2 = time.time()
    #print(p_bad.shape,p_good.shape,candidate_matches.shape)
    para=load_para()
    alignment_matrix,traceback_matrix_rows,traceback_matrix_cols=viterbi(para)
    #a = np.take(perms_matrix.ravel(order='F'),a)
    #perm.HMM(x,K,P,Pbad,score_cutoff=-1)
    end2 = time.time()
    print(end2-start2)