#include "Matrix.h"
#include <iostream>

namespace std {
    
    Matrix::Matrix() {
        
    }
    
    Matrix::Matrix(const Matrix& rhs):data(rhs.data) {
        
    }
    
    Matrix& Matrix::operator=(const Matrix& rhs) {
        Matrix temp(rhs);
        this->swap(temp);
        return *this;
    }
    
    void Matrix::swap(Matrix& other) {
        using std::swap;
        swap(this->data, other.data);
    }
    
    Matrix::Matrix(SizeType rowNumber, SizeType columNumber) {
        
        if (rowNumber == 0 || columNumber == 0) {
            return;
        }
        
        for (size_t index = 0; index < rowNumber; ++index) {
            vector<ElementType> row(columNumber);
            this->data.push_back(row);
        }
    }
    
    Matrix::~Matrix() {
        
    }
    
    void Matrix::addRow(const vector<ElementType>& rowData) {
        
        if (this->getRowNumber() == 0) {
            
            this->data.push_back(rowData);
            
        } else {
            
            assert(rowData.size() == this->getColumNumber());
            this->data.push_back(rowData);
        }
    }
    
    void Matrix::deleteRow(SizeType rowIndex) {
        
        assert(this->getRowNumber() > rowIndex);
        
        vector< vector<ElementType> >::iterator deleteIterator = this->data.begin() + rowIndex;
        
        this->data.erase(deleteIterator);
    }
    
    void Matrix::addColum(const vector<ElementType>& columData) {
        
        if (this->getRowNumber() == 0) {
            for (size_t index = 0; index < columData.size(); ++index) {
                vector<ElementType> row(1, columData.at(index));
                this->data.push_back(row);
            }
        } else {
            
            assert(columData.size() == this->getRowNumber());
            
            for (size_t index = 0; index < columData.size(); ++index) {
                this->data.at(index).push_back(columData.at(index));
            }
        }
    }
    
    void Matrix::deleteColum(SizeType columIndex) {
        
        assert(this->getColumNumber() > columIndex);
        
        if (this->getColumNumber() == 1 && columIndex == 0) {
            this->data.clear();
        } else {
        
            for (size_t index = 0; index < this->getRowNumber(); ++index) {
                vector<ElementType>& row = this->data.at(index);
                vector<ElementType>::iterator deleteIterator = row.begin() + columIndex;
                row.erase(deleteIterator);
            }
        }
    }
    
    bool Matrix::isEmpty() const {
        
        if (this->getRowNumber() > 0 && this->getColumNumber() > 0) {
            return false;
        }
        
        return true;
    }
    
    bool Matrix::isSquareMatrix() const {
        
        if (this->getRowNumber() == this->getColumNumber()) {
            return true;
        }
        
        return false;
    }
    
    void Matrix::switchTwoRow(SizeType rowIndex0, SizeType rowIndex1) {
        
        assert(this->getRowNumber() > rowIndex0 && this->getRowNumber() > rowIndex1);
        
        using std::swap;
        swap(this->data.at(rowIndex0), this->data.at(rowIndex1));
    }
    
    void Matrix::multipleRow(MultipleNumberType multipleNumber, SizeType rowIndex) {
        
        assert(this->getRowNumber() > rowIndex);
        
        vector<ElementType>& row = this->data.at(rowIndex);
        
        for (size_t index = 0; index < row.size(); ++index) {
            row.at(index) *= multipleNumber;
        }
    }
    
    void Matrix::addOneMultipleRowToAnotherRow(SizeType rowIndex0, MultipleNumberType multipleNumber, SizeType rowIndex1) {
        
        assert(this->getRowNumber() > rowIndex0 && this->getRowNumber() > rowIndex1);
        
        const vector<ElementType>& row0 = this->data.at(rowIndex0);
        vector<ElementType>& row1 = this->data.at(rowIndex1);
        
        for (size_t index = 0; index < row0.size(); ++index) {
            row1.at(index) += row0.at(index) * multipleNumber;
        }
    }
    
