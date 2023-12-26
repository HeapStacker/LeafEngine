#include "FrameCapper.h"

unsigned int FrameCapper::fps = 60;

void FrameCapper::SetFrameCap(unsigned int fps)
{
	FrameCapper::fps = fps;
}

unsigned int FrameCapper::GetFrameCap()
{
	return fps;
}

float FrameCapper::GetDeltaTime()
{
	return 1.0f / FrameCapper::GetFrameCap(); //1.f (not 1000.f because delta time needs to be in respect with seconds)
}
