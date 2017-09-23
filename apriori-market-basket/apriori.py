import math
import sys
import itertools

if len(sys.argv) != 3:
    print "Invalid number of arguments!\nUsage: \npython ", sys.argv[0], " <FILENAME> <MINIMUM_SUPPORT>\n"
    quit()

def apriori(basketData, minSupport):
    frequentDatasets = {0: {}}
    itemCounts = get_item_counts(basketData)
    size = 0
    frequentItems = {k:v for k,v in itemCounts.iteritems() if v > minSupport}
    while len(frequentItems) > 0:
        size += 1
        frequentDatasets[size] = frequentItems
        # initialize the frequent items dictionary one item bigger than the last one
        biggerFrequentItems = {}
        # iterate over all pairs of previous frequent items
        frequentItemSet = set(frequentItems.keys())
        for pair in list(itertools.combinations(frequentItems.keys(), 2)):
            itemUnion = set(pair[0]).union(set(pair[1]))
            if is_a_valid_frequent_item_candidate(itemUnion, frequentItemSet, biggerFrequentItems, size+1):
                biggerFrequentItems[frozenset(itemUnion)] = 0

        for transaction in basketData:
            for itemList in biggerFrequentItems.keys():
                if itemList.issubset(transaction):
                    biggerFrequentItems[itemList] += 1
        frequentItems = {k:v for k,v in biggerFrequentItems.iteritems() if v > minSupport}
    return frequentDatasets

def is_a_valid_frequent_item_candidate(candidate, frequentItemSet, newDictionary, size):
    # union of pairs has more items than intended
    if len(candidate) != size:
        return False
    # candidate has already been considered
    if frozenset(candidate) in newDictionary:
        return False
    # check if all size-1 subsets of candidate are frequent
    for item in candidate:
        subset = frozenset(candidate.difference(set([item])))
        if subset not in frequentItemSet:
            return False
    return True

def get_item_counts(basketData):
    itemCounts = {}
    for transaction in basketData:
        for item in transaction:
            if frozenset([item]) in itemCounts:
                itemCounts[frozenset([item])] += 1
            else:
                itemCounts[frozenset([item])] = 1
    return itemCounts

def print_frequent_items(dictionary, size):
    print "Frequent item sets of size: ", size
    for itemList in dictionary:
        print list(itemList), ": ", dictionary[itemList]

minSupport = int(sys.argv[2])
fileName = sys.argv[1]

file = open(fileName)
basketData = []
for line in file:
    line = line.strip()
    items = line.split()
    if (len(items)>0):
        basketData.append(set(items))

frequentDatasets = apriori(basketData, minSupport)
if len(frequentDatasets.keys())==1:
    print "There are no frequent item sets with support greater than or equal to ", minSupport
    sys.exit(0)

for itemListSize in sorted(frequentDatasets.keys(), reverse=True):
    if itemListSize > 0:
        print_frequent_items(frequentDatasets[itemListSize], itemListSize)