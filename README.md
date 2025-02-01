# common-cpp-math

## Description
This library allows to perform rotations in the right hand coordinate system. The index finger points to positive X axis, the middle finger points to positive Y axis and the thumb points to positive Z axis. **It is important to understand.**

Positive yaw rotates counter clockwise when seen toward the Z axis, positive pitch rotates counter clockwise when seen toward the middle finger and positive roll rotates counter clockwise when seen toward the index finger.

Zero angles is basically X axis

For more information see [cpp_math.h](include/cpp-math/cpp_math.h)

All tests are passed!

## Examples
### HeliAngles
Rotate to the left for 45 degrees
```
HeliAngles {
  .yaw = 45,
  .pitch = 0,
  .roll = 0,
}
```

Rotate to the right for 45 degrees
```
HeliAngles {
  .yaw = -45,
  .pitch = 0,
  .roll = 0,
}
```

Rotate to the top for 45 degrees
```
HeliAngles {
  .yaw = 0,
  .pitch = -45,
  .roll = 0,
}
```

Rotate to the bottom for 45 degrees
```cpp
HeliAngles {
  .yaw = 0,
  .pitch = 45,
  .roll = 0,
}
```

Roll right for 45 degrees
```cpp
HeliAngles {
  .yaw = 0,
  .pitch = 0,
  .roll = 45,
}
```

Roll left for 45 degrees
```cpp
HeliAngles {
  .yaw = 0,
  .pitch = 0,
  .roll = -45,
}
```

### CameraAngles
This is the same as HeliAngles but for the camera
For example if we create these two:
```cpp
CameraAngles {
  .yaw = 45,
  .pitch = 0,
}
HeliAngles {
  .yaw = -45,
  .pitch = 0,
  .roll = 0,
}
```
Then camera will counter the angle of the heli and point straight to the forward direction

### calculatePointByDistanceAndAngles
The main function to calculate the point by distance from initial_position and angles from heli and camera at the bottom of the heli. Please don't forget to convert everything in suitable format

## Other
See some more examples in [tests](tests/src/test-rotations.cc)