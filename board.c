#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fillDiagonals(int board[9][9]);
void shuffle(int* array, size_t n);

int main(void) {
    
    // Initialize the board with zeros
    int board[9][9] = {0};  
    
    //Fills the diagonals of the board
    fillDiagonals(board);

    //TODO: fill rest of the board 
    // Print current board
    for(int i=0 ; i<9 ; i++){
        for(int j=0 ; j<9 ; j++){
            printf("%d",board[i][j]);
        }
        printf(";\n");
    }
    return 0;
}

void fillDiagonals(int board[9][9]) {
    //Possible values for a 3x3 square
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 1; i <= 7; i += 3) { 

        // Fill the 3x3 square 
        int pointer = 0;
        shuffle(sample, 9);  // Shuffle the array to randomize numbers sequence

        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                board[i + j - 1][i + k - 1] = sample[pointer++];
            }
        }

    }
}

void shuffle(int* array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            srand(time(NULL));  // Seed the random number generator with the current time
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
