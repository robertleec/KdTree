#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <vector>
#include "assert.h"

namespace std {
    
    typedef double ElementType;
    
    typedef unsigned long SizeType;
    typedef double MultipleNumberType;
    typedef unsigned long RankValueType;
    typedef double TraceValueType;
    
    class Matrix {
        
    public:
        Matrix();
        
        Matrix(const Matrix& rhs);
        Matrix& operator=(const Matrix& rhs);
        
        Matrix(SizeType rowNumber, SizeType columNumber);
        
        ~Matrix();
        
        void swap(Matrix& other);
        
        bool isEmpty() const;
        bool isSquareMatrix() const;
        
        inline const SizeType getRowNumber() const {
            return (this->data).size();
        }
        
        inline const SizeType getColumNumber() const {
            
            if (this->getRowNumber() > 0) {
                
                const vector<ElementType>& firstColum = (this->data).at(0);
                
                return firstColum.size();
            }
            
            return 0;
        }
        
        inline const vector<ElementType> getRow(SizeType rowIndex) const {
            
            assert(this->getRowNumber() > rowIndex);
            
            return vector<ElementType>(this->data.at(rowIndex));
        }
        
        inline const vector<ElementType> getColum(SizeType columIndex) const {
            
            assert(this->getColumNumber() > columIndex);
            
            vector<ElementType> colum;
            
            for (size_t rowIndex = 0; rowIndex < this->getRowNumber(); ++rowIndex) {
                colum.push_back(this->data.at(rowIndex).at(columIndex));
            }
            
            return vector<ElementType>(colum);
        }
        
        void addRow(const vector<ElementType>& rowData);
        void deleteRow(SizeType rowIndex);
        
        void addColum(const vector<ElementType>& columData);
        void deleteColum(SizeType columIndex);
        
        friend inline const Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
            
            assert(lhs.isEmpty() == false && rhs.isEmpty() == false);
            assert(lhs.getRowNumber() == rhs.getRowNumber() && lhs.getColumNumber() == rhs.getColumNumber());
            
            Matrix result(lhs.getRowNumber(), lhs.getColumNumber());
            
            for (SizeType rowIndex = 0; rowIndex < lhs.getRowNumber(); ++rowIndex) {
                for (SizeType columIndex = 0; columIndex < lhs.getColumNumber(); ++columIndex) {
                    result.data.at(rowIndex).at(columIndex) = lhs.data.at(rowIndex).at(columIndex) + rhs.data.at(rowIndex).at(columIndex);
                }
            }
            
            return Matrix(result);
        }
        
        friend inline const Matrix operator*(MultipleNumberType multipleNumber, const Matrix& rhs) {
            
            Matrix result(rhs);
            
            if (rhs.isEmpty() == false) {
                
                for (SizeType rowIndex = 0; rowIndex < rhs.getRowNumber(); ++rowIndex) {
                    for (SizeType columIndex = 0; columIndex < rhs.getColumNumber(); ++columIndex) {
                        result.data.at(rowIndex).at(columIndex) *= multipleNumber;
                    }
                }
                
            }
            
            return Matrix(result);
        }
        
        friend inline const Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
            
            assert(lhs.isEmpty() == false && rhs.isEmpty() == false);
            assert(lhs.getColumNumber() == rhs.getRowNumber());
            
            Matrix result(lhs.getRowNumber(), rhs.getColumNumber());
            
            for (SizeType rowIndex = 0; rowIndex < result.getRowNumber(); ++rowIndex) {
                for (SizeType columIndex = 0; columIndex < result.getColumNumber(); ++columIndex) {
                    
                    ElementType multipleElement = 0;
                    
                    for (SizeType multipleElementIndex = 0; multipleElementIndex < lhs.getColumNumber(); ++multipleElementIndex) {
                        
                        multipleElement += lhs.data.at(rowIndex).at(multipleElementIndex) * rhs.data.at(multipleElementIndex).at(columIndex);
                    }
                    
                    result.data.at(rowIndex).at(columIndex) = multipleElement;
                }
            }
            
