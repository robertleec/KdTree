#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <vector>

namespace Math {
    
    using namespace std;
    
    double variance(const vector<double>& values);
    size_t maxValueIndex(const vector<double>& values);
}

#endif