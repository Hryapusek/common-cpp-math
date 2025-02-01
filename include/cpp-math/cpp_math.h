#pragma once

/**
 * @brief This library uses a right-handed coordinate system.
 * Index finger points to positive X axis
 * Middle finger points to positive Y axis
 * Thumb points to positive Z axis
 */

#include <vector>
#include <ostream>
#include <optional>

namespace cpp_math
{

  using Matrix3d = std::vector<std::vector<double>>;

  struct Vector3d
  {
    double x, y, z;
  };

  struct HeliAngles
  {
    // This should be in degrees [-inf, inf]
    // Since we use a right-handed coordinate system
    // positive values are counter clockwise if we see
    // towards the thumb finger (Z axis)
    double yaw;

    // This should be in degrees [-inf, inf]
    // Since we use a right-handed coordinate system
    // positive values are counter clockwise if we see
    // towards the middle finger
    double pitch;

    // This should be in degrees [-inf, inf]
    // Since we use a right-handed coordinate system
    // positive values are counter clockwise if we see
    // towards the index finger
    double roll;
  };

  struct CameraAngles
  {
    // These should be in degrees [-inf, inf]
    // Read the comment in HeliAngles
    double yaw, pitch;
  };

  enum class HeliAngle
  {
    Roll,
    Pitch,
    Yaw
  };

  enum class Axis
  {
    X,
    Y,
    Z
  };

  std::optional<Vector3d> calculatePointByDistanceAndAngles(
    double distance,
    Vector3d initial_position,
    HeliAngles angles,
    CameraAngles camera_angles
  );

  Vector3d rotateVector(Vector3d const& v, HeliAngles const& angles);
  Vector3d rotateVector(Vector3d const& v, Axis axis, double angle);

  Matrix3d calculateRotationMatrix(Axis axis, double radians);

  Axis heliAngleToAxis(HeliAngle angle);

  double degreesToRadians(double degrees);

  Vector3d addVectors(Vector3d const& v1, Vector3d const& v2);
  Vector3d subtractVectors(Vector3d const& v1, Vector3d const& v2);
  Vector3d multiplyVectorByScalar(Vector3d const& v, double scalar);
  Vector3d multiplyMatrixByVector(Matrix3d const& matrix, Vector3d const& v);
  Matrix3d multiplyMatrices(Matrix3d const& m1, Matrix3d const& m2);

  std::ostream& operator<<(std::ostream& os, Vector3d const& v);
  std::ostream& operator<<(std::ostream& os, Matrix3d const& matrix);
  std::ostream& operator<<(std::ostream& os, HeliAngles const& angles);
  std::ostream& operator<<(std::ostream& os, CameraAngles const& angles);

}  // namespace cpp_math
