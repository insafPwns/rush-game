#include "../include/level1.h"

// completely resets a player for the start of level 1, including marks
void ResetL1(Player *p, float startX)
{
    p->x = startX;
    p->y = GROUND_Y;
    p->marks = 0;
    p->moveCooldown = 0.0f;
    p->reachedGoal = false;
}

// adds delta to the player's marks, keeping the result between 0 and 100
void AddMarks(Player *p, int markdelta) {
    int newMarks = p->marks + markdelta;
    if (newMarks < 0)
    {newMarks = 0;}
    if (newMarks > 100)
    {newMarks = 100;}
    p->marks = newMarks;
}

// clears the item system back to empty at the start of level 1
void ResetItems(ItemSystem *sys)
{
    sys->spawnTimer = 0.0f;

    sys->p1MarksPool = 100;
    sys->p2MarksPool = 100;

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        sys->items[i].active = false;
    }
}

// simple check: is the coin inside the player's rectangle
bool CoinHitsPlayer(float coinX, float coinY, float radius,float playerX, float playerY,float playerW, float playerH)
{
    if (coinX + radius < playerX) return false;
    if (coinX - radius > playerX + playerW) return false;
    if (coinY + radius < playerY) return false;
    if (coinY - radius > playerY + playerH) return false;

    return true;
}

// spawns one new item per second, moves all items down, checks collisions
void UpdateItems(ItemSystem *sys, float frameTime, Player *p1, Player *p2)
{
    sys->spawnTimer = sys->spawnTimer + frameTime;

    if (sys->spawnTimer >= 1.0f)
    {
        sys->spawnTimer = 0.0f;

        for (int side = 0; side < 2; side++)
        {
            int marksPool = 0;

            if (side == 0) marksPool = sys->p1MarksPool;
            if (side == 1) marksPool = sys->p2MarksPool;

            ItemType itemType;

            if (marksPool <= 0)
            {
                itemType = ITEM_BOMB;
            }
            else
            {
                itemType = (ItemType)(rand() % 3);
            }

            float itemSpeed = 0.0f;

            if (itemType == ITEM_GREEN) itemSpeed = 190.0f;
            if (itemType == ITEM_GOLD)  itemSpeed = 310.0f;
            if (itemType == ITEM_BOMB)  itemSpeed = 130.0f;

            float spawnX = 0.0f;

            if (side == 0)
            {
                spawnX = (float)(P1_SPAWN_MIN + rand() % P1_SPAWN_RANGE);
            }

            if (side == 1)
            {
                spawnX = (float)(P2_SPAWN_MIN + rand() % P2_SPAWN_RANGE);
            }

            for (int i = 0; i < MAX_ITEMS; i++)
            {
                if (sys->items[i].active == false)
                {
                    sys->items[i].x = spawnX;
                    sys->items[i].y = -30.0f;
                    sys->items[i].speed = itemSpeed;
                    sys->items[i].type = itemType;
                    sys->items[i].side = side;
                    sys->items[i].active = true;

                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        FallingItem *item = &sys->items[i];

        if (item->active == false) continue;

        item->y = item->y + item->speed * frameTime;

        if (item->y > 760.0f)
        {
            item->active = false;
            continue;
        }

        float radius = 0.0f;
        int markChange = 0;

        if (item->type == ITEM_GREEN) { radius = 18.0f; markChange = 5; }
        if (item->type == ITEM_GOLD)  { radius = 12.0f; markChange = 10; }
        if (item->type == ITEM_BOMB)  { radius = 20.0f; markChange = -10; }

        if (item->side == 0)
        {
            bool hitP1 = CoinHitsPlayer(
                item->x, item->y, radius,
                p1->x, (float)GROUND_Y, CHAR_W, CHAR_H
            );

            if (hitP1 == true)
            {
                AddMarks(p1, markChange);

                if (markChange > 0)
                {
                    sys->p1MarksPool = sys->p1MarksPool - markChange;
                }

                item->active = false;
            }
        }
        else
        {
            bool hitP2 = CoinHitsPlayer(
                item->x, item->y, radius,
                p2->x, (float)GROUND_Y, CHAR_W, CHAR_H
            );

            if (hitP2 == true)
            {
                AddMarks(p2, markChange);

                if (markChange > 0)
                {
                    sys->p2MarksPool = sys->p2MarksPool - markChange;
                }

                item->active = false;
            }
        }
    }
}

// draws all active falling items on screen
void DrawItems(const ItemSystem *sys)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (sys->items[i].active == false) continue;

        float x = sys->items[i].x;
        float y = sys->items[i].y;

        if (sys->items[i].type == ITEM_GREEN)
        {
            DrawCircle(x, y, 18, GREEN);
            DrawCircleLines(x, y, 18, DARKGREEN);
            DrawText("+5", x - 9, y - 8, 16, WHITE);
        }
        else if (sys->items[i].type == ITEM_GOLD)
        {
            DrawCircle(x, y, 12, GOLD);
            DrawCircleLines(x, y, 12, YELLOW);
            DrawText("+10", x - 13, y - 8, 14, WHITE);
        }
        else
        {
            DrawCircle(x, y, 20, RED);
            DrawCircleLines(x, y, 20, MAROON);
            DrawText("X", x - 5, y - 8, 18, BLACK);
        }
    }
}