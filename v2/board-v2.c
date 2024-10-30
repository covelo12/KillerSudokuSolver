#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void fill_diagonals(int board[9][9]);
void shuffle(int* array, size_t n);
bool is_safe(int board[9][9], int row, int col, int num);
bool solve_sudoku(int board[9][9]);
void remove_elements(int board[9][9], int holes);
void print_board(int board[9][9], int cages[9][9]);
void create_cages(int board[9][9], int cages[9][9], int sum_cage[81]);
void print_cages(int cages[9][9], int sum_cage[81]);
void update_cage_sums(int board[9][9], int cages[9][9], int new_sum_cages[81]);
bool solve_sudoku_killer(int board[9][9], int cages[9][9], int sum_cages[81], int new_sum_cages[81]);

const char* colors[] = {
    "\033[0;31m", // Red
    "\033[0;32m", // Green
    "\033[0;33m", // Yellow
    "\033[0;34m", // Blue
    "\033[0;35m", // Magenta
    "\033[0;36m", // Cyan
    "\033[0;37m", // White
};

int main(void) {
    srand(time(NULL));  // Seed random number generator

    int board[9][9] = {0};  // Initialize the board with zeros

    // Fill diagonals with random 3x3 blocks
    fill_diagonals(board);
    int cages[9][9] = {0};
    print_board(board, cages);
    // Use backtracking to fill the entire board
    solve_sudoku(board);

    
    int sum_cage[81] = {0};

    // Create Cages
    create_cages(board, cages, sum_cage);
    printf("\nCAGES FOR YOUR GAME!\n");
    print_cages(cages, sum_cage);

    // Remove elements to create a puzzle with 'holes' number of empty cells
    remove_elements(board, 40);  // E.g., remove 40 elements

    // Print the Sudoku puzzle
    printf("\nKILLER SUDOKU SOLVE IT!\n");
    print_board(board, cages);

    int updated_sum_cage[81] = {0};
    update_cage_sums(board, cages, updated_sum_cage);

    solve_sudoku_killer(board, cages, sum_cage, updated_sum_cage);

    // Print the Sudoku puzzle Solved
    printf("\nKILLER SUDOKU SOLVED!\n");
    print_board(board, cages);

    return 0;
}

// Validates if a cage can be created
bool is_valid_cell(int cage_grid[9][9], int row, int col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9 && cage_grid[row][col] == 0;
}

void create_cages(int board[9][9], int cages[9][9], int sum_cage[81]) {
    int current_cage = 1;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (cages[i][j] == 0) {
                size_t cage_size = rand() % 4 + 1; // cage between 1 and 4
                int row = i;
                int col = j;
                for (int s = 0; s < cage_size; s++) {
                    cages[row][col] = current_cage;
                    sum_cage[current_cage - 1] += board[row][col];

                    int direction = rand() % 2; // direction 0 right, 1 down
                    // Validate if can go down or right
                    bool can_go_down = is_valid_cell(cages, row + 1, col);
                    bool can_go_right = is_valid_cell(cages, row, col + 1);
                    
                    if (can_go_down && can_go_right) {
                        if (direction == 0)
                            row++;
                        else
                            col++;
                    } else if (can_go_down) {
                        row++;
                    } else if (can_go_right) {
                        col++;
                    } else {
                        break;
                    }
                }
                current_cage++;
            }
        }
    }
}

void fill_diagonals(int board[9][9]) {
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

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

bool is_safe(int board[9][9], int row, int col, int num) {
    // Check the row
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;

    // Check the column
    for (int x = 0; x < 9; x++)
        if (board[x][col] == num)
            return false;

    // Check the 3x3 subgrid
    int start_row = row - row % 3;
    int start_col = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + start_row][j + start_col] == num)
                return false;

    return true;
}

bool find_empty_cell(int board[9][9], int *row, int *col) {
    for (*row = 0; *row < 9; (*row)++) {
        for (*col = 0; *col < 9; (*col)++) {
            if (board[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool solve_sudoku(int board[9][9]) {
    int row, col;

    // If there is no empty space, we are done
    if (!find_empty_cell(board, &row, &col))
        return true;

    // Try numbers 1-9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        if (is_safe(board, row, col, num)) {
            board[row][col] = num;

            // Recursively try to fill the rest of the board
            if (solve_sudoku(board))
                return true;

            // If it leads to no solution, reset the cell and backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

void update_cage_sums(int board[9][9], int cages[9][9], int new_sum_cages[81]) {
    // Calculate the new sums based on the current board state
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int cage_num = cages[i][j];
            if (cage_num > 0 && board[i][j] > 0) { // Only sum up non-zero cells
                new_sum_cages[cage_num - 1] += board[i][j];
            }
        }
    }
}

bool solve_sudoku_killer(int board[9][9], int cages[9][9], int sum_cages[81], int new_sum_cages[81]) {
    int row, col;

    // If there is no empty space, we are done
    if (!find_empty_cell(board, &row, &col))
        return true;

    // Try numbers 1-9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        // Check if the number can be placed
        if (is_safe(board, row, col, num) &&
            (new_sum_cages[cages[row][col] - 1] + num <= sum_cages[cages[row][col] - 1])) {
                

            board[row][col] = num;  // Place the number
            new_sum_cages[cages[row][col] - 1] += num;  // Update the cage sum

            // Recursively try to fill the rest of the board
            if (solve_sudoku_killer(board, cages, sum_cages, new_sum_cages))
                return true;

            // If it leads to no solution, reset the cell and backtrack
            new_sum_cages[cages[row][col] - 1] -= num;
            board[row][col] = 0;
        }
    }
    return false;  // No valid number found, trigger backtrack
}

void remove_elements(int board[9][9], int holes) {
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

void print_board(int board[9][9], int cages[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int cage_num = cages[i][j];
            int color = cage_num > 0 ? (cage_num % 7) : 0;  // Get color for cage or default to 0 if it's empty
            if (board[i][j] == 0) {
                printf("%s .%s ", "\033[0m", "\033[0m"); // Print empty cell as dot
            } else {
                // Print the number with its cage color
                printf("%s%2d%s ", colors[color], board[i][j], "\033[0m");
            }
        }
        printf("\n");
    }
}

void print_cages(int cages[9][9], int sum_cages[81]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int cage_num = cages[i][j];
            int color = cage_num % 7;
            printf("%s%3d%s ", colors[color], sum_cages[cages[i][j] - 1], "\033[0m");
        }
        printf("\n");
    }
}
