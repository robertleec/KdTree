#include "KdTree.h"
#include "Statistics.h"
#include "Measurement.h"
#include <cmath>
#include "assert.h"
#include "MaxHeap.h"

namespace std {
    
    KdTreeNode::KdTreeNode() {
        this->initRelatedTreeNode();
    }
    
    KdTreeNode::KdTreeNode(const vector<FeatureType>& features):features(features) {
        this->initRelatedTreeNode();
    }
    
    KdTreeNode::KdTreeNode(const vector<FeatureType>& features, NodeCategory category):features(features), category(category) {
        this->initRelatedTreeNode();
    }
    
    KdTreeNode::~KdTreeNode() {
        
    }
    
    KdTreeNode::KdTreeNode(const KdTreeNode& rhs):splitFeatureIndex(rhs.splitFeatureIndex), features(rhs.features), category(rhs.category), parent(rhs.parent), leftChild(rhs.leftChild), rightChild(rhs.rightChild) {
        
    }
    
    KdTreeNode& KdTreeNode::operator=(const KdTreeNode& rhs) {
        KdTreeNode temp(rhs);
        this->swap(temp);
        return *this;
    }
    
    void KdTreeNode::swap(KdTreeNode& other) {
        using std::swap;
        swap(this->splitFeatureIndex, other.splitFeatureIndex);
        swap(this->features, other.features);
        swap(this->category, other.category);
        swap(this->parent, other.parent);
        swap(this->leftChild, other.leftChild);
        swap(this->rightChild, other.rightChild);
    }
    
    void KdTreeNode::initRelatedTreeNode() {
        this->parent = NULL;
        this->leftChild = NULL;
        this->rightChild = NULL;
    }
    
    const FeatureIndexType KdTreeNode::getSplitFeatureIndex() const {
        return this->splitFeatureIndex;
    }
    
    void KdTreeNode::setSplitFeatureIndex(FeatureIndexType splitFeatureIndex) {
        this->splitFeatureIndex = splitFeatureIndex;
    }
    
    void KdTreeNode::setCategory(NodeCategory category) {
        this->category = category;
    }
    
    NodeCategory KdTreeNode::getCategory() const {
        return this->category;
    }
    
    FeatureType KdTreeNode::getSplitFeature() const {
        
        assert(this->features.size() > this->splitFeatureIndex);
        
        return this->features.at(this->splitFeatureIndex);
    }
    
    void KdTreeNode::setParent(KdTreeNode* node) {
        this->parent = node;
    }
    
    void KdTreeNode::setLeftChild(KdTreeNode* node) {
        this->leftChild = node;
    }
    
    void KdTreeNode::setRightChild(KdTreeNode* node) {
        this->rightChild = node;
    }
    
    KdTreeNode* KdTreeNode::getParent() const {
        return this->parent;
    }
    
    KdTreeNode* KdTreeNode::getLeftChild() const {
        return this->leftChild;
    }
    
    KdTreeNode* KdTreeNode::getRightChild() const {
        return this->rightChild;
    }
    
    const bool KdTreeNode::isLeafNode() const {
        
        bool result = false;
        
        if (this->leftChild == NULL && this->rightChild == NULL) {
            result = true;
        }
        
        return result;
    }
    
    KdTree::KdTree() {
    
    }
    
    KdTree::~KdTree() {
        this->deleteTree(this->rootNode);
    }
    
    void KdTree::deleteTree(KdTreeNode *treeRoot) {
        
        if (treeRoot != NULL) {
            this->deleteTree(treeRoot->getLeftChild());
            this->deleteTree(treeRoot->getRightChild());
            delete treeRoot;
        }
    }
    
    void KdTree::build(const vector< vector<FeatureType> >& featuresVector, const vector<NodeCategory>& categoriesVector) {
        
        vector<KdTreeNode> nodes;
        
        vector< vector<FeatureType> >::const_iterator featuresIterator;
        vector<NodeCategory>::const_iterator categoryIterator;
        
        for (featuresIterator = featuresVector.begin(), categoryIterator = categoriesVector.begin(); featuresIterator != featuresVector.end() && categoryIterator != categoriesVector.end(); ++featuresIterator, ++categoryIterator) {
            
            KdTreeNode node(*featuresIterator, *categoryIterator);
            nodes.push_back(node);
        }
        
        this->rootNode = this->buildTree(nodes);
    }
    
    KdTreeNode* KdTree::buildTree(vector<KdTreeNode>& nodes) {
        
        if (nodes.size() == 0) {
            return NULL;
        }
        
        DimensionNumber splitDimensionIndex = this->getMaxVarianceDimensionIndex(nodes);
        
        this->qSort(nodes, splitDimensionIndex);
        
        KdTreeNode *node = new KdTreeNode(this->getSplitNode(nodes, splitDimensionIndex));
        
        vector<KdTreeNode> leftTree = this->getLeftTree(nodes, *node, splitDimensionIndex);
        vector<KdTreeNode> rightTree = this->getRightTree(nodes, *node, splitDimensionIndex);
        
        node->setSplitFeatureIndex(splitDimensionIndex);
        
        KdTreeNode *leftTreeRootNode = (this->buildTree(leftTree));
        node->setLeftChild(leftTreeRootNode);
        if (leftTreeRootNode != NULL) {
            leftTreeRootNode->setParent(node);
        }
        
        
        KdTreeNode *rightTreeRootNode = (this->buildTree(rightTree));
        node->setRightChild(rightTreeRootNode);
        if (rightTreeRootNode != NULL) {
            rightTreeRootNode->setParent(node);
        }
        
        return node;
    }
    
