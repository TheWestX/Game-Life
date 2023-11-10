#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 40
#define GENERATIONS 100

int grid[WIDTH][HEIGHT];
int newGrid[WIDTH][HEIGHT];

void clearScreen() {
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void initializeGrid() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            grid[i][j] = rand() % 2;
        }
    }
}

int countNeighbors(int x, int y) {
    int count = 0;
    int dx, dy;

    for (dx = -1; dx <= 1; dx++) {
        for (dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                count += grid[nx][ny];
            }
        }
    }

    return count;
}

void updateGrid() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            int neighbors = countNeighbors(i, j);

            if (grid[i][j] == 1) {
                newGrid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                newGrid[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

void drawGrid() {
    clearScreen();
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (grid[i][j] == 1) {
                printf("^");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void usleep(int usec) {
    LARGE_INTEGER waitTime;
    waitTime.QuadPart = -10LL * usec;
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &waitTime, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

int main() {
    srand(time(NULL));
    initializeGrid();

    for (int generation = 0; generation < GENERATIONS; generation++) {
        drawGrid();
        updateGrid();
        usleep(500000); // Пауза 500 миллисекунд (0.5 секунды)
    }

    return 0;
}