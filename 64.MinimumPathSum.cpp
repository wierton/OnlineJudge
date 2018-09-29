#include <stack>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>

class TwoDArray {
  const std::size_t m, n;
  std::size_t *p;

public:
  TwoDArray(std::size_t m, std::size_t n) :
	m(m), n(n)
  {
	p = new std::remove_reference<decltype(*p)>::type [m * n];
  }

  std::size_t *operator[](std::size_t idx) const {
	return p + idx * n;
  }

  ~TwoDArray() {
	delete []p;
  }
};

using Grid = std::vector<std::vector<int>>;
class Solution {
  public:
    int minPathSum(Grid &obstacleGrid) {
	  // assume m and n is greater than 0
	  std::size_t m = obstacleGrid.size();
	  if(m == 0) return 0;

	  std::size_t n = obstacleGrid[0].size();
	  if(n == 0) return 0;

	  TwoDArray arr(m, n);

	  arr[0][0] = obstacleGrid[0][0];
	  for(auto i = 1u; i < m; i++) {
		arr[i][0] = obstacleGrid[i][0] + arr[i - 1][0];
	  }

	  for(auto i = 1u; i < n; i++) {
		arr[0][i] = obstacleGrid[0][i] + arr[0][i - 1];
	  }

	  for(auto i = 1u; i < m; i++) {
		for(auto j = 1u; j < n; j++) {
		  arr[i][j] = obstacleGrid[i][j] + std::min(arr[i - 1][j], arr[i][j - 1]);
		}
	  }

	  return arr[m - 1][n - 1];
    }
};


int main() {
  Solution sol;

  std::vector<Grid> input = {
	{ {1,3,1},
	  {1,5,1},
	  {4,2,1}, },
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.minPathSum(*it) << "\n";
  }
  return 0;
}
