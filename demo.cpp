#include "KdTree.h"
#include <iostream>

using namespace std;

int main (int argc, char* argv[]) {
    
    vector< vector<FeatureType> > featuresVector;
    vector<NodeCategory> categoriesVector;
    
    FeatureType array1[] = {2, 3};
    vector<FeatureType> vector1(array1, array1 + 2);
    featuresVector.push_back(vector1);
    categoriesVector.push_back(1);
    
    FeatureType array2[] = {5, 4};
    vector<FeatureType> vector2(array2, array2 + 2);
    featuresVector.push_back(vector2);
    categoriesVector.push_back(2);
    
    FeatureType array3[] = {9, 6};
    vector<FeatureType> vector3(array3, array3 + 2);
    featuresVector.push_back(vector3);
    categoriesVector.push_back(2);
    
    FeatureType array4[] = {4, 7};
    vector<FeatureType> vector4(array4, array4 + 2);
    featuresVector.push_back(vector4);
    categoriesVector.push_back(1);
    
    FeatureType array5[] = {8, 1};
    vector<FeatureType> vector5(array5, array5 + 2);
    featuresVector.push_back(vector5);
    categoriesVector.push_back(1);
    
    FeatureType array6[] = {7, 2};
    vector<FeatureType> vector6(array6, array6 + 2);
    featuresVector.push_back(vector6);
    categoriesVector.push_back(2);
    
    KdTree tree;
    
    tree.build(featuresVector, categoriesVector);
    
    FeatureType arrayTest[] = {2, 4.5};
    vector<FeatureType> vectorTest(arrayTest, arrayTest + 2);
    
    KdTreeNode *nearestNode = tree.nearestNode(vectorTest);
    
    vector<FeatureType> features = nearestNode->getFeatures();
    
    vector<FeatureType>::const_iterator vectorIterator;
    
    for (vectorIterator = features.begin(); vectorIterator != features.end(); ++vectorIterator) {
        cout << *vectorIterator << endl;
    }
    
    return 0;
}