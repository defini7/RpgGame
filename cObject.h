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

struct Rect
{
	Rect() = default;
	Rect(const olc::vf2d& vPos, const olc::vf2d& vSize);

	olc::vf2d vPos;
	olc::vf2d vSize;
	olc::vf2d vVel;

	Rect* rContact[4];
};

bool PointVsRect(const olc::vf2d& p, const Rect* r);
bool RectVsRect(const Rect* r1, const Rect* r2);
bool RayVsRect(const olc::vf2d& vRayOrigin, const olc::vf2d& vRayDir, const Rect* rTarget, olc::vf2d& vContactPoint, olc::vf2d& vContactNormal, float& fHitNear);

bool DynamicRectVsRect(const Rect* rDynamic, const float fTimeStep, const Rect& rStatic,
	olc::vf2d& vContactPoint, olc::vf2d& vContactNormal, float& fContactTime);

bool ResolveDynamicRectVsRect(Rect* rDynamic, const float fTimeStep, Rect* rStatic);

class cObject
{
public:
	cObject() = default;
	cObject(uint32_t nId, const Rect& rBody, uint32_t nState, uint32_t nDir, float fSpeed, uint32_t nFrameCount, bool bObstacle);

public:
	uint32_t nId;
	uint32_t nState;
	uint32_t nDir;

	Rect rBody;

	bool bObstacle;

	olc::vu2d vMapOffset = { 0, 0 };

	float fSpeed;

	uint32_t nFrameCount;
	float fFrameTime = 0.0f;
	int32_t nCurrentFrame = 0;

	std::vector<std::vector<Dialog>> vecDialogs;
};

