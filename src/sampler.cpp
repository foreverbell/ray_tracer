
#include "sampler.hpp"
#include "miscellaneous.hpp"
#include <algorithm>

namespace ray_tracer {

	sampler::sampler(int num) {
		number_samples = num;
		samples.reserve(number_samples);
	}

	sampler::~sampler() { }

	// Shirley and Chiu(1997)
	void sampler::map_sample_to_disk() {
		samples_disk.reserve(number_samples);
		for (int i = 0; i < number_samples; i += 1) {
			double x = (samples[i].x - 0.5) * 2, y = (samples[i].y - 0.5) * 2, r, angle;
			if (x > -y) {
				if (x > y) {
					r = x;
					angle = y / x;
				} else {
					r = y;
					angle = 2 - x / y;
				}
			} else {
				if (x < y) {
					r = -x;
					angle = 4 + y / x;
				} else {
					r = -y;
					angle = 6 - x / y;
				}
			}
			angle = angle * pi / 4;
			r /= 2;
			samples_disk.push_back(point2D(r * cos(angle) + 0.5, r * sin(angle) + 0.5));
		}
	}

	int sampler::get_sampler_count() const {
		return number_samples;
	}

	sampler_iterator::sampler_iterator(const sampler *sam_) {
		sampler_ptr = sam_;
		for (int i = 0; i < n_sampler_set; i += 1) {
			sample_index[i] = i % sampler_ptr->number_samples;
		}
	}

	int sampler_iterator::get_sampler_count() const {
		return sampler_ptr->get_sampler_count();
	}

	void sampler_iterator::next_sampler() {
		for (int i = 0; i < n_sampler_set; i += 1) {
			if (++sample_index[i] >= get_sampler_count()) {
				sample_index[i] = 0;
			}
		}
	}

	point2D sampler_iterator::get_sampler_unit(int set) const {
		return sampler_ptr->samples[sample_index[set]];
	}

	point2D sampler_iterator::get_sampler_zoomed(int set, double zoom) const {
		return get_sampler_unit(set) * zoom;
	}

	point2D sampler_iterator::get_sampler_disk_unit(int set) const {
		return sampler_ptr->samples_disk[sample_index[set]];
	}

	point2D sampler_iterator::get_sampler_disk_zoomed(int set, double zoom) const {
		return get_sampler_disk_unit(set) * zoom;
	}

	sampler_single::sampler_single() : sampler(1) { 
		generate(1);
		map_sample_to_disk();
	}

	void sampler_single::generate(int num) {
		samples.push_back(point2D(0.5, 0.5));
	}

	sampler_random::sampler_random(int num) : sampler(num) { 
		generate(num);
		map_sample_to_disk();
	}

	void sampler_random::generate(int num) {
		for (int i = 0; i < number_samples; i += 1) {
			samples.push_back(point2D((double) rand() / RAND_MAX, (double) rand() / RAND_MAX));
		}
	}

	sampler_jittered::sampler_jittered(int num) : sampler(num) { 
		generate(num);
		map_sample_to_disk();
	}

	void sampler_jittered::generate(int num) {
		int root = isqrt(num);

		if (root * root == num) {
			for (int i = 0; i < root; i += 1) {
				for (int j = 0; j < root; j += 1) {
					samples.push_back(point2D((i + (double) rand() / RAND_MAX) / root, (j + (double) rand() / RAND_MAX) / root));
				}
			}
			std::random_shuffle(samples.begin(), samples.end());
		} else {
			throw_exception("number of sampler is not squared.");
		}
	}
}
