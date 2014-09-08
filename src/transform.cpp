
#include "transform.hpp"

namespace ray_tracer {

	/* transform */
	transform::transform() {
		matrix.value[0][0] = matrix.value[1][1] = matrix.value[2][2] = matrix.value[3][3] = 1;
		inv_matrix = matrix;
	}

	transform::~transform() { }

	transform transform::revert() const {
		transform ret;

		ret.matrix = inv_matrix;
		ret.inv_matrix = matrix;
		return ret;
	}

	matrix4D transform::get_matrix() const {
		return matrix;
	}

	matrix4D transform::get_inv_matrix() const {
		return inv_matrix;
	}

	/* transform_reflect */
	transform_reflect::transform_reflect(char axis) {
		matrix.value[0][0] = 1;
		matrix.value[1][1] = 1;
		matrix.value[2][2] = 1;
		matrix.value[3][3] = 1;
		switch (axis) {
		case 'X' : case 'x':
			matrix.value[0][0] = -1;
			break;
		case 'Y' : case 'y':
			matrix.value[1][1] = -1;
			break;
		case 'Z' : case 'z':
			matrix.value[2][2] = -1;
			break;
		default:
			break;
		}
		inv_matrix = matrix;
	}

	/* transform_rotate */
	transform_rotate::transform_rotate(char axis, double angle) {
		matrix.value[0][0] = 1;
		matrix.value[1][1] = 1;
		matrix.value[2][2] = 1;
		matrix.value[3][3] = 1;
		inv_matrix.value[0][0] = 1;
		inv_matrix.value[1][1] = 1;
		inv_matrix.value[2][2] = 1;
		inv_matrix.value[3][3] = 1;
		if (axis == 'x' || axis == 'X') {
			matrix.value[1][1] = cos(angle);
			matrix.value[1][2] = sin(angle);
			matrix.value[2][1] = -sin(angle);
			matrix.value[2][2] = cos(angle);
			inv_matrix.value[1][1] = cos(-angle);
			inv_matrix.value[1][2] = sin(-angle);
			inv_matrix.value[2][1] = -sin(-angle);
			inv_matrix.value[2][2] = cos(-angle);
		} else if (axis == 'y' || axis == 'Y') {
			matrix.value[0][0] = cos(angle);
			matrix.value[0][2] = -sin(angle);
			matrix.value[2][0] = sin(angle);
			matrix.value[2][2] = cos(angle);
			inv_matrix.value[0][0] = cos(-angle);
			inv_matrix.value[0][2] = -sin(-angle);
			inv_matrix.value[2][0] = sin(-angle);
			inv_matrix.value[2][2] = cos(-angle);
		} else if (axis == 'z' || axis == 'Z') {
			matrix.value[0][0] = cos(angle);
			matrix.value[0][1] = sin(angle);
			matrix.value[1][0] = -sin(angle);
			matrix.value[1][1] = cos(angle);
			inv_matrix.value[0][0] = cos(-angle);
			inv_matrix.value[0][1] = sin(-angle);
			inv_matrix.value[1][0] = -sin(-angle);
			inv_matrix.value[1][1] = cos(-angle);
		}
	}
	
	transform_rotate::transform_rotate(const point3D &centre, const vector3D &axis, double angle) {
		vector3D normal_axis = axis.normalized();
		matrix4D mat1, mat2, mat3;
		transform_translate translate = transform_translate(centre.x, centre.y, centre.z);
		double c = cos(angle), s = sin(angle);

		mat1 = translate.get_matrix();
		mat3 = translate.get_inv_matrix();
		mat2.value[0][0] = c + (1 - c) * normal_axis.x * normal_axis.x;
		mat2.value[0][1] = (1 - c) * normal_axis.x * normal_axis.y - s * normal_axis.z;
		mat2.value[0][2] = (1 - c) * normal_axis.x * normal_axis.z + s * normal_axis.y;
		mat2.value[1][0] = (1 - c) * normal_axis.x * normal_axis.y + s * normal_axis.z;
		mat2.value[1][1] = c + (1 - c) * normal_axis.y * normal_axis.y;
		mat2.value[1][2] = (1 - c) * normal_axis.y * normal_axis.z - s * normal_axis.x;
		mat2.value[2][0] = (1 - c) * normal_axis.x * normal_axis.z - s * normal_axis.y;
		mat2.value[2][1] = (1 - c) * normal_axis.y * normal_axis.z + s * normal_axis.x;
		mat2.value[2][2] = c + (1 - c) * normal_axis.z * normal_axis.z;
		// mat2.value[0][3] = 1; bug?
		// mat2.value[1][3] = 1;
		// mat2.value[2][3] = 1;
		mat2.value[3][3] = 1;
		matrix = mat1 * mat2 * mat3;

		c = cos(-angle), s = sin(-angle);
		mat2.value[0][0] = c + (1 - c) * normal_axis.x * normal_axis.x;
		mat2.value[0][1] = (1 - c) * normal_axis.x * normal_axis.y - s * normal_axis.z;
		mat2.value[0][2] = (1 - c) * normal_axis.x * normal_axis.z + s * normal_axis.y;
		mat2.value[1][0] = (1 - c) * normal_axis.x * normal_axis.y + s * normal_axis.z;
		mat2.value[1][1] = c + (1 - c) * normal_axis.y * normal_axis.y;
		mat2.value[1][2] = (1 - c) * normal_axis.y * normal_axis.z - s * normal_axis.x;
		mat2.value[2][0] = (1 - c) * normal_axis.x * normal_axis.z - s * normal_axis.y;
		mat2.value[2][1] = (1 - c) * normal_axis.y * normal_axis.z + s * normal_axis.x;
		mat2.value[2][2] = c + (1 - c) * normal_axis.z * normal_axis.z;
		// mat2.value[0][3] = 1;
		// mat2.value[1][3] = 1;
		// mat2.value[2][3] = 1;
		mat2.value[3][3] = 1;
		inv_matrix = mat1 * mat2 * mat3;
	}

	/* transform_scale */
	transform_scale::transform_scale(double x, double y, double z) {
		matrix.value[0][0] = x;
		matrix.value[1][1] = y;
		matrix.value[2][2] = z;
		matrix.value[3][3] = 1;
		inv_matrix.value[0][0] = 1 / x;
		inv_matrix.value[1][1] = 1 / y;
		inv_matrix.value[2][2] = 1 / z;
		inv_matrix.value[3][3] = 1;
	}

	/* transform_translate */
	transform_translate::transform_translate(double x, double y, double z) {
		matrix.value[0][0] = 1;
		matrix.value[1][1] = 1;
		matrix.value[2][2] = 1;
		matrix.value[3][3] = 1;
		matrix.value[0][3] = x;
		matrix.value[1][3] = y;
		matrix.value[2][3] = z;
		inv_matrix.value[0][0] = 1;
		inv_matrix.value[1][1] = 1;
		inv_matrix.value[2][2] = 1;
		inv_matrix.value[3][3] = 1;
		inv_matrix.value[0][3] = -x;
		inv_matrix.value[1][3] = -y;
		inv_matrix.value[2][3] = -z;
	}
}
