#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "colorRGB.hpp"
#include "hit_info.hpp"
#include "light.hpp"

namespace ray_tracer {
	class material {
	public:
		virtual ~material() = 0;
		virtual colorRGB material_shade(hit_info *, const colorRGB &, const vector3D &, const vector3D &) const = 0;
	};
}

#endif