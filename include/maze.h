#ifndef MAZE_MODULE_H
#define MAZE_MODULE_H

#include "shared.h"

void ResetL2(Player *p, int startRow, int startCol);
void Shuffle4(int directions[4]);
void CarveDFS(TileType maze[][COLS], bool visited[][COLS], int currentRow, int currentCol);
void GenerateMaze(void);
bool MovePlayer(Player *p, TileType g[][COLS], int dir);
void DrawMazeAt(TileType g[][COLS], int offsetX, int offsetY);
void DrawPlayerMaze(Texture2D sprite, int row, int col, int offsetX, int offsetY, int size);

#endif