
#include "ray.hpp"

namespace ray_tracer {

	ray::ray() {
		origin = point3D(0, 0, 0);
		dir = vector3D(0, 0, 1);
		bind_light_ptr = NULL;
	}

	ray::ray(const point3D &origin_, const vector3D &dir_) { 
		origin = origin_;
		dir = dir_;
	}

	point3D ray::at(double t_) const {
		return origin + t_ * dir;
	}

	ray ray::inverse_transform(const transformation &trans, const point3D &center) const {
		ray r = ray(center + trans.inv_matrix * (origin - center), trans.inv_matrix ^ dir);
		r.bind_light_ptr = bind_light_ptr;
		return r;
	}
}