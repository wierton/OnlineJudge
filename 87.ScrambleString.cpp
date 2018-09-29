#include <stack>
#include <array>
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <utility>


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
  > NDArray(Args&&...args) :
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

using ThreeDArray = NDArray<3>;

int main() {
  Solution sol;

  ThreeDArray arr(2u, 3u, 4u);
  arr[0][1][2] = 1234567;
  std::cout << arr[0][1][2] << ", " << *(&arr[0][0][0] + 6) << std::endl;

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
