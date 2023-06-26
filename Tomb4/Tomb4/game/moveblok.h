#pragma once

void InitialiseMovingBlock(short item_number);
void MovableBlock(short item_number);
void MovableBlockCollision(short item_number, ItemInfo* laraitem, COLL_INFO* coll);
void InitialisePlanetEffect(short item_number);
void ControlPlanetEffect(short item_number);
void DrawPlanetEffect(ItemInfo* item);
