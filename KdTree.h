#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include <vector>
#include "assert.h"
#include "MaxHeap.h"
#include "Measurement.h"
#include <stack>
#include "iostream"

namespace std {
    
    using namespace std;
    
    typedef unsigned long FeatureIndexType;
    typedef double FeatureType;
    typedef unsigned long NodeCategory;
    typedef double NodeDistanceType;
    
    class KdTreeNode {
        
    public:
        
        KdTreeNode();
        
        KdTreeNode(const vector<FeatureType>& features);
        KdTreeNode(const vector<FeatureType>& features, NodeCategory category);
        
        ~KdTreeNode();
        
        void swap(KdTreeNode& other);
        
        void initRelatedTreeNode();
        
        KdTreeNode(const KdTreeNode& rhs);
        KdTreeNode& operator=(const KdTreeNode& rhs);
        
        const FeatureIndexType getSplitFeatureIndex() const;
        void setSplitFeatureIndex(FeatureIndexType splitFeatureIndex);
        
        inline const vector<FeatureType> getFeatures() const {
            return vector<FeatureType>(this->features);
        }
        
        FeatureType getSplitFeature() const;
        
        void setCategory(NodeCategory category);
        NodeCategory getCategory() const;
        
        void setParent(KdTreeNode* node);
        void setLeftChild(KdTreeNode* node);
        void setRightChild(KdTreeNode* node);
        
        KdTreeNode* getParent() const;
        KdTreeNode* getLeftChild() const;
        KdTreeNode* getRightChild() const;
        
        const bool isLeafNode() const;
        
        friend bool operator>(const KdTreeNode& lhs, const KdTreeNode& rhs) {
            
            bool result = false;
            
            if (lhs.category > rhs.category) {
                result = true;
            } else {
                
                size_t size = lhs.features.size();
                
                for (size_t index = 0; index < size; ++index) {
                    
                    if (lhs.features.at(index) > rhs.features.at(index)) {
                        result = true;
                        break;
                    }
                    
                }
                
            }
            
            return result;
        }
        
    private:
        
        FeatureIndexType splitFeatureIndex;
        
        vector<FeatureType> features;
        NodeCategory category;
        
        KdTreeNode *parent;
        KdTreeNode *leftChild;
        KdTreeNode *rightChild;
    };
    
    typedef size_t DimensionNumber;
    
    class KdTree {
        
    public:
        
        KdTree();
        
        ~KdTree();
        
        void build(const vector< vector<FeatureType> >& featuresVector, const vector<NodeCategory>& categoriesVector);
        
        //Maybe ignore some same distance nodes which have the greatest compare distance in max heap.
        inline const vector<KdTreeNode> nearestKNode(const vector<FeatureType>& features, size_t k) {
            
            vector<KdTreeNode> result;
            
            KdTreeNodeMaxHeap nodeMaxHeap(k);
            KdTreeNode objectNode(features);
            nodeMaxHeap.assignDistanceComparingNode(objectNode);
            
            KdTreeNode* node = this->nearestLeafNode(features);
            
            nodeMaxHeap.addData(*node);
            
            KdTreeNode* searchPathDirectionNode = node;
            KdTreeNode* searchPathNode = node->getParent();
            
            //Do search until root.
            while (searchPathNode != NULL) {
                
                bool isNeedToSearchInBranch = false;
                
                if (nodeMaxHeap.isReachMaxNodeNumber() == false) {
                    isNeedToSearchInBranch = true;
                } else {
                    
                    if (this->isSearchNeededInBranch(nodeMaxHeap, searchPathNode) == true) {
                        isNeedToSearchInBranch = true;
                    }
                    
                }
                
                if (isNeedToSearchInBranch == true) {
                    
                    nodeMaxHeap.addData(*searchPathNode);
                    
                    if (searchPathDirectionNode == searchPathNode->getLeftChild()) {
                        
                        if (searchPathNode->getRightChild() != NULL) {
                            vector<KdTreeNode> rightTreeNodes = this->getTreeNodes(searchPathNode->getRightChild());
                            
                            vector<KdTreeNode>::const_iterator treeNodeIterator;
                            
                            for (treeNodeIterator = rightTreeNodes.begin(); treeNodeIterator != rightTreeNodes.end(); ++treeNodeIterator) {
                                nodeMaxHeap.addData(*treeNodeIterator);
                            }
                        }
                        
                    } else {
                        
                        if (searchPathNode->getLeftChild() != NULL) {
                            vector<KdTreeNode> leftTreeNodes = this->getTreeNodes(searchPathNode->getLeftChild());
                            
                            vector<KdTreeNode>::const_iterator treeNodeIterator;
                            
                            for (treeNodeIterator = leftTreeNodes.begin(); treeNodeIterator != leftTreeNodes.end(); ++treeNodeIterator) {
                                nodeMaxHeap.addData(*treeNodeIterator);
                            }
                            
                        }
                        
                    }
                }
                
                searchPathDirectionNode = searchPathNode;
                searchPathNode = searchPathNode->getParent();
            }
            
            result = nodeMaxHeap.getAllData();
            
            return vector<KdTreeNode>(result);
        }
        
    private:
        
        class KdTreeNodeMaxHeap: public MaxHeap<KdTreeNode> {
            
        public:
            
