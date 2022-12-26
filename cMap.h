#pragma once

#include "olcPixelGameEngine.h"
#include "cObject.h"

#include <cstdint>

class cMap
{
public:
	cMap() = default;
	cMap(uint32_t nWidth, uint32_t nHeight);

public:
	struct Unit
	{
		uint32_t nId;

		Rect rBody;

		bool bObstacle = false;
		bool bTeleport = false;

		olc::vu2d vEndPoint{};
	};

	olc::Renderable gfxMap;

	uint32_t nWidth;
	uint32_t nHeight;

	std::vector<Unit> vecUnits;
};

