#pragma once
#include "Light.h"


namespace lf {
	struct Attenuation {
		float constantMultiplier;
		float linearMultiplier;
		float quadraticMultiplier;
	};

	class DiminishableLight : public Light {
	protected:
		Attenuation attenuation;
	public:
		// set intensity 0-12 (0 and 12 included in range)
		void changeIntensity(unsigned int intensity);
		void changeIntensity(const Attenuation& attenuation);
	};
}