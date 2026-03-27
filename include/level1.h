#ifndef LEVEL1_MODULE_H
#define LEVEL1_MODULE_H

#include "shared.h"

void ResetL1(Player *p, float startX);
void AddMarks(Player *p, int markdelta);
void ResetItems(ItemSystem *sys);
bool CoinHitsPlayer(float coinX, float coinY, float radius,float playerX, float playerY, float playerW, float playerH);
void UpdateItems(ItemSystem *sys, float frameTime, Player *p1, Player *p2);
void DrawItems(const ItemSystem *sys);

#endif