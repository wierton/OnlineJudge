#include <stack>
#include <array>
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <utility>
#include <cmath>


template<class T>
struct DataHolder { };

template<size_t...I>
struct DataHolder<std::index_sequence<I...>> {
  uint8_t data[sizeof...(I)];

  template<unsigned X, unsigned N>
  static constexpr uint64_t Or(std::array<uint64_t, N> arr) {
	if constexpr (X < N) {
	  return std::get<X>(arr) | Or<X + 1, N>(arr);
	} else {
	  return 0;
	}
  }

public:
  constexpr DataHolder(int64_t val) :
	data { static_cast<uint8_t>((val >> (I << 3)) & 0xFF)... }
  { }

  constexpr uint64_t asUInt() const {
	return Or<0, sizeof...(I)>({((uint64_t)data[I] << (I << 3))...});
  }
};

template<unsigned N>
class UInt {
  DataHolder<std::make_index_sequence<N>> holder;

  using result_type = typename std::conditional<N < 4, int,
		typename std::conditional<N == 4, uint32_t, int64_t>::type
	>::type;

public:
  constexpr uint64_t asUInt() const
  { return holder.asUInt(); }

  constexpr UInt(int64_t val) : holder(val) { }

  UInt(const UInt &that) = default;
  UInt &operator=(const UInt &that) = default;
  UInt &operator=(int64_t val) { return *this = UInt(val); }

  // obey usual arithmetic conversion rule
  operator result_type() const { return asUInt(); }
} __attribute__((aligned(1)));

using uint24_t = UInt<3>;
using uint40_t = UInt<5>;
using uint48_t = UInt<6>;
using uint56_t = UInt<7>;


int main() {
  constexpr uint24_t a = 0x1234567;
  constexpr uint24_t const_a = a.asUInt();
  uint24_t b = a;
  b = a;
  std::cout << std::hex;
  std::cout << a << " " << b << " " << const_a << std::endl;
  b = a + 1;
  std::cout << b + 1 << " " << a.asUInt() << std::endl;

  uint24_t _cmp = 1;
  std::cout << (-1 < _cmp) << std::endl; // act as uint16_t
  return 0;
}
