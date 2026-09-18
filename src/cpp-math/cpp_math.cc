#include <cpp-math/cpp_math.h>

#include <cmath>
#include <cstdio>  // For size_t
#include <stdexcept>

namespace
{
  using namespace cpp_math;

  Matrix3d calculateHeliRotationMatrix(HeliAngles const& angles)
  {
    // Heli/body coordinates -> world coordinates.
    //
    // For column vectors:
    //
    // world = Rz(yaw) * Ry(pitch) * Rx(roll) * body
    //
    // The right-most operation is applied first.
    // This is the usual intrinsic roll -> pitch -> yaw composition:
    // roll around heli X,
    // pitch around heli Y,
    // yaw around heli Z.

    auto const roll = calculateRotationMatrix(
      Axis::X,
      degreesToRadians(angles.roll)
    );

    auto const pitch = calculateRotationMatrix(
      Axis::Y,
      degreesToRadians(angles.pitch)
    );

    auto const yaw = calculateRotationMatrix(
      Axis::Z,
      degreesToRadians(angles.yaw)
    );

    return multiplyMatrices(
      yaw,
      multiplyMatrices(pitch, roll)
    );
  }

  Matrix3d calculateCameraRotationMatrix(CameraAngles const& angles)
  {
    // Camera coordinates -> heli/body coordinates.
    //
    // Camera has no roll.
    //
    // camera yaw:
    //   rotation around heli/camera Z
    //
    // camera pitch:
    //   rotation around the yawed camera Y
    //
    // Therefore:
    //
    // body = Rz(yaw) * Ry(pitch) * camera

    auto const pitch = calculateRotationMatrix(
      Axis::Y,
      degreesToRadians(angles.pitch)
    );

    auto const yaw = calculateRotationMatrix(
      Axis::Z,
      degreesToRadians(angles.yaw)
    );

    return multiplyMatrices(yaw, pitch);
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
    // Camera looks along its local +X axis when yaw/pitch are zero.
    Vector3d const forward {1, 0, 0};

    // Camera frame -> heli/body frame.
    auto const cameraRotation =
      calculateCameraRotationMatrix(camera_angles);

    // Heli/body frame -> world frame.
    auto const heliRotation =
      calculateHeliRotationMatrix(angles);

    // First rotate camera forward inside the helicopter coordinate system.
    auto const directionInHeli =
      multiplyMatrixByVector(cameraRotation, forward);

    // Then rotate the whole helicopter coordinate system into the world.
    auto const directionInWorld =
      multiplyMatrixByVector(heliRotation, directionInHeli);

    // directionInWorld is a unit vector because rotation matrices preserve
    // vector length. Scale it by the target distance and add the origin.
    auto const offset =
      multiplyVectorByScalar(directionInWorld, distance);

    return addVectors(initial_position, offset);
  }

  Vector3d rotateVector(
    Vector3d const& v,
    HeliAngles const& angles
  )
  {
    auto const rotation =
      calculateHeliRotationMatrix(angles);

    return multiplyMatrixByVector(rotation, v);
  }

  Axis heliAngleToRotationAxis(HeliAngle angle)
  {
    switch(angle) {
      case HeliAngle::Roll:
        return Axis::X;

      case HeliAngle::Pitch:
        return Axis::Y;

      case HeliAngle::Yaw:
        return Axis::Z;
    }

    throw std::runtime_error(
      "Unknown heli angle: "
      + std::to_string(static_cast<int>(angle))
    );
  }

  Vector3d rotateVector(
    Vector3d const& v,
    Axis axis,
    double angle
  )
  {
    auto const radians = degreesToRadians(angle);
    auto const rotationMatrix =
      calculateRotationMatrix(axis, radians);

    return multiplyMatrixByVector(rotationMatrix, v);
  }

