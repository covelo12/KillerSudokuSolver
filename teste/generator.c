#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0

int is_valid(int **board, int row, int col, int num, int size, int box_size) {
    for (int x = 0; x < size; x++) {
        if (board[row][x] == num || board[x][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % box_size, startCol = col - col % box_size;
    for (int i = 0; i < box_size; i++) {
        for (int j = 0; j < box_size; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int fill_board(int **board, int size, int box_size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == EMPTY) {
                int num = (rand() % size) + 1;
                if (is_valid(board, row, col, num, size, box_size)) {
                    board[row][col] = num;
                    if (fill_board(board, size, box_size)) {
                        return 1;
                    }
                    board[row][col] = EMPTY;
                }
            }
        }
    }
    return 0;
}

void print_board(int **board, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%2d ", board[row][col]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    int size, box_size;
    printf("Enter board size (9 for 9x9, 16 for 16x16): ");
    scanf("%d", &size);

    if (size == 9) {
        box_size = 3;
    } else if (size == 16) {
        box_size = 4;
    } else {
        printf("Invalid board size.\n");
        return 1;
    }

    int **board = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        board[i] = (int *)calloc(size, sizeof(int));
    }

    if (fill_board(board, size, box_size)) {
        print_board(board, size);
    } else {
        printf("Failed to generate Sudoku board.\n");
    }

    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}
