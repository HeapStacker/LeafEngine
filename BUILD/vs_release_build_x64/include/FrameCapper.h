#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include "Colider.h"

class FrameCapper : protected Colision
{
	static unsigned int fps;
	std::chrono::steady_clock::time_point first, last;
	std::chrono::duration<float> duration;
public:
	//needs to be called inside a content loop (game loop, animation loop...)
	FrameCapper();
	~FrameCapper();
	static void SetFrameCap(unsigned int fps);
	static unsigned int GetFrameCap();
	static float GetDeltaTime();
};

