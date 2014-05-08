
#include "tracer.hpp"
#include "ray.hpp"
#include "world.hpp"
#include "camera_pinhole.hpp"

namespace ray_tracer {
	
	camera_pinhole::camera_pinhole() {
		fov_u = PI / 4;
		fov_v = PI / 4;
		tan_fov_u_coef = 2 * tan(fov_u);
		tan_fov_v_coef = 2 * tan(fov_v);
		spherical = false;
	}

	camera_pinhole::camera_pinhole(const point3D &eye_, const point3D &lookat_, const vector3D &up_, double fov_u_, double fov_v_, bool spherical_) : camera(eye_, lookat_, up_) {
		fov_u = fov_u_;
		fov_v = fov_v_;
		tan_fov_u_coef = 2 * tan(fov_u);
		tan_fov_v_coef = 2 * tan(fov_v);
		spherical = spherical_;
	}
	
	colorRGB camera_pinhole::render_scene(double x, double y, int width, int height, shade_context *context_ptr) const {
		double u, v, w;
		
		if (!spherical) {
			u = (x / width - 0.5) * tan_fov_u_coef;
			v = (y / height - 0.5) * tan_fov_v_coef;
			w = -1;
		} else {
			double alpha = PI - (x / width - 0.5) * 2 * fov_u;
			double beta = PI / 2 - (y / height - 0.5) * 2 * fov_v;
		
			u = sin(beta) * sin(alpha);
			v = cos(beta);
			w = sin(beta) * cos(alpha);
		}
		return camera::render_scene(eye, (u * axis_u + v * axis_v + w * axis_w).normalized(), context_ptr);
	}
}