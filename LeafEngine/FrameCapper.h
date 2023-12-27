#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include "Colider.h"

class FrameCapper
{
	static unsigned int fps;
	std::chrono::steady_clock::time_point first, last;
	std::chrono::duration<float> duration;
	static void dealWithColision(ColisionPair& colisionPair);
public:
	//needs to be called inside a content loop (game loop, animation loop...)
	FrameCapper() {
		first = std::chrono::steady_clock::now();
	}
	~FrameCapper() {
		static ColisionPair colisionPair, lastColisionPair;
		while (true) {
			colisionPair = Colider::GetLatestColision();
			//ensures that 2 same, stacked collisions don't repeat
			if (!colisionPair.equalTo(lastColisionPair)) {
				dealWithColision(colisionPair);
				lastColisionPair = colisionPair;
			}
			else break;
		}
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

