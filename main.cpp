// #include <iostream>
// #include <vector>
// #include <queue>

// using namespace std;

// class solution {
//   public:
//     int maxSumSubmatrix(const vector<vector<int>> & matrix, int k) {
        
//     }
// };

// class interval {
//   public:
//     size_t start, end;
//     interval(size_t s, size_t e) : start(s), end(e){}
//     interval(){}
//     interval(const interval & other) {
//         if (this != &other) {
//             start = other.start;
//             end = other.end;
//         }
//     }

//     bool overlap(const interval & other, interval & result) {
//         size_t x1 = std::max(start, other.start), x2 = std::min(end, other.end);
//         if (x1 <= x2) {
//             result.start = x1;
//             result.end = x2;
//             return true;
//         }
//         else {
//             return false;
//         }
//     }
// };

// class ColNode {
//   public:
//     interval intv;
//     shared_ptr<ColNode> left, right;
//     ColNode(size_t colstart, size_t colend)
//         : intv(colstart, colend)
//     {
//         if (colstart < colend) {
//             size_t mid = (colstart+colend)/2;
//             left = make_shared<ColNode>(colstart, mid);
//             right = make_shared<ColNode>(mid+1, colend);
//         }
//         else {
//             left = nullptr;
//             right = nullptr;
//         }
//     }
// };

// class RowNode {
//   public:
//     interval intv;
//     shared_ptr<ColNode> colroot;
//     shared_ptr<RowNode> left, right;
//     RowNode(size_t rowstart, size_t colstart, size_t rowend, size_t colend)
//         : intv(rowstart, rowend)
//     {
//         colroot = make_shared<ColNode>(colstart, colend);
//         if (rowstart < rowend) {
//             size_t mid = (rowstart+rowend)/2;
//             left = make_shared<RowNode>(rowstart, colstart, mid, colend);
//             right = make_shared<RowNode>(mid+1, colstart, rowend, colend);
//         }
//         else {
//             left = nullptr;
//             right = nullptr;    
//         }
//     }
// };

// class matrix {
//   private:
//     vector<vector<int>> mat;
//     shared_ptr<RowNode> root;
//     unordered_map<interval,unordered_map<interval, int>> results;

//   public:
//     matrix(const vector<vector<int>> & a_mat) : mat(a_mat)
//     {
//         root = make_shared<RowNode>(0, mat.size()-1, 0, mat[0].size()-1);
//     }

//   private:
//     int rangeSum(size_t row1, size_t col1, size_t row2, size_t col2) {
//         interval rintv(row1, row2), cintv(col1, col2);
        
//     }

//     int rangeSum_impl(size_t row1, size_t col1, size_t row2, size_t col2) {
//         vector<int> rowsum;
//         for (size_t i = row1; i <= row2; ++i)
//             rowsum.push_back(std::accumulate(mat[i].begin()+col1, mat[i].begin()+col2+1, 0));
//         return std::accumulate(rowsum.begin(), rowsum.end(), 0);
//     }

//     vector<shared_ptr<RowNode>> findRows(const interval & rintv) {
//         vector<shared_ptr<RowNode>> result;
//         queue<shared_ptr<RowNode>> qu;
//         qu.push(root);
//         while (!qu.empty()) {
//             auto curr = qu.top();
//             qu.pop();
//             interval temp;
//             bool isOverlapped = curr->intv.overlap(rintv, temp);
//             if (isOverlapped) {
                
//             }
//         }
//     }
// };


#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <unordered_map>
#include <climits>

using std::vector;
using std::unordered_map;
using pos = std::pair<size_t,size_t>;
using coord = std::pair<pos,pos>;
using std::make_pair;

class pairHasher {
  public:
    size_t operator()(const pos & p) const {
        return std::hash<size_t>()(p.first)^std::hash<size_t>()(p.second);
    }
};

bool operator==(const pos & p1, const pos & p2) {
    return p1.first == p2.first && p1.second == p2.second;
}

class solution {
  public:
    int maxSumSubmatrix(const vector<vector<int>> & matrix, int k) {
        unordered_map<pos,int,pairHasher> dict;
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[0].size(); ++j) {
                auto pr = make_pair(i,j);
                dict[pr] = rangeSum(matrix, pr);
                std::cout << dict[pr] << " ";
            }
            std::cout << std::endl;
        }

        size_t nrows = matrix.size(), ncols = matrix[0].size();
        unordered_map<pos, unordered_map<pos, int, pairHasher>, pairHasher> cands;
        for (size_t i = 0; i < nrows; ++i) {
            for (size_t j = 0; j < ncols; ++j) {
                auto topleft = make_pair(i,j);
                unordered_map<pos, int, pairHasher> temp;
                for (size_t s = i; s < nrows; s++) {
                    for (size_t t = j; t < ncols; t++) {
                        auto botright = make_pair(s,t);
                        temp[botright] = rangeSum(dict, topleft, botright);
                    }
                }
                cands[topleft] = temp;
            }
        }

        int maxSum = INT_MIN;
        for (auto it = cands.begin(); it != cands.end(); ++it) {
            for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
                std::cout << "topleft = (" << it->first.first << "," << it->first.second << ") "
                          << "botright = (" << jt->first.first << "," << jt->first.second << ") "
                          << "= " << jt->second << std::endl;
                if (jt->second <= k && jt->second > maxSum) {
                    maxSum = jt->second;
                    // std::cout << "topleft = (" << it->first.first << "," << it->first.second << ") "
                    //           << "botright = (" << jt->first.first << "," << jt->first.second << ")\n";
                }
            }
        }

        return maxSum;
    }

  private:
    int rangeSum(const vector<vector<int>> & mat, const pos & loc) {
        vector<int> rowsum;
        for (size_t i = 0; i <= loc.first; ++i)
            rowsum.push_back(std::accumulate(mat[i].begin(), mat[i].begin()+loc.second+1, 0));
        return std::accumulate(rowsum.begin(), rowsum.end(), 0);
    }

    int rangeSum(unordered_map<pos,int,pairHasher> & dict, const pos & topleft, const pos & botright) {
        pos left_top;
        left_top.first = topleft.first > 0 ? topleft.first-1 : topleft.first;
        left_top.second = topleft.second > 0 ? topleft.second-1 : topleft.second;
        if (topleft.first > 0 && topleft.second > 0)
            return dict[botright]-dict[make_pair(botright.first, left_top.second)]-dict[make_pair(left_top.first, botright.second)]+dict[left_top];
        else if (topleft.first == 0 && topleft.second > 0) {
            return dict[botright]-dict[make_pair(botright.first, left_top.second)];
        }
        else if (topleft.first > 0 && topleft.second == 0) {
            return dict[botright]-dict[make_pair(left_top.first, botright.second)];
        }
        else {
            return dict[botright];
        }
    }
};

int main() {
    vector<vector<int>> matrix{
        vector<int>{1,0,-1},
            vector<int>{0,-2,3}
    };
    int k = 2;

    solution soln;
    int num = soln.maxSumSubmatrix(matrix, k);
    std::cout << "The max sum of submatrix that is less than " << k << " is:\n";
    std::cout << num << std::endl;
}
