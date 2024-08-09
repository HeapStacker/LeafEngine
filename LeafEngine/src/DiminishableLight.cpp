#include <iostream>
#include "DiminishableLight.h"

namespace lf {

	//with settings below...
	//keep in mind, that lights almost fade out at around 20% of said space
	Attenuation attenuations[] = {
		{0.f, 0.f, 0.f},             //?
		{1.0f, 0.7f, 1.8f},		     //spans 7 units
		{1.0f, 0.35f, 0.44f},		 //spans 13 units
		{1.0f, 0.22f, 0.20f},		 //spans 20 units
		{1.0f, 0.14f, 0.07f},		 //spans 32 units
		{1.0f, 0.09f, 0.032f},		 //spans 50 units
		{1.0f, 0.07f, 0.017f},		 //spans 65 units
		{1.0f, 0.045f, 0.0075f},	 //spans 100 units
		{1.0f, 0.027f, 0.0028f},	 //spans 160 units
		{1.0f, 0.022f, 0.0019f},	 //spans 200 units
		{1.0f, 0.014f, 0.0007f},	 //spans 325 units
		{1.0f, 0.007f, 0.0002f},	 //spans 600 units
		{1.0f, 0.0014f, 0.000007f}   //spans 3250 units
	};

	void DiminishableLight::changeIntensity(unsigned int intensity)
	{
		if (intensity >= 0 && intensity <= 12) this->attenuation = attenuations[intensity];
		else std::cerr << "Intensity not correctly specified.\n";
	}

	void DiminishableLight::changeIntensity(const Attenuation& attenuation)
	{
		this->attenuation = attenuation;
	}
}