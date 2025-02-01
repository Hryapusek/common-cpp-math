#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <cpp-math/cpp_math.h>

using cpp_math::operator<<;

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
    SECTION("90 degrees")
    {
      auto expected = cpp_math::Vector3d(0, 1, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 90);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 90 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }

    SECTION("180 degrees")
    {
      auto expected = cpp_math::Vector3d(-1, 0, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 180);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 180 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }

    SECTION("270 degrees")
    {
      auto expected = cpp_math::Vector3d(0, -1, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 270);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 270 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }

    SECTION("360 degrees")
    {
      auto expected = cpp_math::Vector3d(1, 0, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 360);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 360 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }

    SECTION("-90 degrees")
    {
      auto expected = cpp_math::Vector3d(0, -1, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, -90);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 270 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }

    SECTION("45 degrees")
    {
      auto expected = cpp_math::Vector3d(std::sqrt(2) / 2, std::sqrt(2) / 2, 0);
      auto result = cpp_math::rotateVector(x_vector, cpp_math::Axis::Z, 45);
      INFO("Souce vector is " << x_vector);
      INFO("Vector after rotaion along z with 45 degrees is " << result);
      REQUIRE(vectors_almost_equal(result, expected, 0.0001));
    }
  }
}
