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
