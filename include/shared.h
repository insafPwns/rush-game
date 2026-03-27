#ifndef SHARED_H
#define SHARED_H

#include "raylib.h"
#include <stdbool.h>

// defining some colors
#ifndef CYAN
#define CYAN      ((Color){  0, 255, 255, 255 })
#endif
#ifndef PINK
#define PINK      ((Color){255, 109, 194, 255 })
#endif
#ifndef GOLD
#define GOLD      ((Color){255, 203,   0, 255 })
#endif
#ifndef ORANGE
#define ORANGE    ((Color){255, 161,   0, 255 })
#endif
#ifndef SKYBLUE
#define SKYBLUE   ((Color){102, 191, 255, 255 })
#endif
#ifndef BEIGE
#define BEIGE     ((Color){211, 176, 131, 255 })
#endif
#ifndef DARKBROWN
#define DARKBROWN ((Color){ 76,  63,  47, 255 })
#endif
#ifndef DARKBLUE
#define DARKBLUE  ((Color){  0,  82, 172, 255 })
#endif
#ifndef MAROON
#define MAROON    ((Color){190,  33,  55, 255 })
#endif
#ifndef DARKGREEN
#define DARKGREEN ((Color){  0, 117,  44, 255 })
#endif
#ifndef LIGHTGRAY
#define LIGHTGRAY ((Color){200, 200, 200, 255 })
#endif
#ifndef YELLOW
#define YELLOW    ((Color){253, 249,   0, 255 })
#endif
#ifndef GRAY
#define GRAY      ((Color){130, 130, 130, 255 })
#endif

// window size
#define SCREEN_W  1280
#define SCREEN_H   720

// level 1 settings
#define MAX_ITEMS   10
#define CHAR_W      90
#define CHAR_H     100
#define GROUND_Y   610
#define L1_TIME    40.0f

// level 2
#define ROWS        37
#define COLS        31
#define TSIZE       18
#define MARK_GAP    40
#define L2_TIME     75.0f

#define MAZE_W   558
#define MAZE_H   666
#define MAZE1_X   62
#define MAZE2_X  660
#define MAZE_Y    27
#define MARK_CX  640
#define MARK_CY  360

// exit positions
#define P1_GOAL_ROW  17
#define P1_GOAL_COL  29
#define P2_GOAL_ROW  17
#define P2_GOAL_COL   1

// spawn positions
#define P1_SPAWN_MIN   10
#define P1_SPAWN_RANGE 610
#define P2_SPAWN_MIN   660
#define P2_SPAWN_RANGE 610

typedef enum {
    STATE_TITLE,
    STATE_STORY1,
    STATE_STORY2,
    STATE_L1_INTRO,
    STATE_L1_EXPLAIN,
    STATE_L1_PLAY,
    STATE_L2_EXPLAIN,
    STATE_L2_PLAY,
    STATE_END_BOTH_WIN,
    STATE_END_BOTH_FAIL,
    STATE_END_P1_WIN,
    STATE_END_P2_WIN
} GameState;

typedef enum {
    ITEM_GREEN = 0,
    ITEM_GOLD  = 1,
    ITEM_BOMB  = 2
} ItemType;

typedef enum {
    TILE_WALL = 0,
    TILE_PATH = 1,
    TILE_GOAL = 2
} TileType;

typedef struct {
    float x;
    float y;
    int   row;
    int   col;
    int   marks;
    float moveCooldown;
    bool  reachedGoal;
} Player;

typedef struct {
    float    x;
    float    y;
    float    speed;
    ItemType type;
    int      side;
    bool     active;
} FallingItem;

typedef struct {
    FallingItem items[MAX_ITEMS];
    float spawnTimer;
    int   p1MarksPool;
    int   p2MarksPool;
} ItemSystem;

// globals
extern TileType grid1[ROWS][COLS];
extern TileType grid2[ROWS][COLS];
extern bool vis1[ROWS][COLS];

#endif