#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <cpp-math/cpp_math.h>

bool vectors_almost_equal(
  cpp_math::Vector3d const& v1,
  cpp_math::Vector3d const& v2,
  double epsilon
)
{
  return std::abs(v1.x - v2.x) < epsilon && std::abs(v1.y - v2.y) < epsilon
      && std::abs(v1.z - v2.z) < epsilon;
}

TEST_CASE("Basic rotations")
{
  SECTION("Rotations along the z axis")
  {
    auto x_vector = cpp_math::Vector3d(1, 0, 0);
    auto expected = cpp_math::Vector3d(0, 1, 0);
    REQUIRE(vectors_almost_equal(
      cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 90),
      expected,
      0.0001
    ));
  }
}
