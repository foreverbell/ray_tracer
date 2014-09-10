#ifndef __SURFACE_TRIANGLE_HPP__
#define __SURFACE_TRIANGLE_HPP__

#include "surface.hpp"
#include "point3D.hpp"
#include "vector3D.hpp"
#include "point2D.hpp"

namespace ray_tracer {

	class surface_triangle : public surface {
		friend class surface_mesh;
	public:
		surface_triangle(const point3D &, const point3D &, const point3D &);
		intersection_context intersect(const ray &) const;
		vector3D atnormal(const point3D &) const;
		void setnormal(const vector3D &, const vector3D &, const vector3D &);
		point2D atUV(shade_context *) const;
		void setUV(const point2D &, const point2D &, const point2D &);
	private:
		void betagamma(const point3D &, double &, double &) const;
	private:
		point3D v0, v1, v2;
		vector3D n0, n1, n2, normal;
		point2D uv0, uv1, uv2;
		bool smooth_normal, hasUV;
	private:
		double __cache[8];
	};
}

#endif
