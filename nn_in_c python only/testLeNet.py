#######################################################################################
#######       DO NOT MODIFY, DEFINITELY READ THROUGH ALL OF THE CODE            #######
#######################################################################################

import numpy as np
import cnn_lenet
import pickle
import copy
import random
from matplotlib.mlab import PCA
import time, datetime

def get_lenet():
  """Define LeNet

  Explanation of parameters:
  type: layer type, supports convolution, pooling, relu
  channel: input channel
  num: output channel
  k: convolution kernel width (== height)
  group: split input channel into several groups, not used in this assignment
  """

  layers = {}
  layers[1] = {}
  layers[1]['type'] = 'DATA'
  layers[1]['height'] = 28
  layers[1]['width'] = 28
  layers[1]['channel'] = 1
  layers[1]['batch_size'] = 64

  layers[2] = {}
  layers[2]['type'] = 'CONV'
  layers[2]['num'] = 20
  layers[2]['k'] = 5
  layers[2]['stride'] = 1
  layers[2]['pad'] = 0
  layers[2]['group'] = 1

  layers[3] = {}
  layers[3]['type'] = 'POOLING'
  layers[3]['k'] = 2
  layers[3]['stride'] = 2
  layers[3]['pad'] = 0

  layers[4] = {}
  layers[4]['type'] = 'CONV'
  layers[4]['num'] = 50
  layers[4]['k'] = 5
  layers[4]['stride'] = 1
  layers[4]['pad'] = 0
  layers[4]['group'] = 1

  layers[5] = {}
  layers[5]['type'] = 'POOLING'
  layers[5]['k'] = 2
  layers[5]['stride'] = 2
  layers[5]['pad'] = 0

  layers[6] = {}
  layers[6]['type'] = 'IP'
  layers[6]['num'] = 500
  layers[6]['init_type'] = 'uniform'

  layers[7] = {}
  layers[7]['type'] = 'RELU'

  layers[8] = {}
  layers[8]['type'] = 'LOSS'
  layers[8]['num'] = 10
  return layers

def get_500result(xtest):
  # define lenet
  layers = get_lenet()

  # load data
  # change the following value to true to load the entire dataset
  # fullset = False
  # print("Loading MNIST Dataset...")
  # xtrain, ytrain, xval, yval, xtest, ytest = cnn_lenet.load_mnist(fullset)
  # print("MNIST Dataset Loading Complete!\n")

  # xtrain = np.hstack([xtrain, xval])
  # ytrain = np.hstack([ytrain, yval])
  # m_train = xtrain.shape[1]
  # xtest = xtest[:]
  # ytest = 


  # cnn parameters
  batch_size = 64
  mu = 0.9
  epsilon = 0.01
  gamma = 0.0001
  power = 0.75
  weight_decay = 0.0005
  w_lr = 1
  b_lr = 2

  # test_interval = 100
  # display_interval = 100
  # snapshot = 5000
  # max_iter = 10000

  # initialize parameters
  print("Initializing Parameters...")
  # params = pickle.load(open('lenet10000.mat', 'rb'))
  path = "/Users/bernieyang/Git/retinet/nn_in_c python only/lenet10000.mat"
  with open(path, 'rb') as file:
    params = pickle.load(file, encoding='latin1')
  param_winc = copy.deepcopy(params)

  for l_idx in range(1, len(layers)):
    param_winc[l_idx]['w'] = np.zeros(param_winc[l_idx]['w'].shape)
    param_winc[l_idx]['b'] = np.zeros(param_winc[l_idx]['b'].shape)
  # learning iterations
  # random.seed(100000)
  # indices = range(m_train)
  # random.shuffle(indices)

  # print("Training Started. Printing report on training data every " + str(display_interval) + " steps.")
  # print("Printing report on test data every " + str(test_interval) + " steps.\n")
  layers[1]['batch_size'] = xtest.shape[1]
  # cptest, _ = cnn_lenet.conv_net(params, layers, xtest, ytest, False)
  result = cnn_lenet.conv_net_500(params, layers, xtest)
  # print(len(result))
  return result
  


def get_result(xtest):
  # define lenet
  layers = get_lenet()

  # load data
  # change the following value to true to load the entire dataset
  # fullset = False
  # print("Loading MNIST Dataset...")
  # xtrain, ytrain, xval, yval, xtest, ytest = cnn_lenet.load_mnist(fullset)
  # print("MNIST Dataset Loading Complete!\n")

  # xtrain = np.hstack([xtrain, xval])
  # ytrain = np.hstack([ytrain, yval])
  # m_train = xtrain.shape[1]
  # xtest = xtest[:]
  # ytest = 


  # cnn parameters
  batch_size = 64
  mu = 0.9
  epsilon = 0.01
  gamma = 0.0001
  power = 0.75
  weight_decay = 0.0005
  w_lr = 1
  b_lr = 2

  # test_interval = 100
  # display_interval = 100
  # snapshot = 5000
  # max_iter = 10000

  # initialize parameters
  print("Initializing Parameters...")
  # params = pickle.load(open('lenet10000.mat', 'rb'))

  with open('lenet10000.mat', 'rb') as file:
    params = pickle.load(file, encoding='latin1')
  param_winc = copy.deepcopy(params)

  for l_idx in range(1, len(layers)):
    param_winc[l_idx]['w'] = np.zeros(param_winc[l_idx]['w'].shape)
    param_winc[l_idx]['b'] = np.zeros(param_winc[l_idx]['b'].shape)
  # learning iterations
  # random.seed(100000)
  # indices = range(m_train)
  # random.shuffle(indices)

  # print("Training Started. Printing report on training data every " + str(display_interval) + " steps.")
  # print("Printing report on test data every " + str(test_interval) + " steps.\n")
  layers[1]['batch_size'] = xtest.shape[1]
  # cptest, _ = cnn_lenet.conv_net(params, layers, xtest, ytest, False)
  result = cnn_lenet.conv_net_predict(params, layers, xtest)
  return result
  


