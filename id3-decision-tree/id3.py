from math import log
import pandas as pd
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " FILENAME\n"
    quit()

data = pd.read_csv(sys.argv[1])
train = data.sample(frac=0.7)
test = data.drop(train.index).reset_index(drop=True)
train = train.reset_index(drop=True)
target_attribute = data.columns[-1]

class Node(object):
    def __init__(self):
        self.type = None
        self.children = None
        self.val = None

class NodeType:
    LEAF = 1
    ATTRIBUTE = 2

def get_entropy(values):
    total = sum(values)
    probability = map(lambda x: float(x)/total, values)
    entropy = 0
    for p in probability:
        if p > 0:
            entropy -= p*log(p,2)
    return entropy

def get_best_attribute(examples, target_attribute, attribute_list):
    min_entropy = None
    best_attribute = None
    total_count = len(examples)
    for attribute in attribute_list:
        entropy = 0
        for row in examples[attribute].value_counts().iteritems():
            val = row[0]
            count = row[1]
            entropy += float(count)/total_count * get_entropy(list(examples[examples[attribute]==val][target_attribute].value_counts()))
        if (min_entropy == None or entropy < min_entropy):
            min_entropy = entropy
            best_attribute = attribute
    return best_attribute


def ID3 (examples, target_attribute, attribute_list):
    root = Node()
    if (examples[target_attribute].value_counts().count() == 1 or len(attribute_list)==0):
        root.type = NodeType.LEAF
        root.val = examples[target_attribute].value_counts().idxmax()
    else:
        attribute = get_best_attribute(examples, target_attribute, attribute_list)
        root.type = NodeType.ATTRIBUTE
        root.val = attribute
        root.children = {}
        attribute_copy = list(attribute_list)
        attribute_copy.remove(attribute)
        for val in train[attribute].unique():
            if len(examples[examples[attribute]== val])==0:
                child = Node()
                child.type = NodeType.LEAF
                child.val = examples[target_attribute].value_counts().idxmax()
            else:
                child = ID3(examples[examples[attribute]==val], target_attribute, attribute_copy)
            root.children[val] = child
    return root

def get_label_from_tree(tree, row):
    node = tree
    while (node.type != NodeType.LEAF):
        val = row[node.val]
        node = node.children[val]
    return node.val

def hilite(string, status):
    attr = [status]
    return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)

def print_tree(tree, indents):
    if (tree.type == NodeType.LEAF):
        status = '32'
    else:
        status = '36'
    print '{0}{1}{2}'.format('|  '*(indents-1), '|--' if indents>0 else '', hilite(tree.val, status))
    if (tree.type==NodeType.ATTRIBUTE):
        for key,value in tree.children.iteritems():
            print '{0}{1}{2}'.format('|  '*indents, '|--', hilite(key, '33'))
            print_tree(value, indents+2)

attribute_list = list(data.columns)
attribute_list.remove(target_attribute)
tree = ID3(train, target_attribute, attribute_list)
correct_count = {}
incorrect_count = {}
for val in data[target_attribute].unique():
    correct_count[val] = 0
    incorrect_count[val] = 0

for index, row in test.iterrows():
    prediction = get_label_from_tree(tree, row)
    if prediction == row[target_attribute]:
        correct_count[row[target_attribute]] += 1
    else:
        incorrect_count[row[target_attribute]] += 1

p1 = plt.bar(range(len(correct_count)), correct_count.values(), align='center', color='g')
p2 = plt.bar(range(len(incorrect_count)), incorrect_count.values(), align='center', color='r', bottom=correct_count.values())
plt.xticks(range(len(correct_count)), correct_count.keys())
plt.legend((p1[0], p2[0]), ('Correct', 'Incorrect'))
plt.ylabel('Number of data points')
plt.show(block=False)

print_tree(tree,0)

correct_count_total = sum(correct_count.values())
incorrect_count_total = sum(incorrect_count.values())
print "Total correct predictions: ", correct_count_total
print "Total incorrect predictions: ", incorrect_count_total
print "Prediction accuracy: ", float(correct_count_total)/len(test)

plt.show()
