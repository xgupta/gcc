// { dg-options "-std=gnu++17" }
// { dg-do run { target *-*-* } }
// { dg-require-effective-target c++17 }
// { dg-additional-options "-march=x86-64-v4" { target avx512f } }
// { dg-require-cmath "" }

#include <experimental/simd>

namespace stdx = std::experimental;

using T = std::uint64_t;

template <typename U, int N>
using V = stdx::simd<U, stdx::simd_abi::deduce_t<U, N>>;

[[gnu::noinline, gnu::noipa]]
int reduce(V<T, 4> x)
{
  static_assert(stdx::find_last_set(V<T, 4>([](unsigned i) { return i; }) != V<T, 4>(0)) == 3);
  return stdx::find_last_set(x != -1);
}

[[gnu::noinline, gnu::noipa]]
int reduce2()
{
  using M8 = typename V<short, 8>::mask_type;
  using M4 = typename V<int, 4>::mask_type;
  if constexpr (sizeof(M8) == sizeof(M4))
    {
      M4 k;
      __builtin_memcpy(&__data(k), &__data(M8(true)), sizeof(M4));
      return stdx::find_last_set(k);
    }
  return 3;
}


int main()
{
  const V<T, 4> x {};

  const int r = reduce(x);
  if (r != 3)
      __builtin_abort();

  const int r2 = reduce2();
  if (r2 != 3)
      __builtin_abort();
}
