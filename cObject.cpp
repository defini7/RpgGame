#include "cObject.h"

cObject::cObject(uint32_t nId, const olc::vf2d& vMapPos, uint32_t nState, uint32_t nDir, float fSpeed, uint32_t nFrameCount)
	: nId(nId), vMapPos(vMapPos), nState(nState), nDir(nDir), fSpeed(fSpeed), nFrameCount(nFrameCount) {}

Dialog::Dialog(const std::string& sText, bool bEntry, uint32_t nAuthorId) : sText(sText), bEntry(bEntry), nAuthorId(nAuthorId) {}
