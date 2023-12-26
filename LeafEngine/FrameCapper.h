#pragma once
#include <chrono>
#include <thread>
#include <iostream>

class FrameCapper
{
	static unsigned int fps;
	std::chrono::steady_clock::time_point first, last;
	std::chrono::duration<float> duration;
	static void dealWithColisions();
public:
	//needs to be called inside a content loop (game loop, animation loop...)
	FrameCapper() {
		std::cout << "here we are\n";
		first = std::chrono::steady_clock::now();
	}
	~FrameCapper() {
		last = std::chrono::steady_clock::now();
		duration = last - first;
		if (duration.count() * 1000.0 < 1000.0 / fps) {
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000.0 / fps - duration.count())));
		}
	}
	static void SetFrameCap(unsigned int fps);
	static unsigned int GetFrameCap();
	static float GetDeltaTime();
};

