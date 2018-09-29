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

class Solution {
  public:
  int minDistance(std::string &word1, std::string &word2) {
	std::size_t m = word1.size(), n = word2.size();
	if(m == 0) return n;
	if(n == 0) return m;

	TwoDArray arr(m, n);

	arr[0][0] = word1[0] != word2[0];

	for(auto i = 1u; i < m; i++)
	  if(word1[i] == word2[0]) {
		arr[i][0] = i;
	  } else {
		arr[i][0] = 1 + arr[i - 1][0];
	  }

	for(auto i = 1u; i < n; i++)
	  if(word1[0] == word2[i]) {
		arr[0][i] = i;
	  } else {
		arr[0][i] = 1 + arr[0][i - 1];
	  }

	for(auto i = 1u; i < m; i++) {
	  for(auto j = 1u; j < n; j++) {
		if(word1[i] == word2[j]) {
		  arr[i][j] = arr[i - 1][j - 1];
		} else {
		  arr[i][j] = 1 + std::min(
			  std::min(arr[i - 1][j], arr[i][j - 1]),
			  arr[i - 1][j - 1]);
		}
	  }
	}
	return arr[m - 1][n - 1];
  }
};


int main() {
  Solution sol;

  std::vector<std::pair<std::string, std::string>> input = {
	{ "horse", "" },
	{ "", "ros" },
	{ "a", "ab" },
	{ "horse", "ros" },
	{ "intention", "execution" },
	{ "pneumonoultramicroscopicsilicovolcanoconiosis",
	  "ultramicroscopically" }
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.minDistance(it->first, it->second) << "\n";
  }
  return 0;
}