  Matrix3d calculateRotationMatrix(
    Axis axis,
    double radians
  )
  {
    // clang-format off

    switch(axis) {
      case Axis::Z:
        return {
          { std::cos(radians), -std::sin(radians), 0 },
          { std::sin(radians),  std::cos(radians), 0 },
          { 0,                  0,                 1 }
        };

      case Axis::X:
        return {
          { 1, 0,                  0                 },
          { 0, std::cos(radians), -std::sin(radians) },
          { 0, std::sin(radians),  std::cos(radians) }
        };

      case Axis::Y:
        return {
          {  std::cos(radians), 0, std::sin(radians) },
          {  0,                 1, 0                 },
          { -std::sin(radians), 0, std::cos(radians) }
        };
    }

    // clang-format on

    throw std::runtime_error("Invalid axis");
  }

  double degreesToRadians(double degrees)
  {
    return degrees * M_PI / 180.0;
  }

  Vector3d addVectors(
    Vector3d const& v1,
    Vector3d const& v2
  )
  {
    return {
      v1.x + v2.x,
      v1.y + v2.y,
      v1.z + v2.z
    };
  }

  Vector3d subtractVectors(
    Vector3d const& v1,
    Vector3d const& v2
  )
  {
    return {
      v1.x - v2.x,
      v1.y - v2.y,
      v1.z - v2.z
    };
  }

  Vector3d multiplyVectorByScalar(
    Vector3d const& v,
    double scalar
  )
  {
    return {
      v.x * scalar,
      v.y * scalar,
      v.z * scalar
    };
  }

  Vector3d multiplyMatrixByVector(
    Matrix3d const& matrix,
    Vector3d const& v
  )
  {
    if(matrix.size() != 3
       || matrix[0].size() != 3
       || matrix[1].size() != 3
       || matrix[2].size() != 3) {
      throw std::runtime_error("Matrix must be 3x3");
    }

    return {
      matrix[0][0] * v.x
        + matrix[0][1] * v.y
        + matrix[0][2] * v.z,

      matrix[1][0] * v.x
        + matrix[1][1] * v.y
        + matrix[1][2] * v.z,

      matrix[2][0] * v.x
        + matrix[2][1] * v.y
        + matrix[2][2] * v.z
    };
  }

  Matrix3d multiplyMatrices(
    Matrix3d const& m1,
    Matrix3d const& m2
  )
  {
    if(m1.size() != 3
       || m1[0].size() != 3
       || m1[1].size() != 3
       || m1[2].size() != 3
       || m2.size() != 3
       || m2[0].size() != 3
       || m2[1].size() != 3
       || m2[2].size() != 3) {
      throw std::runtime_error("Matrices must be 3x3");
    }

    Matrix3d result {
      {0, 0, 0},
      {0, 0, 0},
      {0, 0, 0}
    };

    for(size_t row = 0; row < 3; ++row) {
      for(size_t col = 0; col < 3; ++col) {
        for(size_t i = 0; i < 3; ++i) {
          result[row][col] +=
            m1[row][i] * m2[i][col];
        }
      }
    }

    return result;
  }

  std::ostream& operator<<(
    std::ostream& os,
    Vector3d const& v
  )
  {
    return os
      << "("
      << v.x << ", "
      << v.y << ", "
      << v.z
      << ")";
  }

  std::ostream& operator<<(
    std::ostream& os,
    Matrix3d const& matrix
  )
  {
    for(size_t row = 0; row < matrix.size(); ++row) {
      os
        << "("
        << matrix[row][0] << ", "
        << matrix[row][1] << ", "
        << matrix[row][2]
        << ")"
        << std::endl;
    }

    return os;
  }

  std::ostream& operator<<(
    std::ostream& os,
    HeliAngles const& angles
  )
  {
    return os
      << "Roll: " << angles.roll
      << ", Pitch: " << angles.pitch
      << ", Yaw: " << angles.yaw;
  }

  std::ostream& operator<<(
    std::ostream& os,
    CameraAngles const& angles
  )
  {
    return os
      << "Pitch: " << angles.pitch
      << ", Yaw: " << angles.yaw;
  }

}  // namespace cpp_math