# -*- coding: utf-8 -*

import time

from sklearn import metrics
from sklearn.model_selection import train_test_split

import numpy as np

from sklearn import tree
from sklearn.ensemble import GradientBoostingClassifier


# Decision Tree Classifier
def decision_tree_classifier(train_x, train_y):
    model = tree.DecisionTreeClassifier(criterion='gini',splitter='best') #criterion='entropy',splitter='best,random'criterion='gini' splitter='best' max_depth=None
    model.fit(train_x, train_y)
    return model
# GBDT(Gradient Boosting Decision Tree) Classifier
def gradient_boosting_classifier(train_x, train_y):#梯度提升树
    model = GradientBoostingClassifier(n_estimators=100,learning_rate=0.07,loss="deviance")#弱学习器（即回归树）的数量由参数n_estimators控制, learning_rate是在通过在范围（0.0,1.0）loss有对数似然损失函数"deviance"和指数损失函数"exponential"两者输入选择
    model.fit(train_x, train_y)
    return model


def iris_type(s):
    it = {b'Iris-setosa': 0, b'Iris-versicolor': 1, b'Iris-virginica': 2}
    return it[s]

if __name__ == '__main__':

    path = 'G:/研究生论文/分类代码及模式/模式信息用户qing.txt'#模式信息用户qing.txt
    data = np.loadtxt(path, dtype=float, delimiter=',')
    thresh = 0.5
    test_classifiers = ['DT','GBDT']
    classifiers = {
                   'DT':decision_tree_classifier,
                   'GBDT':gradient_boosting_classifier,
    }

    print ('reading training and testing data...')
    x,y= np.split(data, (11,), axis=1)
    #------用basic数据训练分类器时打开以下三行注释（用 模式信息用户qing.txt 文件或模式信息用户our.txt文件均可）---
    #x1=x[:,0:6]
    # print(x1.shape)
    #x=x1

    train_x, test_x, train_y, test_y = train_test_split(x, y, random_state=1, train_size=0.8, test_size=0.2)
    num_train, num_feat = train_x.shape
    num_test, num_feat = test_x.shape
    is_binary_class = (len(np.unique(train_y)) == 2)
    print ('******************** Data Info *********************')
    print(('#training data: %d, #testing_data: %d, dimension: %d' % (num_train, num_test, num_feat)))


    for classifier in test_classifiers:
        print(('******************* %s ********************' % classifier))
        start_time = time.time()
        model = classifiers[classifier](train_x, train_y.ravel())
        print(('training took %fs!' % (time.time() - start_time)))

        predict = model.predict(test_x)


        if is_binary_class:
           precision = metrics.precision_score(test_y, predict)
           recall = metrics.recall_score(test_y, predict)
           print(('precision: %.2f%%, recall: %.2f%%' % (100 * precision, 100 * recall)))


        accuracy = metrics.accuracy_score(test_y, predict)
        print(('accuracy: %.2f%%' % (100 * accuracy)))

