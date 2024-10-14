#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


enum colour{
    NO_COLOR,
    BLUE,
    RED,
    YELLOW,
    PINK,
    PURPLE,
    GREEN
};
typedef struct {
    int val;
    enum colour color;
}cell;
typedef struct {
    int x;
    int y;
}coordinates;

void fillDiagonals(int board[9][9]);
void shuffle(int* array, size_t n);
bool isSafe(int board[9][9], int row, int col, int num);
bool solveSudoku(int board[9][9]);
void removeElements(int board[9][9], int holes);
void printBoard(int board[9][9]);
void toKiller(int board[9][9], cell killerBoard[9][9]);
int choseRandomNumber(int min, int max);
void expandIsland(cell board[9][9], coordinates island[9], enum colour c) ;




void printKillerBoard(cell killerBoard[9][9]) {
    // ANSI color codes for different colors
    const char *colorCodes[] = {
        "\033[0m",   // NO_COLOR (Default/White)
        "\033[34m",  // BLUE
        "\033[31m",  // RED
        "\033[33m",  // YELLOW
        "\033[35m",  // PINK (Magenta)
        "\033[36m",  // PURPLE (Cyan)
        "\033[32m"   // GREEN
    };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // Print vertical grid separators
            if (j == 3 || j == 6)
                printf(" | ");

            // Get the color associated with the current cell
            const char *color = colorCodes[killerBoard[i][j].color];

            // Print the cell value with the corresponding color
            printf("%s%2d\033[0m ", color, killerBoard[i][j].val);  // Reset color with \033[0m after each value
        }
        printf("\n");
        
        // Print horizontal grid separators
        if (i == 2 || i == 5)
            printf("---------------------\n");
    }
}

int main(void) {

    int board[9][9] = {0};  // Initialize the board with zeros

    // Fill diagonals with random 3x3 blocks
    fillDiagonals(board);

    // Use backtracking to fill the entire board
    solveSudoku(board);

    //Trasnform sudoku to killer sudoku 
    cell killerBoard[9][9];
    toKiller(board, killerBoard);
    printKillerBoard(killerBoard);
    // Remove elements to create a puzzle with 'holes' number of empty cells
    removeElements(board, 40);  // E.g., remove 40 elements

    // Print the Sudoku puzzle
    //printBoard(board);

    return 0;
}

void toKiller(int board[9][9], cell killerBoard[9][9]) {
    // Initialize killerBoard with values from the Sudoku board and set all colors to NO_COLOR
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            killerBoard[i][j].val = board[i][j];
            killerBoard[i][j].color = NO_COLOR;
        }
    }

    // Choose the number of conglomerations (groups of connected cells)
    int conglomerations = choseRandomNumber(5, 10);

    for (enum colour c = BLUE; c <= GREEN; c++) {
        // For each color, choose a random starting point for the island
        int i, j;
        do {
            i = choseRandomNumber(0, 8);
            j = choseRandomNumber(0, 8);
        } while (killerBoard[i][j].color != NO_COLOR); // Ensure the selected cell is not part of an island


        // Initialize the island with the first cell
        coordinates island[9];  // Maximum of 9 cells per island
        island[0].x = i;
        island[0].y = j;
        killerBoard[i][j].color = c;  // Assign color to the first cell

        // Expand the island from the initial cell
        expandIsland(killerBoard, island, c);
    }
}

void expandIsland(cell board[9][9], coordinates island[9], enum colour c) {
    int islandSize = 1;  // Initial size is 1 (the starting cell)

    for (int i = 0; i < 6; i++) {  // Try expanding up to 6 more cells
        int direction = choseRandomNumber(0, 3);
        int x = island[islandSize - 1].x;  // Start from the last cell in the island
        int y = island[islandSize - 1].y;

        switch (direction) {
            case 0: // UP 
                if (x > 0) x--;
                break;
            case 1: // RIGHT
                if (y < 8) y++;
                break;
            case 2: // DOWN
                if (x < 8) x++;
                break;
            case 3: // LEFT
                if (y > 0) y--;
                break;
        }

        // Check if the adjacent cell is valid and not part of an island
        if (board[x][y].color == NO_COLOR) {
            // Add the new cell to the island and color it
            island[islandSize].x = x;
            island[islandSize].y = y;
            board[x][y].color = c;
            islandSize++;  // Increment the size of the island
        }
        else break;
    }
}


int choseRandomNumber(int min, int max) {
  
    srand(time(NULL));  // Seed random number generator
    //random number in the range [min, max]
    int rd_num = rand() % (max - min + 1) + min;
    return rd_num;
}

void fillDiagonals(int board[9][9]) {
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    srand(time(NULL));  // Seed random number generator
    for (int i = 0; i <= 6; i += 3) {
        shuffle(sample, 9);  // Shuffle the array to randomize values
        int pointer = 0;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                board[i + j][i + k] = sample[pointer++];
            }
        }
    }
}

void shuffle(int* array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

bool isSafe(int board[9][9], int row, int col, int num) {
    // Check the row
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;

    // Check the column
    for (int x = 0; x < 9; x++)
        if (board[x][col] == num)
            return false;

    // Check the 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool findEmptyCell(int board[9][9], int *row, int *col) {
    for (*row = 0; *row < 9; (*row)++) {
        for (*col = 0; *col < 9; (*col)++) {
            if (board[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool solveSudoku(int board[9][9]) {
    int row, col;

    // If there is no empty space, we are done
    if (!findEmptyCell(board, &row, &col))
        return true;

    // Try numbers 1-9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            // Recursively try to fill the rest of the board
            if (solveSudoku(board))
                return true;

            // If it leads to no solution, reset the cell and backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

void removeElements(int board[9][9], int holes) {
    // Randomly remove 'holes' number of elements from the board
    for (int i = 0; i < holes; i++) {
        int row = rand() % 9;
        int col = rand() % 9;

        // Ensure we're removing an existing number, not an already empty cell
        while (board[row][col] == 0) {
            row = rand() % 9;
            col = rand() % 9;
        }

        board[row][col] = 0;  // Remove the number by setting it to 0
    }
}

void printBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0)
                printf(". ");
            else
                printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}
