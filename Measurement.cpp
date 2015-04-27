#include "Measurement.h"
#include <cmath>
#include "Statistics.h"
#include "assert.h"

namespace Measurement {
    
    using namespace std;
    
    DistanceValueType euclideanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            distance += pow(vector0.at(index) - vector1.at(index), 2);
        }
        
        distance = sqrt(distance);
        
        return distance;
    }
    
    DistanceValueType manhattanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            distance += fabs(vector0.at(index) - vector1.at(index));
        }
        
        return distance;
    }
    
    DistanceValueType chebyshevDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            DistanceValueType temp = fabs(vector0.at(index) - vector1.at(index));
            
            if (temp > distance) {
                distance = temp;
            }
        }
        
        return distance;
    }
    
    DistanceValueType minkowskiDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, unsigned long p) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            
            distance += pow(fabs(vector0.at(index) - vector1.at(index)), p);
            
        }
        
        distance = pow(distance, 1.0 / p);
        
        return distance;
    }
    
    DistanceValueType standardizedEuclideanDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, const vector<StandardDeviationValueType>& standardDeviationsVector) {
        
        assert(vector0.size() == vector1.size());
        assert(vector0.size() == standardDeviationsVector.size());
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            assert(standardDeviationsVector.at(index) != 0);
        }
        
        DistanceValueType distance = 0;
        
        for (size_t index = 0; index < size; ++index) {
            distance += pow((vector0.at(index) - vector1.at(index)) / standardDeviationsVector.at(index), 2);
        }
        
        distance = sqrt(distance);
        
        return distance;
    }
    
    DistanceValueType mahalanobisDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1, const vector<MeanValueType>& meansVector) {
        
        assert(vector0.size() == vector1.size());
        assert(vector0.size() == meansVector.size());
        
        Matrix matrixS;
        
        size_t size = vector0.size();
        
        for (size_t rowIndex = 0; rowIndex < size; ++rowIndex) {
            
            vector<ElementType> covarianceRow;
            
            for (size_t columIndex = 0; columIndex < size; ++columIndex) {
                
                ElementType element = ((vector0.at(rowIndex) - meansVector.at(rowIndex)) * (vector0.at(columIndex) - meansVector.at(columIndex)) + (vector1.at(rowIndex) - meansVector.at(rowIndex)) * (vector1.at(columIndex) - meansVector.at(columIndex))) / 2;
                
                covarianceRow.push_back(element);
            }
            
            matrixS.addRow(covarianceRow);
        }
        
        Matrix matrix0;
        matrix0.addRow(vector0);
        
        Matrix matrix1;
        matrix1.addRow(vector1);
        
        Matrix matrixD = matrix0 + ((-1) * matrix1);
        
        Matrix resultMatrix = matrixD * matrixS.inverseOfSquareMatrix() * matrixD.transposeMatrix();
        
        vector<ElementType> resultVector = resultMatrix.getRow(0);
        DistanceValueType distance = resultVector.at(0);
        
        return distance;
    }
    
    DistanceValueType bhattacharyyaDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            distance += sqrt(vector0.at(index) * vector1.at(index));
        }
        
        distance = (-1) * log(distance);
        
        return distance;
    }
    
    DistanceValueType hammingDistance(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        DistanceValueType distance = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            
            if (vector0.at(index) != vector1.at(index)) {
                ++distance;
            }
        }
        
        return distance;
    }
    
    typedef double SquareValueType;
    CosineValueType cosine(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        
        CosineValueType cosineValue = 0;
        
        size_t size = vector0.size();
        
        SquareValueType dotProduct = 0;
        SquareValueType sumOfSquare0 = 0;
        SquareValueType sumOfSquare1 = 0;
        
        for (size_t index = 0; index < size; ++index) {
            
            dotProduct += vector0.at(index) * vector1.at(index);
            sumOfSquare0 += pow(vector0.at(index), 2);
            sumOfSquare1 += pow(vector1.at(index), 2);
        }
        
        assert(sumOfSquare0 != 0 && sumOfSquare1 != 0);
        
        cosineValue = dotProduct / (sqrt(sumOfSquare0) * sqrt(sumOfSquare1));
        
        return cosineValue;
    }
    
    typedef double MeanValueType;
    typedef double StandardScoreValueType;
    typedef double SampleStandardDeviationValueType;
    PearsonCorrelationCoefficientValueType pearsonCorrelationcoefficient(const vector<DimensionValueType>& vector0, const vector<DimensionValueType>& vector1) {
        
        assert(vector0.size() == vector1.size());
        assert(vector0.size() > 1);
        
        MeanValueType mean0 = Math::mean(vector0);
        MeanValueType mean1 = Math::mean(vector1);
        
        SampleStandardDeviationValueType sampleStandardDeviation0 = Math::sampleStandardDeviation(vector0);
        SampleStandardDeviationValueType sampleStandardDeviation1 = Math::sampleStandardDeviation(vector1);
        
        PearsonCorrelationCoefficientValueType result = 0;
        
        size_t size = vector0.size();
        
        for (size_t index = 0; index < size; ++index) {
            
            StandardScoreValueType standardScore0 = (vector0.at(index) - mean0) / sampleStandardDeviation0;
            StandardScoreValueType standardScore1 = (vector1.at(index) - mean1) / sampleStandardDeviation1;
            
            result += standardScore0 * standardScore1;
        }
        
        result /= (size - 1);
        
        return result;
    }
    
}