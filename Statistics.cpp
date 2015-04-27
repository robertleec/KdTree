#include "Statistics.h"
#include <cmath>
#include "assert.h"

namespace Math {
    
    using namespace std;
    
    double sum(const vector<double>& values) {
        
        assert(values.size() > 0);
        
        double sumValue = 0;
        
        vector<double>::const_iterator vectorIterator;
        
        for (vectorIterator = values.begin(); vectorIterator != values.end(); ++vectorIterator) {
            sumValue += *vectorIterator;
        }
        
        return sumValue;
    }
    
    double mean(const vector<double>& values) {
        
        assert(values.size() > 0);
        
        double result = 0;
        
        result = sum(values) / values.size();
        
        return result;
    }
    
    double sumOfSquares(const vector<double>& values) {
        
        assert(values.size() > 0);
        
        double result = 0;
        
        vector<double>::const_iterator vectorIterator;
        
        for (vectorIterator = values.begin(); vectorIterator != values.end(); ++vectorIterator) {
            result += pow(*vectorIterator, 2);
        }
        
        return result;
    }
    
    double variance(const vector<double>& values) {
        
        assert(values.size() > 0);
        
        double sumOfSquaresValue = sumOfSquares(values);
        double meanValue = mean(values);
        
        double result;
        
        result = sumOfSquaresValue - pow(meanValue, 2);
        
        return result;
    }
    
    double sampleStandardDeviation(const vector<double>& values) {
        
        assert(values.size() > 1);
        
        double result = 0;
        
        result = sqrt(variance(values) / (values.size() - 1));
        
        return result;
    }
    
    size_t maxValueIndex(const vector<double>& values) {
        
        double maxValue = 0;
        size_t maxIndex = 0;
        
        size_t index = 0;
        
        vector<double>::const_iterator vectorIterator;
        
        for (vectorIterator = values.begin(); vectorIterator != values.end(); ++vectorIterator) {
            
            if (vectorIterator == values.begin()) {
                maxValue = *vectorIterator;
            } else {
                if (*vectorIterator > maxValue) {
                    maxValue = *vectorIterator;
                    maxIndex = index;
                }
            }
            ++index;
        }
        
        return maxIndex;
    }
}