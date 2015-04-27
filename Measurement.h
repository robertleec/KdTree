#include <vector>
#include <set>
#include "Matrix.h"

namespace Measurement {
    
    using namespace std;
    
    typedef double DistanceValueType;
    typedef double DimensionValueType;
    typedef double StandardDeviationValueType;
    typedef double MeanValueType;
    typedef double CosineValueType;
    typedef double JaccardDistanceValueType;
    typedef double PearsonCorrelationCoefficientValueType;
    
    DistanceValueType euclideanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    DistanceValueType manhattanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    DistanceValueType chebyshevDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    DistanceValueType minkowskiDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, unsigned long p);
    
    DistanceValueType standardizedEuclideanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, const vector<StandardDeviationValueType>& standardDeviationsVector);
    
    DistanceValueType mahalanobisDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, const vector<MeanValueType>& meansVector);
    
    DistanceValueType bhattacharyyaDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    DistanceValueType hammingDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    CosineValueType cosine(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
    template<typename T>
    JaccardDistanceValueType jaccardDistance(const set<T>& set0, const set<T>& set1) {
        
        size_t intersectionCount = 0;
        
        typename set<T>::const_iterator iterator0;
        
        for (iterator0 = set0.begin(); iterator0 != set0.end(); ++iterator0) {
            
            if (set1.find(*iterator0) != set1.end()) {
                ++intersectionCount;
            }
        }
        
        size_t unionCount = set0.size() + set1.size() - intersectionCount;
        
        JaccardDistanceValueType distance = 1 - ((double)intersectionCount / unionCount);
        
        return distance;
    }
    
    PearsonCorrelationCoefficientValueType pearsonCorrelationCoefficient(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1);
    
}