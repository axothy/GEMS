#pragma once
struct rgb {
	float red, green, blue;
};

enum ColorName {
	BLACK, WHITE, RED,
	GREEN, BLUE, YELLOW,
	PINK, CYAN
};

namespace colors {
	rgb pallete[8] = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f},
	};
};