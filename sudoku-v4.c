#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define N 9           
#define SUBGRID_SIZE (int)(sqrt(N))         
#define MAX_CAGES (N * N)
#define ELEMENTS_REMOVED 40
#define CAGE_SIZE 10

// Cell struct for individual Sudoku cells
typedef struct {
    int value;              // The number in the cell
    int cage_id;            // Cage ID
} Cell;

// Cage struct for each cage in the Killer Sudoku
typedef struct {
    int current_sum;        // Current sum of numbers in the cage
    int total_sum;          // Target sum for the cage
} Cage;

typedef struct {
    Cell board[N][N];       // The actual Sudoku board
    bool row[N][N + 1];        // Marks if a number is present in each row
    bool col[N][N + 1];        // Marks if a number is present in each column
    bool grid[N][N + 1];       // Marks if a number is present in each 3x3 subgrid
    Cage cages[MAX_CAGES];         // Array of cages, assuming a maximum of 81 cages
} SudokuBoard;


void init_sudoku_board(SudokuBoard* sb);
void fill_diagonals(SudokuBoard* sb);
void shuffle(int* array, size_t n);
bool is_safe(SudokuBoard* sb, int row, int col, int num);
bool solve_sudoku(SudokuBoard* sb);
void remove_elements(SudokuBoard* sb, int holes);
void print_board(SudokuBoard* sb);
void create_cages(SudokuBoard* sb);
void print_cages(SudokuBoard* sb);
void update_cage_sums(SudokuBoard* sb);
bool find_empty_cell(SudokuBoard* sb, int *row, int *col);
void place_number(SudokuBoard* sb, int row, int col, int num, int cage_id);
void remove_number(SudokuBoard* sb, int row, int col, int num, int cage_id);

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
    srand(time(NULL));

    SudokuBoard sb; 
    init_sudoku_board(&sb); 
    fill_diagonals(&sb);
    solve_sudoku(&sb);
    print_board(&sb);

    create_cages(&sb);
    printf("\nCAGES FOR YOUR GAME!\n");
    print_cages(&sb);

    remove_elements(&sb, ELEMENTS_REMOVED);
    printf("\nKILLER SUDOKU SOLVE IT!\n");
    print_board(&sb);
    
    update_cage_sums(&sb);
    solve_sudoku(&sb);
    printf("\nKILLER SUDOKU SOLVED!\n");
    print_board(&sb);
    return 0;
}

// Initialize the Structurs
void init_sudoku_board(SudokuBoard* sb) {
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sb->board[i][j].value = 0;
            sb->board[i][j].cage_id = 0;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N + 1; j++) {
            sb->row[i][j] = false;
            sb->col[i][j] = false;
            sb->grid[i][j] = false;
        }
    }
}

void place_number(SudokuBoard* sb, int row, int col, int num, int cage_id) {
    sb->board[row][col].value = num;
    if (cage_id > 0)
        sb->cages[sb->board[row][col].cage_id-1].current_sum += num;
    sb->row[row][num] = true;
    sb->col[col][num] = true;
    sb->grid[(row / SUBGRID_SIZE) * SUBGRID_SIZE + col / SUBGRID_SIZE][num] = true;
}

void remove_number(SudokuBoard* sb, int row, int col, int num, int cage_id) {
    sb->board[row][col].value = 0;
    if (cage_id > 0)
        sb->cages[sb->board[row][col].cage_id-1].current_sum -= num;
    sb->row[row][num] = false;
    sb->col[col][num] = false;
    sb->grid[(row / SUBGRID_SIZE) * SUBGRID_SIZE + col / SUBGRID_SIZE][num] = false;
}


