#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include <vector>
#include "assert.h"

namespace std {
    
    template<typename T>
    class MinHeap {
        
    public:
        
        MinHeap() {
            
        }
        
        MinHeap(const MinHeap& rhs):isNodesNumberLimited(rhs.isNodesNumberLimited), limitedNodesNumber(rhs.limitedNodesNumber), nodes(rhs.nodes) {
            
        }
        
        MinHeap& operator=(const MinHeap& rhs) {
            MinHeap temp(rhs);
            this->swap(temp);
            return *this;
        }
        
        MinHeap(size_t limitedNodesNumber):limitedNodesNumber(limitedNodesNumber) {
            this->isNodesNumberLimited = true;
        }
        
        MinHeap(const vector<T>& dataVector):nodes(dataVector) {
            
        }
        
        ~MinHeap() {
            
        }
        
        void swap(MinHeap& other) {
            using std::swap;
            swap(this->isNodesNumberLimited, other.isNodesNumberLimited);
            swap(this->limitedNodesNumber, other.limitedNodesNumber);
            swap(this->nodes, other.nodes);
        }
        
        size_t nodesNumber() {
            return this->nodes.size();
        }
        
        void buildHeap(const vector<T>& dataVector) {
            this->nodes = dataVector;
        }
        
        const vector<T> getAllData() {
            return vector<T>(this->nodes);
        }
        
        void addData(const T& data) {
            
            if (this->isReachMaxNodeNumber() == true) {
                
                if (this->isNodeSmallerThanAnother(this->nodes.at(0), data)) {
                    using std::swap;
                    
                    T temp(data);
                    swap(this->nodes.at(0), temp);
                    this->adjustHeap();
                }
            } else {
                this->nodes.push_back(data);
                this->adjustHeap();
            }
        }
        
        const T minData() const {
            
            assert(this->nodes.size() > 0);
            
            return this->nodes.at(0);
        }
        
        const T removeMinData() {
            
            assert(this->nodes.size() > 0);
            
            if (this->nodes.size() > 1) {
                using std::swap;
                
                swap(this->nodes.at(0), this->nodes.at(this->nodes.at(this->nodes.size() - 1)));
            }
            
            T result(this->nodes.at(this->nodes.size() - 1));
            
            this->nodes.erase(this->nodes.back());
            
            return T(result);
        }
        
        void adjustHeap() {
            
            if (this->nodes.size() > 1) {
                
                for (long index = this->rFirstNonLeafNodeIndex(); index >= 0; --index) {
                    this->adjustSubHeap(index);
                }
                
            }
            
        }
        
        const size_t rFirstNonLeafNodeIndex() const {
            
            assert(this->nodes.size() > 1);
            
            size_t index = this->nodes.size() / 2 - 1;
            
            return index;
        }
        
        void adjustSubHeap(size_t subHeapRootIndex) {
            
            using std::swap;
            
            size_t leftChildIndex = (subHeapRootIndex + 1) * 2 - 1;
            size_t rightChildIndex = (subHeapRootIndex + 1) * 2;
            
            if (leftChildIndex < this->nodes.size() && this->isNodeSmallerThanAnother(this->nodes.at(leftChildIndex), this->nodes.at(subHeapRootIndex))) {
                swap(this->nodes.at(leftChildIndex), this->nodes.at(subHeapRootIndex));
                this->adjustSubHeap(leftChildIndex);
            }
            
            if (rightChildIndex < this->nodes.size() && this->isNodeSmallerThanAnother(this->nodes.at(rightChildIndex), this->nodes.at(subHeapRootIndex)))) {
                swap(this->nodes.at(rightChildIndex), this->nodes.at(subHeapRootIndex));
                this->adjustSubHeap(rightChildIndex);
            }
        }
        
        virtual bool isNodeSmallerThanAnother(const T& node0, const T& node1) = 0;
        
        bool isReachMaxNodeNumber() const {
            
            bool result = false;
            
            if (this->isNodesNumberLimited == true && this->nodes.size() == this->limitedNodesNumber) {
                result = true;
            }
            
            return result;
        }
        
    private:
        
        vector<T> nodes;
        
        bool isNodesNumberLimited;
        size_t limitedNodesNumber;
    };
}

namespace std {
    template<typename T>
    void swap(MinHeap<T>& a, MinHeap<T>& b) {
        a.swap(b);
    }
}

#endif