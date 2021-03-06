
#include "surface_fractal_terrain.hpp"
#include "point2D.hpp"

/*
 * http://gameprogrammer.com/fractal.html
 */

namespace ray_tracer {
	
	double surface_fractal_terrain::generate_random(double l, double r) {
		std::uniform_real_distribution<double> distribution(l, r);

		return distribution(rand_generator);
	}

	surface_fractal_terrain::surface_fractal_terrain(double sidelen, int size, double hscale, double h, int seed) {
		rand_generator.seed(seed);
		
		if ((size & (size - 1)) != 0) {
			throw_exception("size should be the power of 2.");
		}

		std::vector<std::vector<double> > height;

		generate_heightmap(height, size, hscale, h);
		generate_mesh(height, size, sidelen);
	}

	void surface_fractal_terrain::generate_heightmap(std::vector<std::vector<double> > &ret, int size, double hscale, double h) {
		ret.clear();
		ret.resize(size + 1);
		for (int i = 0; i <= size; ++i) ret[i].resize(size + 1);

		double ratio = pow(2.0, -h);
		double scale = hscale * ratio;
		int stride = size / 2;

		while (stride != 0) {
			// square-step
			for (int i = stride; i < size; i += (stride << 1)) {
				for (int j = stride; j < size; j += (stride << 1)) {
					double avg = (ret[i - stride][j - stride] +
						ret[i - stride][j + stride] +
						ret[i + stride][j - stride] +
						ret[i + stride][j + stride]) / 4;
					ret[i][j] = generate_random(-0.5, 0.5) * ratio + avg;
				}
			}
			// diamond-step
			int odd = 0;
			for (int i = 0; i < size; i += stride) {
				odd ^= 1;
				for (int j = (odd ? stride : 0); j < size; j += (stride << 1)) {
					double avg = 0;
					if (i == 0) {
						avg = ret[size - stride][j] + ret[i + stride][j] + ret[i][j - stride] + ret[i][j + stride];
					} else if (j == 0) {
						avg = ret[i - stride][j] + ret[i + stride][j] + ret[i][size - stride] + ret[i][j + stride];
					} else {
						avg = ret[i - stride][j] + ret[i + stride][j] + ret[i][j - stride] + ret[i][j + stride];
					}
					avg /= 4;
					ret[i][j] = scale * generate_random(-0.5, 0.5) + avg;
					if (i == 0) {
						ret[size][j] = ret[i][j];
					}
					if (j == 0) {
						ret[i][size] = ret[i][j];
					}
				}
			}
			stride /= 2;
			scale *= ratio;
		}
	}

	void surface_fractal_terrain::generate_mesh(const std::vector<std::vector<double> > &height, int size, double sidelen) {
		double scale = sidelen / size, delta = 1.0 / size;
		std::vector<point3D> vertices;

#define __id(i, j) ((i) * (size + 1) + (j))
		for (int i = 0; i <= size; ++i) {
			for (int j = 0; j <= size; ++j) {
				vertices.push_back(point3D(i * scale, j * scale, height[i][j]));
			}
		}
		setup_vertex(vertices);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				double u = i * delta, v = j * delta;

				add_surface(__id(i, j), __id(i + 1, j), __id(i, j + 1)).set_UV(point2D(u, v), point2D(u + delta, v), point2D(u, v + delta));
				add_surface(__id(i + 1, j), __id(i + 1, j + 1), __id(i, j + 1)).set_UV(point2D(u + delta, v), point2D(u + delta, v + delta), point2D(u, v + delta));
			}
		}
#undef __id
		setup_tree();
		interpolate_normal();
	}

}