void fill_diagonals(SudokuBoard* sb) {
    int sample[N];
    
    for (int i = 0; i < N; i++) {
        sample[i] = i + 1;  // Fill with numbers from 1 to N
    }

    for (int i = 0; i <= (N-SUBGRID_SIZE); i += SUBGRID_SIZE) {
        shuffle(sample, N);  // Shuffle the array to randomize values

        int pointer = 0;
        for (int row = 0; row < SUBGRID_SIZE; row++) {
            for (int col = 0; col < SUBGRID_SIZE; col++) {
                place_number(sb, i + row, i + col, sample[pointer++], 0);
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

bool is_safe(SudokuBoard* sb, int row, int col, int num) {
    // Check row, column, and subgrid
    if (sb->row[row][num] || sb->col[col][num] || sb->grid[(row / SUBGRID_SIZE) * SUBGRID_SIZE + col / SUBGRID_SIZE][num]) {
        return false; // Number is already present
    }

    int cage_id = sb->board[row][col].cage_id;
    if (cage_id > 0) {
        int cage_sum = sb->cages[cage_id - 1].current_sum;
        int cage_total_sum = sb->cages[cage_id - 1].total_sum;

        // If adding the number exceeds the cage's total sum, it's not safe
        if (cage_sum + num > cage_total_sum) {
            return false;
        }
    }

    return true;
}

bool solve_sudoku(SudokuBoard* sb) {
    int row, col;

    // If there is no empty space, we are done
    if (!find_empty_cell(sb, &row, &col))
        return true;

    // Try numbers 1-N in the empty cell
    for (int num = 1; num <= N; num++) {
        
        if (is_safe(sb, row, col, num)) {
        
            place_number(sb, row, col, num, sb->board[row][col].cage_id);
            
            // Recursively try to fill the rest of the board
            if (solve_sudoku(sb))
                return true;

            // If it leads to no solution, reset the cell
            remove_number(sb, row, col, num, sb->board[row][col].cage_id);
        }
    }
    return false;  // No valid number found, trigger backtrack
}

bool find_empty_cell(SudokuBoard* sb, int *row, int *col) {
    for (*row = 0; *row < N; (*row)++) {
        for (*col = 0; *col < N; (*col)++) {
            if (sb->board[*row][*col].value == 0) {
                return true;
            }
        }
    }
    return false;
}

void create_cages(SudokuBoard* sb) {
    int current_cage = 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            
            if (sb->board[i][j].cage_id == 0) {
                size_t cage_size = rand() % CAGE_SIZE + 1; // cage between 1 and 4
                int row = i;
                int col = j;
                sb->cages[current_cage - 1].total_sum = 0; 
                
                for (int s = 0; s < cage_size; s++) {
                    
                    sb->board[row][col].cage_id = current_cage;
                    sb->cages[current_cage - 1].total_sum += sb->board[row][col].value;

                    int direction = rand() % 2; // direction 0 right, 1 down
                    // Validate if can go down or right
                    bool can_go_down = (row + 1 < N) && (sb->board[row + 1][col].cage_id == 0);
                    bool can_go_right = (col + 1 < N) && (sb->board[row][col + 1].cage_id == 0);
                    
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
                        break; // No valid move, exit the cage creation loop
                    }
                }
                current_cage++;
            }
        }
    }
}

void print_board(SudokuBoard* sb) {
    for (int i = 0; i < N; i++) {
        if (i % SUBGRID_SIZE == 0 && i != 0) {
            printf("-------------------------------\n"); // Print separator between grids
        }
        for (int j = 0; j < N; j++) {
            if (j % SUBGRID_SIZE == 0 && j != 0) {
                printf("| "); // Print separator between subgrids
            }
            int color = sb->board[i][j].cage_id % 7; // Get color for cage
            if (sb->board[i][j].value == 0) {
                printf(" . "); // Print empty cell as a dot
            } else {
                printf("%s%2d%s ", colors[color], sb->board[i][j].value, "\033[0m"); // Print colored number
            }
        }
        printf("\n"); // New line after each row
    }
}

void print_cages(SudokuBoard* sb) {
    for (int i = 0; i < N; i++) {
        if (i % SUBGRID_SIZE == 0 && i != 0) {
            printf("-------------------------------\n"); // Print separator between grids
        }
        for (int j = 0; j < N; j++) {
            if (j % SUBGRID_SIZE == 0 && j != 0) {
                printf("| "); // Print separator between subgrids
            }
            int cage_num = sb->board[i][j].cage_id;
            if (cage_num > 0) {
                printf("%s%2d%s ", colors[cage_num % 7], sb->cages[cage_num - 1].total_sum, "\033[0m"); // Print colored cage sum
            } else {
                printf(" .  "); // Print dot for cells without a cage
            }
        }
        printf("\n"); // New line after each row
    }
}



void remove_elements(SudokuBoard* sb, int holes) {
    // Randomly remove 'holes' number of elements from the board
    for (int i = 0; i < holes; i++) {
        int row = rand() % N;
        int col = rand() % N;

        // Ensure we're removing an existing number, not an already empty cell
        while (sb->board[row][col].value == 0) {
            row = rand() % N;
            col = rand() % N;
        }
        
        remove_number(sb, row, col, sb->board[row][col].value, 0);
    }
}

void update_cage_sums(SudokuBoard* sb) {
    // Initialize curr sums
    for (int k = 0; k < MAX_CAGES; k++) {
        sb->cages[k].current_sum = 0;
        if (sb->cages[k].total_sum == 0)
            break;
    }

    // Calculate the new sums based on the current board state
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int cage_num = sb->board[i][j].cage_id;  
            if (cage_num > 0 && sb->board[i][j].value > 0) { 
                sb->cages[cage_num - 1].current_sum += sb->board[i][j].value;
            }
        }
    }
}