            return Matrix(result);
        }
        
        inline const Matrix transposeMatrix() {
            
            Matrix result(this->getColumNumber(), this->getRowNumber());
            
            if (this->isEmpty() == false) {
                
                for (SizeType rowIndex = 0; rowIndex < result.getRowNumber(); ++rowIndex) {
                    for (SizeType columIndex = 0; columIndex < result.getColumNumber(); ++columIndex) {
                        result.data.at(rowIndex).at(columIndex) = this->data.at(columIndex).at(rowIndex);
                    }
                }
            }
            
            return Matrix(result);
        }
        
        void switchTwoRow(SizeType rowIndex0, SizeType rowIndex1);
        void multipleRow(MultipleNumberType multipleNumber, SizeType rowIndex);
        void addOneMultipleRowToAnotherRow(SizeType rowIndex0, MultipleNumberType multipleNumber, SizeType rowIndex1);
        
        void switchTwoColum(SizeType columIndex0, SizeType columIndex1);
        void multipleColum(MultipleNumberType multipleNumber, SizeType columIndex);
        void addOneMultipleColumToAnotherColum(SizeType columIndex0, MultipleNumberType multipleNumber, SizeType columindex1);
        
        inline const Matrix inverseOfSquareMatrix() {
            
            assert(this->isEmpty() == false);
            assert(this->isSquareMatrix() == true);
 
            Matrix matrixCopy(*this);
            Matrix result = Matrix::unitaryMatrix(this->getRowNumber());
            
            for (size_t rowIndex = 0; rowIndex < matrixCopy.getRowNumber(); ++rowIndex) {
                
                SizeType firstRowNonZeroInColum = matrixCopy.firstRowContainsNonZeroElementColumUpRow(rowIndex, rowIndex);
                
                //Matrix is singular matrix because its rank isn't equals its order.
                if (firstRowNonZeroInColum == matrixCopy.getRowNumber()) {
                    result = Matrix::emptyMatrix();
                    break;
                }
                
                result.switchTwoRow(rowIndex, firstRowNonZeroInColum);
                matrixCopy.switchTwoRow(rowIndex, firstRowNonZeroInColum);
                
                result.multipleRow(1.0 / matrixCopy.data.at(rowIndex).at(rowIndex), rowIndex);
                matrixCopy.multipleRow(1.0 / matrixCopy.data.at(rowIndex).at(rowIndex), rowIndex);
                
                for (size_t eliminationIndex = 0; eliminationIndex < matrixCopy.getRowNumber(); ++eliminationIndex) {
                    
                    if (eliminationIndex == rowIndex) {
                        continue;
                    }
                    
                    result.addOneMultipleRowToAnotherRow(rowIndex, (-1) * matrixCopy.data.at(eliminationIndex).at(rowIndex), eliminationIndex);
                    matrixCopy.addOneMultipleRowToAnotherRow(rowIndex, (-1) * matrixCopy.data.at(eliminationIndex).at(rowIndex), eliminationIndex);
                }
            }
            
            return Matrix(result);
        }
        
        RankValueType getRank();
        TraceValueType getTrace();
        
        static inline const Matrix unitaryMatrix(SizeType size) {
            
            Matrix result(size, size);
            
            for (SizeType index = 0; index < size; ++index) {
                result.data.at(index).at(index) = 1;
            }
            
            return Matrix(result);
        }
        
        static inline const Matrix emptyMatrix() {
            
            return Matrix();
        }
        
    private:
        vector< vector<ElementType> > data;
        
        SizeType firstRowContainsNonZeroElementColumUpRow(SizeType columIndex, SizeType upRowIndex);
        SizeType firstColumNonZeroElementUpRow(SizeType upRowIndex);
    };
    
}

namespace std {
    template<>
    void swap<std::Matrix>(std::Matrix& a, std::Matrix& b);
}

#endif