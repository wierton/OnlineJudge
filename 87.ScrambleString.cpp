#include <stack>
#include <array>
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <utility>

// #define DEBUG

template<std::size_t D, class T> class NDArray;

template<std::size_t D, class T>
struct NDElementAt {
  static NDArray<D - 1, T> apply(NDArray<D, T> *arr, std::size_t idx) {
	std::size_t size = std::accumulate(&arr->shape[1],
		&arr->shape[D], 1u, std::multiplies<T>());
	return  { &(arr->shape[1]), arr->data + idx * size };
  }
};

template<class T>
struct NDElementAt<2, T> {
  static T *apply(NDArray<2, T> *arr, std::size_t idx) {
	return arr->data + idx * arr->shape[1];
  }
};

template<class T>
struct NDElementAt<1, T> {
  static T &apply(NDArray<1, T> *arr, std::size_t idx) {
	return arr->data[idx];
  }
};

template<class T, class First, class...Args>
struct NDCompatibilityChecker {
  enum {
	value = std::is_convertible<First, T>::value && NDCompatibilityChecker<T, Args...>::value
  };
};

template<class T, class First>
struct NDCompatibilityChecker<T, First> {
  enum {
	value = std::is_convertible<First, T>::value
  };
};

/* n-dim array */
template<std::size_t D, class T=std::size_t>
class NDArray {
  using SizeT = std::size_t;
  using ShapeT = SizeT[D];

  const SizeT *shape;
  enum {
	ND_RAW = 0,
	ND_REF = 1,
	ND_CONST = 2,
  } attr;
  T *data;

  NDArray(const SizeT *shape, T *data) :
	shape(shape), attr(ND_REF), data(data)
  {
  }

  template<std::size_t I, class U> friend class NDElementAt;

  bool is_raw() const { return attr == ND_RAW; }
  bool is_ref() const { return static_cast<int>(attr) & ND_REF; }

public:
  template<class...Args,
	class=typename std::enable_if<NDCompatibilityChecker<T, Args...>::value, void>::type
  > explicit NDArray(Args&&...args) :
	shape(new ShapeT {std::forward<Args>(args)...}),
	attr(ND_RAW)
  {
	SizeT size = std::accumulate(&shape[0], &shape[D],
		1u, std::multiplies<T>());
	data = new T [size];
  }

  // basic type or RVO (dependent on implicit type conversion)
  decltype(auto) operator[](SizeT idx) {
	return NDElementAt<D, T>::apply(this, idx);
  }

  ~NDArray() {
	if(is_raw()) {
	  delete []shape;
	  delete []data;
	}
  }
};

using TwoDArray = NDArray<2>;
using ThreeDArray = NDArray<3>;

class Solution {
public:
  bool isScramble(std::string &word1, std::string &word2) {
	std::size_t m = word1.size(), n = word2.size();
	if(m != n) return false;
	if(m == 0 || n == 0) return true;
	std::size_t len = m;

	ThreeDArray arr(m, n, len);

	for(auto k = 0u; k < len; k++) {
	  for(auto i = 0u; i < len; i++) {
		for(auto j = 0u; j < len; j++) {
		  arr[i][j][0] = word1[i] == word2[j];
		}
	  }
	}

	for(auto k = 1u; k < len; k++) {
	  for(auto i = 0u; i + k < len; i++) {
		for(auto j = 0u; j + k < len; j++) {
		  arr[i][j][k] = false;
		  for(auto t = 0u; t < k; t++) {
#ifdef DEBUG
			printf("k = %d, t = %d\n", k, t);
			printf("arr[%d, %d, %d]=%d && arr[%d, %d, %d]=%d\n",
				i, j, t, arr[i][j][t],
				i + t + 1, j + t + 1, k - t - 1,
				arr[i + t + 1][j + t + 1][k - t - 1]
				);
			printf("arr[%d, %d, %d]=%d && arr[%d, %d, %d]=%d\n",
			  i, j + k - t, t,
			  arr[i][j + k - t][t],
			  i + t + 1, j, k - t - 1,
			  arr[i + t + 1][j][k - t - 1]
			);
#endif
			if(
			  (arr[i][j][t] && arr[i + t + 1][j + t + 1][k - t - 1]) ||
			  (arr[i][j + k - t][t] && arr[i + t + 1][j][k - t - 1])
			) {
			  arr[i][j][k] = true;
			  break;
			}
		  }
#ifdef DEBUG
		  printf("----> arr[%d, %d, %d] = %d\n", i, j, k, arr[i][j][k]);
#endif
		}
	  }
	}

	/*
	for(auto k = 0u; k < len; k++) {
	  for(auto i = 0u; i < len; i++) {
		for(auto j = 0u; j < len; j++) {
		  printf("%d, %d, %d: %d\n", i, j, k, arr[i][j][k]);
		}
	  }
	}
	*/

	return arr[0][0][len - 1];
  }
};

int main() {
  Solution sol;

  std::vector<std::pair<std::string, std::string>> input = {
	{ "great", "rgeat" },
	{ "abcde", "caebd" },
	{ "aa", "ab" },
	{ "abc", "bca" },
  };

  for(auto it = input.begin(); it != input.end(); ++it) {
	std::cout << sol.isScramble(it->first, it->second) << "\n";
  }
  return 0;
}
