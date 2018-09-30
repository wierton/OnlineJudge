#include <stack>
#include <array>
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <utility>

// #define DEBUG
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

using TwoDArray = NDArray<2>;
using ThreeDArray = NDArray<3>;

class Solution {
public:
  vector<TreeNode*> generateTrees(int n) {
  }
};

std::iostream &operator<<(std::iostream &os, TreeNode *tree) {
  if(tree)
	os << tree->val << ",";
  else
	os << "null,";
}


int main() {
  Solution sol;

  std::vector<std::size_t> input = {
	3
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.generateTrees(*it) << "\n";
  }
  return 0;
}
