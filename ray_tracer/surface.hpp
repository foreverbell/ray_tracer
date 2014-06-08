#ifndef __SURFACE_HPP__
#define __SURFACE_HPP__

#include <memory>
#include "vector3D.hpp"
#include "point3D.hpp"
#include "point2D.hpp"
#include "colorRGB.hpp"
#include "shade_context.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "transform.hpp"

namespace ray_tracer {
	class surface {
		friend class surface_compound;
		friend class tracer;
		friend class world;
	public:
		surface();
		virtual ~surface() = 0;
		// ray intersection
		/*
			return a negative value if the ray doesn't hit any surface,
			hit surface pointer remains unchanged if not compounded. 
		*/
		virtual double hit(const ray &, const surface **) const = 0;
		
		// normals && shading
		virtual vector3D atnormal(const point3D &) const;
		colorRGB material_shade(shade_context *, const colorRGB &, const vector3D &, const vector3D &, bool) const;
		void set_material(const material *);
		colorRGB texture_shade(shade_context *) const;
		void set_texture(const texture *);
		void set_shading_surface(const surface *);

		// texture mapping (UV mapping)
		virtual point2D atUV(shade_context *) const;

		// transformation
		void set_transform_center(const point3D &);
		void clear_transform();
		void apply_transform(const transform &);

	protected:
		bool hit_bound(const ray &) const;
		void set_bbox(double, double, double, double, double, double);
		void set_bsphere(const point3D &, double);

	private:
		bool hit_bsphere(const ray &) const;
		bool hit_bbox(const ray &) const; 

	protected:
		// shading
		const surface *shading_surface_ptr;
		const material *material_ptr;
		const texture *texture_ptr;

		// transformation
		bool transformed;
		point3D transform_center;
		transform transform_matrix;

	private:
		// bounding sphere & box
		bool bs_have, bb_have;
		point3D bs_center, bb_p1, bb_p2;
		double bs_radius;
	};
}

#endif