#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class solution {
  public:
    int maxSumSubmatrix(const vector<vector<int>> & matrix, int k) {
        
    }
};

class interval {
  public:
    size_t start, end;
    interval(size_t s, size_t e) : start(s), end(e){}
    interval(){}
    interval(const interval & other) {
        if (this != &other) {
            start = other.start;
            end = other.end;
        }
    }

    bool overlap(const interval & other, interval & result) {
        size_t x1 = std::max(start, other.start), x2 = std::min(end, other.end);
        if (x1 <= x2) {
            result.start = x1;
            result.end = x2;
            return true;
        }
        else {
            return false;
        }
    }
};

class ColNode {
  public:
    interval intv;
    shared_ptr<ColNode> left, right;
    ColNode(size_t colstart, size_t colend)
        : intv(colstart, colend)
    {
        if (colstart < colend) {
            size_t mid = (colstart+colend)/2;
            left = make_shared<ColNode>(colstart, mid);
            right = make_shared<ColNode>(mid+1, colend);
        }
        else {
            left = nullptr;
            right = nullptr;
        }
    }
};

class RowNode {
  public:
    interval intv;
    shared_ptr<ColNode> colroot;
    shared_ptr<RowNode> left, right;
    RowNode(size_t rowstart, size_t colstart, size_t rowend, size_t colend)
        : intv(rowstart, rowend)
    {
        colroot = make_shared<ColNode>(colstart, colend);
        if (rowstart < rowend) {
            size_t mid = (rowstart+rowend)/2;
            left = make_shared<RowNode>(rowstart, colstart, mid, colend);
            right = make_shared<RowNode>(mid+1, colstart, rowend, colend);
        }
        else {
            left = nullptr;
            right = nullptr;    
        }
    }
};

class matrix {
  private:
    vector<vector<int>> mat;
    shared_ptr<RowNode> root;
    unordered_map<interval,unordered_map<interval, int>> results;

  public:
    matrix(const vector<vector<int>> & a_mat) : mat(a_mat)
    {
        root = make_shared<RowNode>(0, mat.size()-1, 0, mat[0].size()-1);
    }

  private:
    int rangeSum(size_t row1, size_t col1, size_t row2, size_t col2) {
        interval rintv(row1, row2), cintv(col1, col2);
        
    }

    int rangeSum_impl(size_t row1, size_t col1, size_t row2, size_t col2) {
        vector<int> rowsum;
        for (size_t i = row1; i <= row2; ++i)
            rowsum.push_back(std::accumulate(mat[i].begin()+col1, mat[i].begin()+col2+1, 0));
        return std::accumulate(rowsum.begin(), rowsum.end(), 0);
    }

    vector<shared_ptr<RowNode>> findRows(const interval & rintv) {
        vector<shared_ptr<RowNode>> result;
        queue<shared_ptr<RowNode>> qu;
        qu.push(root);
        while (!qu.empty()) {
            auto curr = qu.top();
            qu.pop();
            interval temp;
            bool isOverlapped = curr->intv.overlap(rintv, temp);
            if (isOverlapped) {
                
            }
        }
    }
};
