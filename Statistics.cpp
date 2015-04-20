#include "Statistics.h"
#include <cmath>
#include "assert.h"

namespace Math {
    
    using namespace std;
    
    double variance(const vector<double>& values) {
        
        assert(values.size() > 0);
        
        double sumOfSquares = 0;
        double mean = 0;
        
        vector<double>::const_iterator iterator;
        
        for (iterator = values.begin(); iterator != values.end(); ++iterator) {
            sumOfSquares += pow((*iterator), 2);
            mean += *iterator;
        }
        
        mean /= values.size();
        
        double result;
        
        result = sumOfSquares - pow(mean, 2);
        
        return result;
    }
    
    size_t maxValueIndex(const vector<double>& values) {
        
        double maxValue = 0;
        size_t maxIndex = 0;
        
        size_t index = 0;
        
        vector<double>::const_iterator iterator;
        
        for (iterator = values.begin(); iterator != values.end(); ++iterator) {
            
            if (iterator == values.begin()) {
                maxValue = *iterator;
            } else {
                if (*iterator > maxValue) {
                    maxValue = *iterator;
                    maxIndex = index;
                }
            }
            ++index;
        }
        
        return maxIndex;
    }
}