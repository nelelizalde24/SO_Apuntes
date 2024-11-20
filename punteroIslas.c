#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 8

// Movimientos para las 8 direcciones (horizontal, vertical, diagonal)
int rowDirs[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int colDirs[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// Comprueba si la posición (x, y) es válida
bool isValid(int x, int y, int visited[ROWS][COLS], int grid[ROWS][COLS]) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && grid[x][y] == 1 && !visited[x][y]);
}

// DFS para marcar todas las celdas conectadas como visitadas
void dfs(int x, int y, int visited[ROWS][COLS], int grid[ROWS][COLS]) {
    visited[x][y] = 1;
    for (int i = 0; i < 8; i++) {
        int newX = x + rowDirs[i];
        int newY = y + colDirs[i];
        if (isValid(newX, newY, visited, grid)) {
            dfs(newX, newY, visited, grid);
        }
    }
}

// Función para contar islas en la matriz
int countIslands(int grid[ROWS][COLS]) {
    int visited[ROWS][COLS] = {0};
    int count = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                dfs(i, j, visited, grid);
                count++;
            }
        }
    }
    return count;
}

int main() {
    int grid[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 1},
        {0, 1, 1, 0, 0, 1, 0, 1},
        {0, 1, 1, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    printf("Número de islas: %d\n", countIslands(grid));

    return 0;
}

