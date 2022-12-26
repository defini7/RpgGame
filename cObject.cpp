#include "cObject.h"

cObject::cObject(uint32_t nId, const Rect& rBody, uint32_t nState, uint32_t nDir, float fSpeed, uint32_t nFrameCount, bool bObstacle)
	: nId(nId), rBody(rBody), nState(nState), nDir(nDir), fSpeed(fSpeed), nFrameCount(nFrameCount), bObstacle(bObstacle) {}

Dialog::Dialog(const std::string& sText, bool bEntry, uint32_t nAuthorId) : sText(sText), bEntry(bEntry), nAuthorId(nAuthorId) {}

Rect::Rect(const olc::vf2d& vPos, const olc::vf2d& vSize) : vPos(vPos), vSize(vSize) {}

bool ResolveDynamicRectVsRect(Rect* rDynamic, const float fTimeStep, Rect* rStatic)
{
	olc::vf2d vContactPoint, vContactNormal;
	float fContactTime = 0.0f;
	if (DynamicRectVsRect(rDynamic, fTimeStep, *rStatic, vContactPoint, vContactNormal, fContactTime))
	{
		if (vContactNormal.y > 0) rDynamic->rContact[0] = rStatic; else nullptr;
		if (vContactNormal.x < 0) rDynamic->rContact[1] = rStatic; else nullptr;
		if (vContactNormal.y < 0) rDynamic->rContact[2] = rStatic; else nullptr;
		if (vContactNormal.x > 0) rDynamic->rContact[3] = rStatic; else nullptr;

		rDynamic->vVel += vContactNormal * olc::vf2d(std::abs(rDynamic->vVel.x), std::abs(rDynamic->vVel.y)) * (1 - fContactTime);
		return true;
	}

	return false;
}

bool DynamicRectVsRect(const Rect* rDynamic, const float fTimeStep, const Rect& rStatic,
	olc::vf2d& vContactPoint, olc::vf2d& vContactNormal, float& fContactTime)
{
	if (rDynamic->vVel.x == 0 && rDynamic->vVel.y == 0)
		return false;

	Rect rExpandedrTarget;
	rExpandedrTarget.vPos = rStatic.vPos - rDynamic->vSize / 2;
	rExpandedrTarget.vSize = rStatic.vSize + rDynamic->vSize;

	if (RayVsRect(rDynamic->vPos + rDynamic->vSize / 2, rDynamic->vVel * fTimeStep, &rExpandedrTarget, vContactPoint, vContactNormal, fContactTime))
		return (fContactTime >= 0.0f && fContactTime < 1.0f);
	else
		return false;
}

bool RayVsRect(const olc::vf2d& vRayOrigin, const olc::vf2d& vRayDir, const Rect* rTarget, olc::vf2d& vContactPoint, olc::vf2d& vContactNormal, float& fHitNear)
{
	vContactNormal = { 0,0 };
	vContactPoint = { 0,0 };

	olc::vf2d vInvDir = 1.0f / vRayDir;

	olc::vf2d vTNear = (rTarget->vPos - vRayOrigin) * vInvDir;
	olc::vf2d vTFar = (rTarget->vPos + rTarget->vSize - vRayOrigin) * vInvDir;

	if (std::isnan(vTFar.y) || std::isnan(vTFar.x)) return false;
	if (std::isnan(vTNear.y) || std::isnan(vTNear.x)) return false;

	if (vTNear.x > vTFar.x) std::swap(vTNear.x, vTFar.x);
	if (vTNear.y > vTFar.y) std::swap(vTNear.y, vTFar.y);

	if (vTNear.x > vTFar.y || vTNear.y > vTFar.x) return false;

	fHitNear = std::max(vTNear.x, vTNear.y);

	float t_hivTFar = std::min(vTFar.x, vTFar.y);

	if (t_hivTFar < 0)
		return false;

	vContactPoint = vRayOrigin + fHitNear * vRayDir;

	if (vTNear.x > vTNear.y)
		if (vInvDir.x < 0)
			vContactNormal = { 1, 0 };
		else
			vContactNormal = { -1, 0 };
	else if (vTNear.x < vTNear.y)
		if (vInvDir.y < 0)
			vContactNormal = { 0, 1 };
		else
			vContactNormal = { 0, -1 };

	return true;
}

bool PointVsRect(const olc::vf2d& p, const Rect* r)
{
	return (p.x >= r->vPos.x && p.y >= r->vPos.y && p.x < r->vPos.x + r->vSize.x && p.y < r->vPos.y + r->vSize.y);
}

bool RectVsRect(const Rect* r1, const Rect* r2)
{
	return (r1->vPos.x < r2->vPos.x + r2->vSize.x && r1->vPos.x + r1->vSize.x > r2->vPos.x && r1->vPos.y < r2->vPos.y + r2->vSize.y && r1->vPos.y + r1->vSize.y > r2->vPos.y);
}
