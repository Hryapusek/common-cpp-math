#include <cpp-math/cpp_math.h>

// #include <iostream>
#include <memory>
#include <limits>
#include <stdexcept>
#include <cstdio>  // For size_t
#include <cmath>

namespace
{
  using namespace cpp_math;

  double getAngle(HeliAngles const& angles, HeliAngle angle)
  {
    switch(angle) {
      case HeliAngle::Yaw: return angles.yaw;
      case HeliAngle::Pitch: return angles.pitch;
      case HeliAngle::Roll: return angles.roll;
    }
    throw std::runtime_error("Unknown heli angle: " + std::to_string(static_cast<int>(angle)));
  }

  std::string angleToString(HeliAngle angle)
  {
    switch(angle) {
      case HeliAngle::Yaw: return "yaw";
      case HeliAngle::Pitch: return "pitch";
      case HeliAngle::Roll: return "roll";
    }
    throw std::runtime_error("Unknown heli angle: " + std::to_string(static_cast<int>(angle)));
  }

  [[maybe_unused]] std::string anglesToString(std::vector<HeliAngle> const& angles)
  {
    std::string result = "{";
    for(auto angle : angles) {
      result += angleToString(angle) + ", ";
    }
    result += "}";
    return result;
  }

  bool close_to_zero(double value)
  {
    return std::abs(value) < std::numeric_limits<double>::epsilon() * 100;
  }

  bool can_rotate(Vector3d const& v, HeliAngle const& angle)
  {
    if(angle == HeliAngle::Yaw) {
      return not close_to_zero(v.x) or not close_to_zero(v.y);
    }
    else if(angle == HeliAngle::Pitch) {
      return not close_to_zero(v.x) or not close_to_zero(v.z);
    }
    else if(angle == HeliAngle::Roll) {
      return not close_to_zero(v.z) or not close_to_zero(v.y);
    }
    throw std::runtime_error("Unknown heli angle: " + std::to_string(static_cast<int>(angle)));
  }

  std::unique_ptr<Vector3d> try_to_rotate(
    Vector3d const& v,
    HeliAngles const& angles,
    std::vector<HeliAngle> const& angles_to_rotate
  )
  {
    auto result = v;
    // std::cout << "Trying rotation: " << anglesToString(angles_to_rotate) << std::endl;

    for(auto angle : angles_to_rotate) {
      if (close_to_zero(getAngle(angles, angle))) {
        continue;
      }
      if(not can_rotate(result, angle)) {
        // std::cout << "Can't rotate " << angleToString(angle) << std::endl;
        return {};
      }
      result = rotateVector(result, heliAngleToRotationAxis(angle), getAngle(angles, angle));
    }

    return std::make_unique<Vector3d>(result);
  }


  std::vector<std::vector<HeliAngle>> get_angles_permutations()
  {
    return {
      {HeliAngle::Roll, HeliAngle::Pitch, HeliAngle::Yaw},
      {HeliAngle::Roll, HeliAngle::Yaw, HeliAngle::Pitch},
      {HeliAngle::Yaw, HeliAngle::Roll, HeliAngle::Pitch},
      {HeliAngle::Yaw, HeliAngle::Pitch, HeliAngle::Roll},
      {HeliAngle::Pitch, HeliAngle::Roll, HeliAngle::Yaw},
      {HeliAngle::Pitch, HeliAngle::Yaw, HeliAngle::Roll},
    };
  } 

  std::unique_ptr<Vector3d> try_to_rotate(Vector3d const& v, HeliAngles const& angles)
  {
    for (auto angles_triplet : get_angles_permutations()) {
      auto result = try_to_rotate(v, angles, angles_triplet);
      if(result) {
        return result;
      }
    }

    return {};
  }

}  // namespace

namespace cpp_math
{
  Vector3d calculatePointByDistanceAndAngles(
    double distance,
    Vector3d initial_position,
    HeliAngles angles,
    CameraAngles camera_angles
  )
  {
    Vector3d normalizedVector = Vector3d{1, 0, 0};
    angles.pitch += camera_angles.pitch;
    angles.yaw += camera_angles.yaw;
    // std::cout << "Result heli angles: " << angles << std::endl;
    auto result = rotateVector(normalizedVector, angles);
    normalizedVector = result;
    // std::cout << "Normalized vector: " << normalizedVector << std::endl;

    return addVectors(initial_position, multiplyVectorByScalar(normalizedVector, distance));
  }

  Vector3d rotateVector(Vector3d const& v, HeliAngles const& angles)
  {
    if(angles.roll == 0 && angles.pitch == 0 && angles.yaw == 0) {
      return v;
    }
    auto result = try_to_rotate(v, angles);
    if(not result) {
      return v;
    }
    return *result;
  }

  Axis heliAngleToRotationAxis(HeliAngle angle)
  {
    switch(angle) {
      case HeliAngle::Roll: return Axis::X;
      case HeliAngle::Pitch: return Axis::Y;
      case HeliAngle::Yaw: return Axis::Z;
    }
    throw std::runtime_error("Unknown heli angle: " + std::to_string(static_cast<int>(angle)));
  }

  Vector3d rotateVector(Vector3d const& v, Axis axis, double angle)
  {
    auto radians = degreesToRadians(angle);
    auto rotation_matrix = calculateRotationMatrix(axis, radians);
    return multiplyMatrixByVector(rotation_matrix, v);
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
    return Vector3d{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
  }

  Vector3d subtractVectors(Vector3d const& v1, Vector3d const& v2)
  {
    return Vector3d{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
  }

  Vector3d multiplyVectorByScalar(Vector3d const& v, double scalar)
  {
    return Vector3d{v.x * scalar, v.y * scalar, v.z * scalar};
  }

  Vector3d multiplyMatrixByVector(Matrix3d const& matrix, Vector3d const& v)
  {
    if(matrix.size() != 3 || matrix[0].size() != 3) {
      throw std::runtime_error("Matrix must be 3x3");
    }

    Vector3d result;
    result.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z;
    result.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z;
    result.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z;

    return result;
  }

  Matrix3d multiplyMatrices(Matrix3d const& m1, Matrix3d const& m2)
  {
    if(m1.size() != 3 || m1[0].size() != 3 || m2.size() != 3 || m2[0].size() != 3) {
      throw std::runtime_error("Matrices must be 3x3");
    }

    // std::cout << "Multiplying matrices\n";
    // std::cout << m1 << "\n";
    // std::cout << m2 << "\n";

    Matrix3d result = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    for(size_t row = 0; row < 3; ++row) {
      for(size_t col = 0; col < 3; ++col) {
        double value = 0;
        for(size_t i = 0; i < 3; ++i) {
          value += m1[row][i] * m2[i][col];
        }
        result[row][col] = value;
      }
    }

    // std::cout << "Result:\n";
    // std::cout << result << "\n";

    return result;
  }

  std::ostream& operator<<(std::ostream& os, Vector3d const& v)
  {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }

  std::ostream& operator<<(std::ostream& os, Matrix3d const& matrix)
  {
    for(size_t row = 0; row < matrix.size(); ++row) {
      os << "(" << matrix[row][0] << ", " << matrix[row][1] << ", " << matrix[row][2] << ")"
         << std::endl;
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, HeliAngles const& angles)
  {
    return os << "Roll: " << angles.roll << ", Pitch: " << angles.pitch << ", Yaw: " << angles.yaw;
  }

  std::ostream& operator<<(std::ostream& os, CameraAngles const& angles)
  {
    return os << "Pitch: " << angles.pitch << ", Yaw: " << angles.yaw;
  }

}  // namespace cpp_math
