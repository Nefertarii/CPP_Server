#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>

template <typename Object>
class Matrix
{
private:
    std::vector<std::vector<Object>> array;

public:
    Matrix(int rows, int cols) : array(rows)
    {
        for (auto &Row : array)
            Row.resize(cols);
    }
    Matrix(std::vector<std::vector<Object>> v) : array(v) {}
    Matrix(std::vector<std::vector<Object>> &&v) : array(std::move(v)) {}
    const std::vector<Object> &operator[](int row) const { return array[row]; }
    std::vector<Object> &operator[](int row) { return array[row]; }
    int numrows() const { return array.size(); }
    int numcols() const { return numrows() ? array[0].size() : 0; }
};

#endif