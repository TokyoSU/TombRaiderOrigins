#pragma once
#include "global/types.h"

void AddFootPrintLeft(ITEM_INFO* item);
void AddFootPrintRight(ITEM_INFO* item);
void S_DrawFootPrints();

extern std::vector<FOOTPRINT> Footprints;
