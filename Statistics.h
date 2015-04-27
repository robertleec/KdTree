#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <vector>
#include <map>
#include <set>

namespace Math {
    
    using namespace std;
    
    double sum(const vector<double>& values);
    double mean(const vector<double>& values);
    double sumOfSquares(const vector<double>& values);
    double variance(const vector<double>& values);
    double sampleStandardDeviation(const vector<double>& values);
    size_t maxValueIndex(const vector<double>& values);
    
    template <typename T>
    inline const map<T, double> distribution(const vector<T>& data) {
        
        map<T, double> result;
        
        multiset<T> multiSet;
        
        size_t size = data.size();
        
        for (size_t index = 0; index < size; ++index) {
            
            multiSet.insert(data.at(index));
        }
        
        typename multiset<T>::iterator multiSetIterator;
        
        for (multiSetIterator = multiSet.begin(); multiSetIterator != multiSet.end(); ++multiSetIterator) {
            size_t count = multiSet.count(*multiSetIterator);
            result[*multiSetIterator] = count / (double)size;
        }

        return map<T, double>(result);
    }
}

#endif