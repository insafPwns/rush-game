// rush game
//inluding the libraries
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "../include/shared.h"
#include "../include/text.h"
#include "../include/level1.h"
#include "../include/maze.h"

// POP build workaround
#include "text.c"
#include "level1.c"
#include "maze.c"

// main - sets up the window and runs the game loop
int main(void) {
    srand((unsigned int)time(NULL));
    InitWindow(SCREEN_W, SCREEN_H, "RUSH - Catch Your Marks");
    SetTargetFPS(60);

    Texture2D texTitle   = LoadTexture("resources/firstpage.png");
    Texture2D texStory1  = LoadTexture("resources/story1.png");
    Texture2D texStory2  = LoadTexture("resources/story2.png");
    Texture2D texL1Intro = LoadTexture("resources/catchmarks.png");
    Texture2D texExplain = LoadTexture("resources/levelexplanation.png");
    Texture2D texL1Bg    = LoadTexture("resources/level1background.png");
    Texture2D texNathan  = LoadTexture("resources/nathan.png");
    Texture2D texInsRush = LoadTexture("resources/insafrush.png");
    Texture2D texZenRush = LoadTexture("resources/zeinebrush.png");
    Texture2D texInsL1   = LoadTexture("resources/insaflevel1.png");
    Texture2D texZenL1   = LoadTexture("resources/zeineblevel1.png");
    Texture2D texMark    = LoadTexture("resources/mark.png");
    Texture2D texPass    = LoadTexture("resources/pass.png");
    Texture2D texFail    = LoadTexture("resources/fail.png");
    Texture2D texWin1    = LoadTexture("resources/win1.png");
    Texture2D texWin2    = LoadTexture("resources/win2.png");

    GameState state     = STATE_TITLE;
    int       dialogIdx = 0;
    float     l1Time    = L1_TIME;
    float     l2Time    = L2_TIME;

    Player     p1;
    Player     p2;
    ItemSystem items;
    ResetItems(&items);

    float p1StartX = (float)(320 - CHAR_W / 2);
    float p2StartX = (float)(960 - CHAR_W / 2);

    const char *story1Lines[3] = {
        "This semester's assessment is a Raylib project.",
        "I'll be in New Zealand for the next two weeks.",
        "You'll need to build it and present it before I get back."
    };
    const char *story2Lines[4] = {
        "I'll try to think about you while I'm there...",
        "...but I don't think I'll be able to.",
        "Now go on.",
        "Catch your marks."
    };

    while (WindowShouldClose() == false) {

        float dt = GetFrameTime();
        bool click = IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool blink = (((int)GetTime()) % 2 == 0);

        switch (state) {

            case STATE_TITLE:
                if (IsKeyPressed(KEY_ENTER)) {
                    dialogIdx = 0;
                    state     = STATE_STORY1;
                }
                break;

            case STATE_STORY1:
                if (click == true) {
                    dialogIdx = dialogIdx + 1;
                    if (dialogIdx >= 3) {
                        dialogIdx = 0;
                        state     = STATE_STORY2;
                    }
                }
                break;

            case STATE_STORY2:
                if (click == true) {
                    dialogIdx = dialogIdx + 1;
                    if (dialogIdx >= 4) {
                        dialogIdx = 0;
                        state     = STATE_L1_INTRO;
                    }
                }
                break;

            case STATE_L1_INTRO:
                if (click == true) state = STATE_L1_EXPLAIN;
                break;

            case STATE_L1_EXPLAIN:
                if (click == true) {
                    ResetL1(&p1, p1StartX);
                    ResetL1(&p2, p2StartX);
                    ResetItems(&items);
                    l1Time = L1_TIME;
                    state  = STATE_L1_PLAY;
                }
                break;

            case STATE_L1_PLAY: {
                l1Time = l1Time - dt;
                if (l1Time < 0.0f) l1Time = 0.0f;

                UpdateItems(&items, dt, &p1, &p2);

                float speed = 330.0f;

                if (IsKeyDown(KEY_D)) p1.x = p1.x + speed * dt;
                if (IsKeyDown(KEY_Q)) p1.x = p1.x - speed * dt;
                if (p1.x < 0)            p1.x = 0;
                if (p1.x > 640 - CHAR_W) p1.x = (float)(640 - CHAR_W);

                if (IsKeyDown(KEY_RIGHT)) p2.x = p2.x + speed * dt;
                if (IsKeyDown(KEY_LEFT))  p2.x = p2.x - speed * dt;
                if (p2.x < 640)            p2.x = 640.0f;
                if (p2.x > 1280 - CHAR_W)  p2.x = (float)(1280 - CHAR_W);

                if (l1Time <= 0.0f) state = STATE_L2_EXPLAIN;
                break;
            }

            case STATE_L2_EXPLAIN:
                if (click == true) {
                    GenerateMaze();
                    ResetL2(&p1, 1, 1);
                    ResetL2(&p2, 1, COLS - 2);
                    l2Time = L2_TIME;
                    state  = STATE_L2_PLAY;
                }
                break;

            case STATE_L2_PLAY: {
                l2Time          = l2Time - dt;
                p1.moveCooldown = p1.moveCooldown - dt;
                p2.moveCooldown = p2.moveCooldown - dt;

                float stepDelay = 0.13f;

                if (p1.moveCooldown <= 0.0f && p1.reachedGoal == false) {
                    int dir = -1;
                    if (IsKeyDown(KEY_Z)) dir = 0;
                    if (IsKeyDown(KEY_S)) dir = 1;
                    if (IsKeyDown(KEY_Q)) dir = 2;
                    if (IsKeyDown(KEY_D)) dir = 3;
                    if (dir >= 0) {
                        bool moved = MovePlayer(&p1, grid1, dir);
                        if (moved == true) p1.moveCooldown = stepDelay;
                    }
                }

                if (p2.moveCooldown <= 0.0f && p2.reachedGoal == false) {
                    int dir = -1;
                    if (IsKeyDown(KEY_UP))    dir = 0;
                    if (IsKeyDown(KEY_DOWN))  dir = 1;
                    if (IsKeyDown(KEY_LEFT))  dir = 2;
                    if (IsKeyDown(KEY_RIGHT)) dir = 3;
                    if (dir >= 0) {
                        bool moved = MovePlayer(&p2, grid2, dir);
                        if (moved == true) p2.moveCooldown = stepDelay;
                    }
                }

                bool timeUp = (l2Time <= 0.0f);
                bool p1Out  = (p1.reachedGoal == true && p1.marks >= 40);
                bool p2Out  = (p2.reachedGoal == true && p2.marks >= 40);
                bool bothDone = (p1Out == true && p2Out == true);
                bool bothAtExit = (p1.reachedGoal == true && p2.reachedGoal == true);

                if (timeUp == true || bothDone == true || bothAtExit == true) {
                    if (p1Out == true  && p2Out == true)  state = STATE_END_BOTH_WIN;
                    if (p1Out == false && p2Out == false) state = STATE_END_BOTH_FAIL;
                    if (p1Out == true  && p2Out == false) state = STATE_END_P1_WIN;
                    if (p1Out == false && p2Out == true)  state = STATE_END_P2_WIN;
                }
                break;
            }

            case STATE_END_BOTH_WIN:
            case STATE_END_BOTH_FAIL:
            case STATE_END_P1_WIN:
            case STATE_END_P2_WIN:
                if (IsKeyPressed(KEY_ENTER) == true) {
                    state     = STATE_TITLE;
                    dialogIdx = 0;
                }
                break;

            default: break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (state) {

            case STATE_TITLE:
                DrawTexture(texTitle, 0, 0, WHITE);
                DrawText("RUSH", 526, 206, 120, BLACK);
                DrawText("RUSH", 520, 200, 120, RED);
                DrawText("Finals week... or my final week?", 432, 346, 30, BLACK);
                DrawText("Finals week... or my final week?", 430, 340, 30, WHITE);
                if (blink == true) DrawTextCShadow("Press ENTER to Start", 660, 30, WHITE);
                break;

            case STATE_STORY1:
            case STATE_STORY2: {
                if (state == STATE_STORY1) {
                    DrawTexture(texStory1, 0, 0, WHITE);
                    DrawTextureEx(texNathan, (Vector2){700.0f, 55.0f}, 0.0f, 0.23f, WHITE);
                } else {
                    DrawTexture(texStory2, 0, 0, WHITE);
                }
                DrawRectangle(60, 570, 1160, 130, Fade(BLACK, 0.80f));
                DrawRectangleLines(60, 570, 1160, 130, WHITE);
                DrawText("Nathan:", 80, 582, 22, YELLOW);
                const char *line;
                if (state == STATE_STORY1) {
                    line = story1Lines[dialogIdx];
                } else {
                    line = story2Lines[dialogIdx];
                }
                DrawText(line, 80, 614, 24, WHITE);
                if (blink == true) DrawText("[ Click or ENTER ]", 950, 668, 18, GRAY);
                break;
            }

            case STATE_L1_INTRO:
                DrawTexture(texL1Intro, 0, 0, WHITE);
                DrawTextureEx(texInsRush, (Vector2){820.0f, 480.0f}, 0.0f, 0.27f, WHITE);
                DrawTextureEx(texZenRush, (Vector2){1020.0f, 480.0f}, 0.0f, 0.27f, WHITE);
                DrawText("LEVEL 1", 458, 80, 72, BLACK);
                DrawText("LEVEL 1", 452, 74, 72, GOLD);
                DrawText("Catch the Marks", 292, 162, 48, BLACK);
                DrawText("Catch the Marks", 288, 156, 48, ORANGE);
                if (blink == true) DrawTextCShadow("Click or ENTER to Start", 540, 28, WHITE);
                break;

            case STATE_L1_EXPLAIN:
                DrawTexture(texExplain, 0, 0, WHITE);
                DrawTextC("LEVEL 1 : CATCH THE MARKS",                      182, 38, GOLD);
                DrawTextC("You have 40 seconds. Maximum 100 marks total.",  234, 22, WHITE);
                DrawTextC("Coins fall on YOUR side only. Catch them to earn marks.", 264, 20, LIGHTGRAY);
                DrawCircle(380, 310, 16, GREEN);
                DrawTextC("+5 marks  ;  GREEN = Raylib feature",            300, 20, WHITE);
                DrawCircle(380, 354, 11, GOLD);
                DrawTextC("+10 marks  :  GOLD = Professor feedback ", 344, 20, WHITE);
                DrawCircle(380, 398, 16, RED);
                DrawTextC("-10 marks  :  BOMB = Distraction, avoid it",     388, 20, WHITE);
                DrawTextC("CONTROLS",                                         444, 24, YELLOW);
                DrawTextC("Player 1 (Insaf) : D = right    Q = left",       478, 22, WHITE);
                DrawTextC("Player 2 (Zeineb): RIGHT arrow  LEFT arrow",     510, 22, WHITE);
                DrawTextC("Marks carry over to Level 2",                     558, 19, CYAN);
                if (blink == true) DrawTextCShadow("[ Click or ENTER to Play ]", 592, 26, WHITE);
                break;

            case STATE_L1_PLAY: {
                DrawTexture(texL1Bg, 0, 0, WHITE);
                DrawLine(640, 0, 640, 720, WHITE);

                DrawText("PLAYER 1 - Insaf",   20, 10, 22, CYAN);
                DrawText("PLAYER 2 - Zeineb", 660, 10, 22, PINK);

                char buf[64];
                sprintf(buf, "Marks: %d/100", p1.marks);
                DrawText(buf, 20, 36, 20, WHITE);
                sprintf(buf, "Marks: %d/100", p2.marks);
                DrawText(buf, 660, 36, 20, WHITE);

                char timeBuf[32];
                sprintf(timeBuf, "TIME: %d", (int)ceilf(l1Time));
                int   timeW     = MeasureText(timeBuf, 30);
                Color timeColor = YELLOW;
                if (l1Time < 10.0f) timeColor = RED;
                DrawText(timeBuf, 640 - timeW / 2, 8, 30, timeColor);

                DrawItems(&items);

                float sc1 = (float)CHAR_W / (float)texInsL1.width;
                float sc2 = (float)CHAR_W / (float)texZenL1.width;
                DrawTextureEx(texInsL1, (Vector2){p1.x, (float)GROUND_Y}, 0.0f, sc1, WHITE);
                DrawTextureEx(texZenL1, (Vector2){p2.x, (float)GROUND_Y}, 0.0f, sc2, WHITE);
                DrawTextC("P1: D / Q          P2: RIGHT / LEFT", 696, 18, GRAY);
                break;
            }

            case STATE_L2_EXPLAIN: {
                DrawTexture(texExplain, 0, 0, WHITE);
                float markScale = 150.0f / (float)texMark.height;
                int   markW     = (int)(texMark.width * markScale);
                DrawTextureEx(texMark, (Vector2){(float)(640 - markW / 2), 400.0f},
                              0.0f, markScale, WHITE);
                DrawTextC("LEVEL 2 - THE MAZE ",                                      190, 36, GOLD);
                DrawTextC("DEMONSTRATE YOUR PROJECT BEFORE IT S TOO LATE",                         232, 21, WHITE);
                DrawTextC("P1 is on the LEFT   find the GOLD exit on the right side",   258, 19, CYAN);
                DrawTextC("P2 is on the RIGHT  find the GOLD exit on the left side",    282, 19, PINK);
                DrawTextC("GO FIND YOUR TUTOR ,both must reach him AND have 40+ marks",  306, 19, ORANGE);
                DrawTextC("CONTROLS",                                                      340, 22, YELLOW);
                DrawTextC("Player 1: Z up  S down  Q left  D right",                     366, 19, WHITE);
                DrawTextC("Player 2: Arrow keys",                                          390, 19, WHITE);
                if (blink == true) DrawTextCShadow("[ Click or ENTER to Start ]", 565, 24, WHITE);
                break;
            }

            case STATE_L2_PLAY: {
                ClearBackground(DARKBLUE);

                DrawMazeAt(grid1, MAZE1_X, MAZE_Y);
                DrawMazeAt(grid2, MAZE2_X, MAZE_Y);

                float markScale = (float)(MARK_GAP - 6) / (float)texMark.width;
                int   markH     = (int)(texMark.height * markScale);
                float markDrawX = (float)(MARK_CX - (MARK_GAP - 6) / 2);
                float markDrawY = (float)(MARK_CY - markH / 2);
                DrawTextureEx(texMark, (Vector2){markDrawX, markDrawY}, 0.0f, markScale, WHITE);

                int labelY      = MAZE_Y - 20;
                int insafLabelX = MAZE1_X + MAZE_W / 2 - MeasureText("Insaf",  18) / 2;
                int zenLabelX   = MAZE2_X + MAZE_W / 2 - MeasureText("Zeineb", 18) / 2;
                DrawText("Insaf",  insafLabelX, labelY, 18, CYAN);
                DrawText("Zeineb", zenLabelX,   labelY, 18, PINK);

                int p1cx = MAZE1_X + p1.col * TSIZE + TSIZE / 2;
                int p1cy = MAZE_Y  + p1.row * TSIZE + TSIZE / 2;
                int p2cx = MAZE2_X + p2.col * TSIZE + TSIZE / 2;
                int p2cy = MAZE_Y  + p2.row * TSIZE + TSIZE / 2;
                DrawCircle(p1cx, p1cy, (float)TSIZE * 0.7f, Fade(CYAN, 0.6f));
                DrawCircle(p2cx, p2cy, (float)TSIZE * 0.7f, Fade(PINK, 0.6f));

                int spriteSize = TSIZE * 2;
                DrawPlayerMaze(texInsL1, p1.row, p1.col, MAZE1_X, MAZE_Y, spriteSize);
                DrawPlayerMaze(texZenL1, p2.row, p2.col, MAZE2_X, MAZE_Y, spriteSize);

                int hudY = MAZE_Y + MAZE_H + 4;

                char hudBuf[64];
                sprintf(hudBuf, "P1  %d marks", p1.marks);
                DrawText(hudBuf, MAZE1_X, hudY, 17, CYAN);
                if (p1.reachedGoal == true && p1.marks >= 40) DrawText("OUT!", MAZE1_X, hudY + 18, 17, GREEN);
                if (p1.reachedGoal == true && p1.marks < 40)  DrawText("need 40!", MAZE1_X, hudY + 18, 15, ORANGE);

                sprintf(hudBuf, "P2  %d marks", p2.marks);
                DrawText(hudBuf, MAZE2_X, hudY, 17, PINK);
                if (p2.reachedGoal == true && p2.marks >= 40) DrawText("OUT!", MAZE2_X, hudY + 18, 17, GREEN);
                if (p2.reachedGoal == true && p2.marks < 40)  DrawText("need 40!", MAZE2_X, hudY + 18, 15, ORANGE);

                sprintf(hudBuf, "Time: %d s", (int)ceilf(l2Time));
                int   timerW     = MeasureText(hudBuf, 20);
                Color timerColor = WHITE;
                if (l2Time < 20.0f) timerColor = RED;
                DrawText(hudBuf, MARK_CX - timerW / 2, hudY, 20, timerColor);

                DrawText("Z/S/Q/D", MAZE1_X, hudY + 36, 14, LIGHTGRAY);
                DrawText("Arrows",  MAZE2_X, hudY + 36, 14, LIGHTGRAY);
                break;
            }

            case STATE_END_BOTH_WIN:
            case STATE_END_BOTH_FAIL:
            case STATE_END_P1_WIN:
            case STATE_END_P2_WIN: {
                Texture2D bg = texPass;
                if (state == STATE_END_BOTH_FAIL) bg = texFail;
                if (state == STATE_END_P1_WIN)    bg = texWin1;
                if (state == STATE_END_P2_WIN)    bg = texWin2;
                DrawTexture(bg, 0, 0, WHITE);
                DrawRectangle(240, 430, 800, 160, Fade(BLACK, 0.72f));
                char line1[64];
                char line2[64];
                sprintf(line1, "Insaf :  %d marks", p1.marks);
                sprintf(line2, "Zeineb:  %d marks", p2.marks);
                DrawText(line1, 640 - MeasureText(line1, 30) / 2, 448, 30, CYAN);
                DrawText(line2, 640 - MeasureText(line2, 30) / 2, 490, 30, PINK);
                if (blink == true) DrawTextCShadow("ENTER to restart", 550, 22, WHITE);
                break;
            }

            default: break;
        }

        EndDrawing();
    }

    UnloadTexture(texTitle);   UnloadTexture(texStory1);
    UnloadTexture(texStory2);
    UnloadTexture(texL1Intro); UnloadTexture(texExplain);
    UnloadTexture(texL1Bg);    UnloadTexture(texNathan);
    UnloadTexture(texInsRush); UnloadTexture(texZenRush);
    UnloadTexture(texInsL1);   UnloadTexture(texZenL1);
    UnloadTexture(texMark);    UnloadTexture(texPass);
    UnloadTexture(texFail);    UnloadTexture(texWin1);
    UnloadTexture(texWin2);

    CloseWindow();
    return 0;
}