    DimensionNumber KdTree::getMaxVarianceDimensionIndex(const vector<KdTreeNode>& nodesVector) {
        
        assert(nodesVector.size() > 0);
        
        vector<double> variancesVector;
        
        vector<KdTreeNode>::const_iterator nodeIterator;
        
        nodeIterator = nodesVector.begin();
        
        DimensionNumber dimensionNumber;
        
        dimensionNumber = (*nodeIterator).getFeatures().size();
        
        for (DimensionNumber index = 0; index < dimensionNumber; ++index) {
            vector<FeatureType> dimensionVector = this->getNodeFeatureInDimension(nodesVector, index);
            variancesVector.push_back(Math::variance(dimensionVector));
        }
        
        return Math::maxValueIndex(variancesVector);
    }
    
    void KdTree::qSort(vector<KdTreeNode>& nodes, DimensionNumber splitDimensionIndex) {
        this->quickSort(nodes, 0, nodes.size() - 1, splitDimensionIndex);
    }
    
    void KdTree::quickSort(vector<KdTreeNode>& nodes, size_t low, size_t up, DimensionNumber splitDimensionIndex) {
        if (low < up) {
            size_t middle = this->partition(nodes, low, up, splitDimensionIndex);
            
            if (middle > 0) {
                this->quickSort(nodes, low, middle - 1, splitDimensionIndex);
                this->quickSort(nodes, middle + 1, up, splitDimensionIndex);
            }
        }
    }
    
    size_t KdTree::partition(vector<KdTreeNode>& nodes, size_t low, size_t up, DimensionNumber splitDimensionIndex) {
        
        using std::swap;
        
        KdTreeNode& pivot = nodes.at(up);
        size_t index0 = low;
        
        for (size_t index1 = low; index1 < up - 1; ++index1) {
            
            if (nodes.at(index1).getFeatures().at(splitDimensionIndex) <= pivot.getFeatures().at(splitDimensionIndex)) {
                
                swap(nodes.at(index0), nodes.at(index1));
                ++index0;
            }
        }
        
        swap(nodes.at(index0), nodes.at(up));
        
        return index0;
    }
    
    KdTreeNode* KdTree::nearestLeafNode(const vector<FeatureType>& features) {
        
        KdTreeNode* node = this->rootNode;
        
        while (node->isLeafNode() == false) {
            
            FeatureIndexType splitFeatureIndex = node->getSplitFeatureIndex();
            
            if (features.at(splitFeatureIndex) < node->getSplitFeature()) {
                
                if (node->getLeftChild() == NULL) {
                    break;
                }
                
                node = node->getLeftChild();
            } else {
                
                if (node->getRightChild() == NULL) {
                    break;
                }
                
                node = node->getRightChild();
            }
        }
        
        return node;
    }
    
    const bool KdTree::isSearchNeededInBranch(const KdTreeNodeMaxHeap& nodeMaxHeap, KdTreeNode* parent) const {
        
        bool result = false;
        
        if (parent != NULL) {
            
            FeatureIndexType splitFeatureIndex = parent->getSplitFeatureIndex();
            
            NodeDistanceType splitFeatureDistance = fabs(nodeMaxHeap.featuresCompared().at(splitFeatureIndex) - parent->getFeatures().at(splitFeatureIndex));
            
            //Ingore the same node distance between parent.
            if (nodeMaxHeap.maxDistanceCompared() > splitFeatureDistance) {
                result = true;
            }
        }
        
        return result;
    }
    
    const bool KdTree::isFeatureNodeContained(const vector<FeatureType>& features) const {
        
        bool result = false;
        
        KdTreeNode* node = this->rootNode;
        
        while (node->isLeafNode() == false) {
            
            if (node->getFeatures() == features) {
                result = true;
                break;
            }
            
            FeatureIndexType splitFeatureIndex = node->getSplitFeatureIndex();
            
            if (node->getSplitFeature() < features.at(splitFeatureIndex)) {
                
                if (node->getLeftChild() == NULL) {
                    break;
                }
                
                node = node->getLeftChild();
            } else {
                
                if (node->getRightChild() == NULL) {
                    break;
                }
                
                node = node->getRightChild();
            }
        }
        
        if (result != true) {
            
            if (node->getFeatures() == features) {
                result = true;
            }
        }
        
        return result;
    }
}

namespace std {
    template<>
    void swap<std::KdTreeNode>(std::KdTreeNode& a, std::KdTreeNode& b) {
        a.swap(b);
    }
}
