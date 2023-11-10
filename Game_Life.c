#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 40
#define GENERATIONS 100

int grid[WIDTH][HEIGHT];  // Declare an array to store the current state of cells
int newGrid[WIDTH][HEIGHT];  // Declare an array to store the next state of cells

void clearScreen() {
    COORD topLeft = {0, 0};  // Create a COORD structure with coordinates for the top-left corner
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);  // Get the console handle
    CONSOLE_SCREEN_BUFFER_INFO screen;  // Create a structure for console information
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);  // Get information about the console
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);  // Clear the screen by filling it with spaces
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);  // Set the color for clearing
    SetConsoleCursorPosition(console, topLeft);  // Set the cursor position to the top-left corner
}

void initializeGrid() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            grid[i][j] = rand() % 2;  // Initialize the grid with random values of 0 or 1
        }
    }
}

int countNeighbors(int x, int y) {
    int count = 0;
    int dx, dy;

    for (dx = -1; dx <= 1; dx++) {
        for (dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;  // Skip the current cell
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                count += grid[nx][ny];  // Count the number of live neighbors for a given cell
            }
        }
    }

    return count;
}

void updateGrid() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            int neighbors = countNeighbors(i, j);  // Count neighbors for cell (i, j)

            if (grid[i][j] == 1) {
                newGrid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;  // Update the cell's state based on the number of neighbors
            } else {
                newGrid[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            grid[i][j] = newGrid[i][j];  // Update the current grid based on the new grid
        }
    }
}

void drawGrid() {
    clearScreen();
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (grid[i][j] == 1) {
                printf("^");  // Print a symbol for a live cell
            } else {
                printf(" ");  // Print a space for an empty cell
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
        usleep(500000); // Pause for 500 milliseconds (0.5 seconds)
    }

    return 0;
}