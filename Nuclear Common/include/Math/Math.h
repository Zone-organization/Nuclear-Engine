#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "Transformation.h"
#include "Quaternion.h"
#include "Operation.h"

#define MathPI 3.14159265358979323846

namespace Math {
	inline float ToRadians(float Degrees)
	{
		return Degrees * (float)(MathPI / 180.0f);
	}
}