    void Matrix::switchTwoColum(SizeType columIndex0, SizeType columIndex1) {
        
        assert(this->getColumNumber() > columIndex0 && this->getColumNumber() > columIndex1);
        
        using std::swap;
        
        for (size_t index = 0; index < this->getRowNumber(); ++index) {
            
            vector<ElementType>& row = this->data.at(index);
            swap(row.at(columIndex0), row.at(columIndex1));
        }
    }
    
    void Matrix::multipleColum(MultipleNumberType multipleNumber, SizeType columIndex) {
        
        assert(this->getColumNumber() > columIndex);
        
        for (size_t index = 0; index < this->getRowNumber(); ++index) {
            
            vector<ElementType>& row = this->data.at(index);
            row.at(columIndex) *= multipleNumber;
        }
    }
    
    void Matrix::addOneMultipleColumToAnotherColum(SizeType columIndex0, MultipleNumberType multipleNumber, SizeType columIndex1) {
        
        assert(this->getColumNumber() > columIndex0 && this->getColumNumber() > columIndex1);
        
        for (size_t index = 0; index < this->getRowNumber(); ++index) {
            
            vector<ElementType>& row = this->data.at(index);
            
            row.at(columIndex1) += row.at(columIndex0) * multipleNumber;
        }
        
    }
    
    SizeType Matrix::firstRowContainsNonZeroElementColumUpRow(SizeType columIndex, SizeType upRowIndex) {
        
        assert(this->getColumNumber() > columIndex);
        assert(this->getRowNumber() > upRowIndex);
        
        SizeType firstRowIndex = this->getRowNumber();
        
        for (size_t rowIndex = upRowIndex; rowIndex < this->getRowNumber(); ++rowIndex) {
            
            if (this->data.at(rowIndex).at(columIndex) != 0) {
                firstRowIndex = rowIndex;
                break;
            }
        }
        
        return firstRowIndex;
    }
    
    SizeType Matrix::firstColumNonZeroElementUpRow(SizeType upRowIndex) {
        
        assert(this->getRowNumber() > upRowIndex);
        
        SizeType firstColumIndex = this->getColumNumber();
        
        for (size_t rowIndex = upRowIndex; rowIndex < this->getRowNumber(); ++rowIndex) {
            
            vector<ElementType>& row = this->data.at(rowIndex);
            
            for (size_t columIndex = 0; columIndex < row.size(); ++columIndex) {
                
                if (row.at(columIndex) != 0 && firstColumIndex > columIndex) {
                    firstColumIndex = columIndex;
                    break;
                }
            }
        }
        
        return firstColumIndex;
    }
    
    RankValueType Matrix::getRank() {
        
        Matrix matrixCopy(*this);
        
        RankValueType rank = 0;
        
        for (size_t rowIndex = 0; rowIndex < matrixCopy.getRowNumber(); ++rowIndex) {
            
            SizeType firstColumNonZero = matrixCopy.firstColumNonZeroElementUpRow(rowIndex);
            
            //Not found non-zero element up row index.
            if (firstColumNonZero == matrixCopy.getColumNumber()) {
                break;
            }
            
            ++rank;
            
            SizeType firstRowNonZeroInColum = matrixCopy.firstRowContainsNonZeroElementColumUpRow(firstColumNonZero, rowIndex);

            matrixCopy.switchTwoRow(rowIndex, firstRowNonZeroInColum);
            
            for (size_t eliminationIndex = rowIndex + 1; eliminationIndex < matrixCopy.getRowNumber(); ++eliminationIndex) {
                
                matrixCopy.addOneMultipleRowToAnotherRow(rowIndex, (-1) * (matrixCopy.data.at(eliminationIndex).at(firstColumNonZero) / matrixCopy.data.at(rowIndex).at(firstColumNonZero)), eliminationIndex);
            }
        }
        
        return rank;
    }
    
    TraceValueType Matrix::getTrace() {
        
        assert(this->isSquareMatrix() == true);
        
        TraceValueType trace = 0;
        
        for (size_t rowIndex = 0; rowIndex < this->getRowNumber(); ++rowIndex) {
            trace += this->data.at(rowIndex).at(rowIndex);
        }
        
        return trace;
    }
}

namespace std {
    template<>
    void swap<std::Matrix>(std::Matrix&a, std::Matrix& b) {
        a.swap(b);
    }
}