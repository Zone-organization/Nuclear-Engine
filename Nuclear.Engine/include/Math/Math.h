#pragma once

#include "glm/detail/_fixes.hpp"
#include "glm/detail/setup.hpp"

#include <cmath>
#include <climits>
#include <cfloat>
#include <limits>
#include <cassert>
#include "glm/fwd.hpp"

#if GLM_MESSAGES == GLM_MESSAGES_ENABLED && !defined(GLM_MESSAGE_CORE_INCLUDED_DISPLAYED)
#	define GLM_MESSAGE_CORE_INCLUDED_DISPLAYED
#	pragma message("GLM: Core library included")
#endif//GLM_MESSAGES

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat2x3.hpp"
#include "glm/mat2x4.hpp"
#include "glm/mat3x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat3x4.hpp"
#include "glm/mat4x2.hpp"
#include "glm/mat4x3.hpp"
#include "glm/mat4x4.hpp"

#include "glm/trigonometric.hpp"
#include "glm/exponential.hpp"
#include "glm/common.hpp"
#include "glm/packing.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/vector_relational.hpp"
#include "glm/integer.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "NETypes.h"