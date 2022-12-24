#include "cMap.h"

cMap::cMap(uint32_t nWidth, uint32_t nHeight) : nWidth(nWidth), nHeight(nHeight)
{
	vecUnits.resize(nWidth* nHeight);
}
