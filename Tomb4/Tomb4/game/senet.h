#pragma once

void InitialiseSenet(short item_number);
void MakeMove(long piece, long displacement);
void SenetControl(short item_number);
long CheckSenetWinner(long won);
void InitialiseGameStix(short item_number);
void ThrowSticks(ItemInfo* item);
void GameStixControl(short item_number);
void GameStixCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void ShockwaveExplosion(ItemInfo* item, ulong col, long speed);
void ControlGodHead(short item_number);
void DrawGodHead(ItemInfo* item);

extern long SenetTargetX;
extern long SenetTargetZ;
extern short senet_item[6];
extern char piece_moving;
extern char last_throw;
extern char senet_board[17];
extern char senet_piece[6];
