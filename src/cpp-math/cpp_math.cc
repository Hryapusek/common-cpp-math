
#include <stdexcept>
#include <cstdio>  // For size_t
#include <cmath>

#include <cpp-math/cpp_math.h>

namespace cpp_math
{
  Vector3d rotateVector(Vector3d const& v, Axis axis, double angle)
  {
    auto radians = degreesToRadians(angle);
    auto rotation_matrix = calculateRotationMatrix(axis, radians);
  }

  Matrix3d calculateRotationMatrix(Axis axis, double radians)
  {
    // clang-format off
    switch(axis) {
      case Axis::Z:
        return {
                {cos(radians), -sin(radians), 0             },
                {sin(radians),  cos(radians), 0             }, 
                {0,             0,            1             }
               };
      case Axis::X:
        return {
                {1,             0,             0            }, 
                {0,             cos(radians), -sin(radians) }, 
                {0,             sin(radians),  cos(radians) }
               };
      case Axis::Y:
        return {
                {cos(radians),  0,             sin(radians) }, 
                {0,             1,             0            }, 
                {-sin(radians), 0,             cos(radians) }
               };
    }
    // clang-format on
    throw std::runtime_error("Invalid axis");
  }

  double degreesToRadians(double degrees) { return degrees * M_PI / 180.0; }

  Vector3d addVectors(Vector3d const& v1, Vector3d const& v2)
  {
    return Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
  }

  Vector3d subtractVectors(Vector3d const& v1, Vector3d const& v2)
  {
    return Vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
  }

  Vector3d multiplyVectorByScalar(Vector3d const& v, double scalar)
  {
    return Vector3d(v.x * scalar, v.y * scalar, v.z * scalar);
  }

  Vector3d multiplyMatrixByVector(Matrix3d const& matrix, Vector3d const& v)
  {
    if(matrix.size() != 3 || matrix[0].size() != 3) {
      throw std::runtime_error("Matrix must be 3x3");
    }
    Vector3d result;
    for(size_t row = 0; row < matrix.size(); ++row) {
      result.x += matrix[row][0] * v.x;
      result.y += matrix[row][1] * v.y;
      result.z += matrix[row][2] * v.z;
    }
    return result;
  }
}  // namespace cpp_math
