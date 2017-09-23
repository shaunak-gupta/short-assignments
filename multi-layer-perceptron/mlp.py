from math import log
import math
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " FILENAME\n"
    quit()

def sigmoid_scalar(x):
    return 1/(1+math.exp(-1*x))

def sigmoid_gradient_scalar(x):
    return sigmoid_scalar(x) * (1-sigmoid_scalar(x))

def get_data_matrix(dataframe, feature_list, feature_map):
    matrix = np.zeros(shape=(len(dataframe), len(feature_list)))
    for index, feature in enumerate(feature_list):
        if (feature_map[feature][0] == 'numeric'):
            min_value = feature_map[feature][1]
            max_value = feature_map[feature][2]
            matrix[:,index] = dataframe[feature].apply(lambda x: float(x-min_value)/(max_value-min_value))
        else:
            dictionary = feature_map[feature][1]
            matrix[:,index] = dataframe[feature].apply(lambda x: dictionary[x])
    return matrix

def randInitializeWeights(l_in, l_out):
    epsilon = 0.12
    weights = np.random.rand(l_out, 1+l_in) * 2*epsilon - epsilon
    return weights

# computes the cost function value and its gradient given neural network parameters
def costFunction(theta1, theta2, input_layer_size, hidden_layer_size, num_labels, X, y):
    m = X.shape[0]  # number of training samples
    j = 0       # cost function value
    theta1_grad = np.zeros(shape=theta1.shape)
    theta2_grad = np.zeros(shape=theta2.shape)
    
    sigmoid = np.vectorize(sigmoid_scalar)
    sigmoid_gradient = np.vectorize(sigmoid_gradient_scalar)
    
    input_layer = np.ones(shape=(X.shape[0], X.shape[1]+1))
    input_layer[:,1:] = X
    
    hidden_layer = np.ones(shape=(input_layer.shape[0], theta1.shape[0]+1))
    hidden_layer[:,1:] = sigmoid(np.dot(input_layer, np.transpose(theta1)))
    
    output_layer = sigmoid(np.dot(hidden_layer, np.transpose(theta2)))
    
    for index, y_val in enumerate(y):
        y_vec = np.zeros(num_labels)
        y_vec[y_val] = 1
        log_v = np.vectorize(log)
        j -= (1.0/m) * (np.dot(log_v(output_layer[index,:]), y_vec) + np.dot(log_v(1 - output_layer[index,:]),(1-y_vec)))
        
    for t in range(0,m):
        a1 = input_layer[t,:]
        a2 = hidden_layer[t,:]
        a3 = output_layer[t,:]
        y_vec = np.zeros(num_labels)
        y_vec[y[t]] = 1
        
        delta3 = a3 - y_vec
        delta2 = np.dot(np.transpose(theta2), np.multiply(delta3, np.transpose(sigmoid_gradient(np.dot(a2, np.transpose(theta2))))))
        delta2 = delta2[1:]
        
        delta2 = delta2.reshape(delta2.shape[0],1)
        delta3 = delta3.reshape(delta3.shape[0],1)
        a1 = a1.reshape(1, a1.shape[0])
        a2 = a2.reshape(1, a2.shape[0])
        
        theta1_grad += np.dot(delta2,a1)
        theta2_grad += np.dot(delta3,a2)
    
    theta1_grad = (1.0/m)*theta1_grad
    theta2_grad = (1.0/m)*theta2_grad
    return j, theta1_grad, theta2_grad

def predict(theta1, theta2, X):
    m = X.shape[0]
    num_labels = theta2.shape[0]

    sigmoid = np.vectorize(sigmoid_scalar)
    input_layer = np.ones(shape=(X.shape[0], X.shape[1]+1))
    input_layer[:,1:] = X
    h1 = sigmoid(np.dot(input_layer,np.transpose(theta1)))

    hidden_layer = np.ones(shape=(h1.shape[0], h1.shape[1]+1))
    hidden_layer[:,1:] = h1
    h2 = sigmoid(np.dot(hidden_layer, np.transpose(theta2)))
    output = []
    for i in range(len(h2)):
        output.append(h2[i].argmax())
    return output

data = pd.read_csv(sys.argv[1])
train = data.sample(frac=0.7)
test = data.drop(train.index).reset_index(drop=True)
train = train.reset_index(drop=True)
target_attribute = data.columns[-1]

feature_list = list(data.columns)
feature_list.remove(target_attribute)

num_iterations = 1500
alpha = 0.01
cost_array = []

feature_map = {}
for feature in feature_list:
    if data[feature].dtype.kind in 'bif':
        min_value = data[feature].min()
        max_value = data[feature].max()
        feature_map[feature] = ['numeric', min_value, max_value]
    else:
        categories = data[feature].unique()
        quantum = 1.0/(len(categories)-1)
        value = 0
        category_map = {}
        for category in categories:
            category_map[category] = value
            value += quantum
        feature_map[feature] = ['categorical', category_map]

output_map = {}
value = 0
for category in data[target_attribute].unique():
    output_map[category] = value
    value += 1

output_vec = train[target_attribute].apply(lambda x: output_map[x])

input_matrix = get_data_matrix(train, feature_list, feature_map)
input_layer_size = len(feature_list)
hidden_layer_size = int(1.5*input_layer_size)
num_labels = len(output_map)

training_examples_count = len(train)

theta1 = randInitializeWeights(input_layer_size, hidden_layer_size)
theta2 = randInitializeWeights(hidden_layer_size, num_labels)

for i in range(num_iterations):
    cost, theta1_grad, theta2_grad = costFunction(theta1, theta2, input_layer_size, hidden_layer_size, num_labels, input_matrix, output_vec)
    cost_array.append(cost)
    theta1 = theta1 - alpha*theta1_grad
    theta2 = theta2 - alpha*theta2_grad

# prediction on validation set
test_output_vec = test[target_attribute].apply(lambda x: output_map[x])

test_input_matrix = get_data_matrix(test, feature_list, feature_map)

prediction = predict(theta1, theta2, test_input_matrix)

plt.plot(cost_array)
plt.show()

correct_count = {}
incorrect_count = {}
for val in data[target_attribute].unique():
    correct_count[val] = 0
    incorrect_count[val] = 0

for index, row in test.iterrows():
    if prediction[index] == test_output_vec[index]:
        correct_count[row[target_attribute]] += 1
    else:
        incorrect_count[row[target_attribute]] += 1

p1 = plt.bar(range(len(correct_count)), correct_count.values(), align='center', color='g')
p2 = plt.bar(range(len(incorrect_count)), incorrect_count.values(), align='center', color='r', bottom=correct_count.values())
plt.xticks(range(len(correct_count)), correct_count.keys())
plt.legend((p1[0], p2[0]), ('Correct', 'Incorrect'))
plt.ylabel('Number of data points')
plt.show(block=False)

correct_count_total = sum(correct_count.values())
incorrect_count_total = sum(incorrect_count.values())
print "Total correct predictions: ", correct_count_total
print "Total incorrect predictions: ", incorrect_count_total
print "Prediction accuracy: ", float(correct_count_total)/len(test)

plt.show()