            KdTreeNodeMaxHeap(size_t limitedNodesNumber):MaxHeap<KdTreeNode>(limitedNodesNumber) {
            
            }
            
            bool isNodeGreaterThanAnother(const KdTreeNode& node0, const KdTreeNode& node1) {
                
                bool result = false;
                
                NodeDistanceType distance0 = Measurement::euclideanDistance(node0.getFeatures(), distanceComparingNode.getFeatures());
                
                NodeDistanceType distance1 = Measurement::euclideanDistance(node1.getFeatures(), distanceComparingNode.getFeatures());
                
                if (distance0 > distance1) {
                    result = true;
                }
                
                return result;
            }
            
            void assignDistanceComparingNode(const KdTreeNode& node) {
                this->distanceComparingNode = node;
            }
            
            inline const vector<FeatureType> featuresCompared() const {
                return distanceComparingNode.getFeatures();
            }
            
            inline const NodeDistanceType maxDistanceCompared() const {
                
                KdTreeNode topNode = this->maxData();
                
                NodeDistanceType distance = Measurement::euclideanDistance(topNode.getFeatures(), distanceComparingNode.getFeatures());
                
                return distance;
            }
            
        private:
            
            KdTreeNode distanceComparingNode;
        };
        
        KdTreeNode *rootNode;
        
        void deleteTree(KdTreeNode *treeRoot);
        
        KdTreeNode *buildTree(vector<KdTreeNode>& nodes);
        DimensionNumber getMaxVarianceDimensionIndex(const vector<KdTreeNode>& nodesVector);
        
        inline const KdTreeNode getSplitNode(const vector<KdTreeNode>& nodesVector, DimensionNumber splitDimensionIndex) {
            
            assert(nodesVector.size() > 0);
            return KdTreeNode(nodesVector.at(nodesVector.size() / 2));
        }
        
        inline const vector<KdTreeNode> getLeftTree(const vector<KdTreeNode>& nodesVector, const KdTreeNode& splitNode, DimensionNumber splitDimensionIndex) {
            
            vector<KdTreeNode> result;
            
            vector<KdTreeNode>::const_iterator iterator;
            
            for (iterator = nodesVector.begin(); iterator != nodesVector.end(); ++iterator) {
                
                vector<FeatureType> features = (*iterator).getFeatures();
                
                if (features.at(splitDimensionIndex) < splitNode.getFeatures().at(splitDimensionIndex)) {
                    result.push_back(*iterator);
                }
            }
            
            return vector<KdTreeNode>(result);
        }
        
        inline const vector<KdTreeNode> getRightTree(const vector<KdTreeNode>& nodesVector, const KdTreeNode& splitNode, DimensionNumber splitDimensionIndex) {
            
            vector<KdTreeNode> result;
            
            vector<KdTreeNode>::const_iterator iterator;
            
            for (iterator = nodesVector.begin(); iterator != nodesVector.end(); ++iterator) {
                
                vector<FeatureType> features = (*iterator).getFeatures();
                
                if (features.at(splitDimensionIndex) > splitNode.getFeatures().at(splitDimensionIndex)) {
                    result.push_back(*iterator);
                }
            }
            return vector<KdTreeNode>(result);
        }
        
        inline const vector<FeatureType> getNodeFeatureInDimension(const vector<KdTreeNode>& nodes, DimensionNumber dimensionIndex) {
            
            vector<FeatureType> dimensionFeature;
            
            vector<KdTreeNode>::const_iterator iterator;
            
            for (iterator = nodes.begin(); iterator != nodes.end(); ++iterator) {
                assert((*iterator).getFeatures().size() > dimensionIndex);
                dimensionFeature.push_back((*iterator).getFeatures().at(dimensionIndex));
            }
            
            return vector<FeatureType>(dimensionFeature);
        }
        
        void qSort(vector<KdTreeNode>& nodes, DimensionNumber splitDimensionIndex);
        void quickSort(vector<KdTreeNode>& nodes, size_t low, size_t up, DimensionNumber splitDimensionIndex);
        size_t partition(vector<KdTreeNode>& nodes, size_t low, size_t up, DimensionNumber splitDimensionIndex);
        
        //Ignore middle same compare distance node.
        KdTreeNode* nearestLeafNode(const vector<FeatureType>& features);
        
        const bool isSearchNeededInBranch(const KdTreeNodeMaxHeap& nodeMaxHeap, KdTreeNode* node) const;
        
        const bool isFeatureNodeContained(const vector<FeatureType>& features) const;
        
        inline const vector<KdTreeNode> getTreeNodes(KdTreeNode* treeRootNode) const {
            
            vector<KdTreeNode> treeNodes;
            
            stack<KdTreeNode*> path;
            
            KdTreeNode* node = treeRootNode;
            
            while (node != NULL) {
                path.push(node);
                node = node->getLeftChild();
            }
            
            while (path.empty() == false) {
                
                node = path.top();
                treeNodes.push_back(*node);
                path.pop();
                
                if (node->getRightChild() != NULL) {
                    
                    node = node->getRightChild();
                    
                    while (node != NULL) {
                        path.push(node);
                        node = node->getLeftChild();
                    }
                }
            }
            
            return vector<KdTreeNode>(treeNodes);
        }
    };

}

namespace std {
    template<>
    void swap<std::KdTreeNode>(std::KdTreeNode& a, std::KdTreeNode& b);
}

#endif