#pragma once

#include <math.h>

#define PI 3.1415926535897932384626433832795
	
#define ToRadians(x) (float)(((x) * MATH_PI / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / MATH_PI))


class Quaternion;

#include "ClVector.h"
#include "ClMatrix.h"


typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

typedef Vector2<float> Vector2f;
typedef Vector4<float> Vector4f;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;

typedef Matrix<int, 2> Matrix2i;
typedef Matrix3<int> Matrix3i;
typedef Matrix4<int> Matrix4i;

typedef Matrix<float, 2> Matrix2f;
typedef Matrix3<float> Matrix3f;
typedef Matrix4<float> Matrix4f;

typedef Matrix<double, 2> Matrix2d;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<double> Matrix4d;

#include "ClQuaternion.h"

