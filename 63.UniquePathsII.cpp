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
    int uniquePathsWithObstacles(Grid &obstacleGrid) {
	  // assume m and n is greater than 0
	  std::size_t m = obstacleGrid.size();
	  if(m == 0) return 0;

	  std::size_t n = obstacleGrid[0].size();
	  if(n == 0) return 0;

	  TwoDArray arr(m, n);

	  arr[0][0] = !obstacleGrid[0][0];
	  for(auto i = 1u; i < m; i++) {
		if(obstacleGrid[i][0]) {
		  arr[i][0] = 0;
		} else {
		  arr[i][0] = arr[i - 1][0];
		}
	  }

	  for(auto i = 1u; i < n; i++) {
		if(obstacleGrid[0][i]) {
		  arr[0][i] = 0;
		} else {
		  arr[0][i] = arr[0][i - 1];
		}
	  }

	  for(auto i = 1u; i < m; i++) {
		for(auto j = 1u; j < n; j++) {
		  if(obstacleGrid[i][j]) {
			arr[i][j] = 0;
		  } else {
			arr[i][j] = arr[i - 1][j] + arr[i][j - 1];
		  }
		}
	  }

	  return arr[m - 1][n - 1];
    }
};


int main() {
  Solution sol;

  std::vector<Grid> input = {
	{ { 0, 0, 0},
	  { 0, 1, 0},
	  { 0, 0, 0}, },
	{ { 1 }, },
	{ { 0 }, },
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.uniquePathsWithObstacles(*it) << "\n";
  }
  return 0;
}
