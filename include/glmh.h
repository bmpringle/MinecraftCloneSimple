/*
 * see glm/glm.hpp
 * this is just glm/glm.hpp but without glm/packing.hpp becasue it includes volatile which is deprecated
*/

#include "glm/detail/_fixes.hpp"

#include "glm/detail/setup.hpp"

#pragma once

#include <cmath>
#include <climits>
#include <cfloat>
#include <limits>
#include <cassert>
#include "glm/fwd.hpp"

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
/*
 * Prevent volatile-deperactino warning
 * #include "glm/packing.hpp"
*/
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"
#include "glm/vector_relational.hpp"
#include "glm/integer.hpp"
