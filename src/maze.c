#include "../include/maze.h"

// the maze grids are global because the DFS carver is recursive
TileType grid1[ROWS][COLS];
TileType grid2[ROWS][COLS];
bool     vis1[ROWS][COLS];

// resets only the maze-related fields for level 2
void ResetL2(Player *p, int startRow, int startCol) {
    p->row          = startRow;
    p->col          = startCol;
    p->moveCooldown = 0.0f;
    p->reachedGoal  = false;
}

// shuffles the four direction indices into a random order
void Shuffle4(int directions[4])
{
    for (int i = 0; i < 4; i++)
    {
        int randomIndex = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[randomIndex];
        directions[randomIndex] = temp;
    }
}

// DFS maze carving function
void CarveDFS(TileType maze[][COLS], bool visited[][COLS], int currentRow, int currentCol)
{
    visited[currentRow][currentCol] = true;
    maze[currentRow][currentCol] = TILE_PATH;

    int directions[4] = {0, 1, 2, 3};
    Shuffle4(directions);

    for (int i = 0; i < 4; i++)
    {
        int direction = directions[i];

        int nextRow = currentRow;
        int nextCol = currentCol;

        if (direction == 0) nextRow = currentRow - 2;
        if (direction == 1) nextRow = currentRow + 2;
        if (direction == 2) nextCol = currentCol - 2;
        if (direction == 3) nextCol = currentCol + 2;

        if (nextRow <= 0 || nextRow >= ROWS - 1 || nextCol <= 0 || nextCol >= COLS - 1)
        {
            continue;
        }

        if (visited[nextRow][nextCol] == true)
        {
            continue;
        }

        int wallRow = (currentRow + nextRow) / 2;
        int wallCol = (currentCol + nextCol) / 2;

        maze[wallRow][wallCol] = TILE_PATH;

        CarveDFS(maze, visited, nextRow, nextCol);
    }
}

// builds both mazes from the beginning
void GenerateMaze(void)
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            grid1[row][col] = TILE_WALL;
            vis1[row][col] = false;
        }
    }

    CarveDFS(grid1, vis1, 1, 1);

    grid1[P1_GOAL_ROW][P1_GOAL_COL] = TILE_PATH;
    grid1[P1_GOAL_ROW][COLS - 1] = TILE_GOAL;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            grid2[row][COLS - 1 - col] = grid1[row][col];
        }
    }
}

// tries to move the player one cell in the given direction inside grid g
bool MovePlayer(Player *p, TileType g[][COLS], int dir) {

    if (p->reachedGoal == true) return false;

    int rowSteps[4] = {-1,  1,  0, 0};
    int colSteps[4] = { 0,  0, -1, 1};

    int newRow = p->row + rowSteps[dir];
    int newCol = p->col + colSteps[dir];

    if (newRow < 0 || newRow >= ROWS) return false;
    if (newCol < 0 || newCol >= COLS) return false;

    if (g[newRow][newCol] == TILE_WALL) return false;

    p->row = newRow;
    p->col = newCol;

    if (g[newRow][newCol] == TILE_GOAL) {
        p->reachedGoal = true;
    }

    return true;
}

// draws one full maze grid with its top-left corner at pixel position (offsetX, offsetY)
void DrawMazeAt(TileType g[][COLS], int offsetX, int offsetY) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int x = offsetX + c * TSIZE;
            int y = offsetY + r * TSIZE;

            if (g[r][c] == TILE_WALL) {
                DrawRectangle(x, y, TSIZE, TSIZE, DARKBROWN);
                DrawRectangleLines(x, y, TSIZE, TSIZE, BLACK);
            } else if (g[r][c] == TILE_PATH) {
                DrawRectangle(x, y, TSIZE, TSIZE, BEIGE);
            } else if (g[r][c] == TILE_GOAL) {
                DrawRectangle(x, y, TSIZE, TSIZE, GOLD);
            }
        }
    }
}

// draws a player sprite centred on their current maze cell
void DrawPlayerMaze(Texture2D sprite, int row, int col,
                    int offsetX, int offsetY, int size) {
    float scale  = (float)size / (float)sprite.width;
    float pixelX = (float)(offsetX + col * TSIZE) - (float)(size - TSIZE) * 0.5f;
    float pixelY = (float)(offsetY + row * TSIZE) - (float)(size - TSIZE) * 0.5f;
    DrawTextureEx(sprite, (Vector2){pixelX, pixelY}, 0.0f, scale, WHITE);
}