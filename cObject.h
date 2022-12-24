#pragma once

#include "olcPixelGameEngine.h"

#include <cstdint>

struct Dialog
{
	Dialog() = default;
	Dialog(const std::string& sText, bool bEntry, uint32_t nAuthorId);

	uint32_t nAuthorId;
	std::string sText;

	bool bEntry;
	bool bFinished = false;
	bool bConfirmed = false;
	bool bConsequenceDone = false;
};

class cObject
{
public:
	cObject() = default;
	cObject(uint32_t nId, const olc::vf2d& vMapPos, uint32_t nState, uint32_t nDir, float fSpeed, uint32_t nFrameCount);

public:
	uint32_t nId;
	olc::vf2d vMapPos;
	uint32_t nState;
	uint32_t nDir;

	olc::vu2d vMapOffset = { 0, 0 };

	float fSpeed;

	uint32_t nFrameCount;
	float fFrameTime = 0.0f;
	int32_t nCurrentFrame = 0;

	std::vector<std::vector<Dialog>> vecDialogs;
};

