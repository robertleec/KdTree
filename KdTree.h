#ifndef __KD_TREE_H__
#define __KD_TREE_H__

#include <vector>
#include "assert.h"

namespace std {
    
    using namespace std;
    
    typedef double FeatureType;
    typedef unsigned long NodeCategory;
    
    class KdTreeNode {
        
    public:
        
        KdTreeNode();
        
        KdTreeNode(const vector<FeatureType>& features);
        KdTreeNode(const vector<FeatureType>& features, NodeCategory category);
        
        ~KdTreeNode();
        
        void swap(KdTreeNode& other);
        
        KdTreeNode(const KdTreeNode& rhs);
        KdTreeNode& operator=(const KdTreeNode& rhs);
        
        inline const vector<FeatureType> getFeatures() const {
            return vector<FeatureType>(this->features);
        }
        
        void setCategory(NodeCategory category);
        NodeCategory getCategory() const;
        
        void setLeftChild(KdTreeNode* node);
        void setRightChild(KdTreeNode* node);
        
        KdTreeNode* getLeftChild() const;
        KdTreeNode* getRightChild() const;
        
    private:
        
        vector<FeatureType> features;
        NodeCategory category;
        
        KdTreeNode *leftChild;
        KdTreeNode *rightChild;
    };
    
    typedef size_t DimensionNumber;
    
    class KdTree {
        
    public:
        
        KdTree();
        
        ~KdTree();
        
        void build(const vector< vector<FeatureType> >& featuresVector, const vector<NodeCategory>& categoriesVector);
        
    private:
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
    };
    
}

namespace std {
    template<>
    void swap<std::KdTreeNode>(std::KdTreeNode& a, std::KdTreeNode& b);
}

#endif