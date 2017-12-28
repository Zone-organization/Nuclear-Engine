#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "Transformation.h"
#include "Operation.h"

#define MathPI 3.14159265358979323846

namespace NuclearEngine {
	namespace Math {
		inline float ToRadians(float Degrees)
		{
			return Degrees * (float)(MathPI / 180.0f);
		}
	}
}