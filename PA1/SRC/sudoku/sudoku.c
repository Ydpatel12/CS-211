#include <stdio.h>
#include<stdlib.h>
#include <stdbool.h>

bool isSafe(int board[9][9], int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    int startRow = row - (row % 3);
    int startCol = col - (col % 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

bool solveSudoku(int board[9][9]) {
    int row;
    int col;
    bool isEmpty = false;

    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                isEmpty = true;
                break;
            }
        }
        if (isEmpty) {
            break;
        }
    }

    if (!isEmpty) {
        return true;
    }

    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            if (solveSudoku(board)) {
                return true;
            }

            board[row][col] = 0;
        }
    }

    return false;
}

bool checkCorrectness(int board[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int num = board[row][col];
            // Check row and column
            for (int i = 0; i < 9; i++) {
                if (i != col && board[row][i] == num) return false;
                if (i != row && board[i][col] == num) return false;
            }
            // Check 3x3 subboard
            int startRow = (row / 3) * 3;
            int startCol = (col / 3) * 3;
            for (int i = startRow; i < startRow + 3; i++) {
                for (int j = startCol; j < startCol + 3; j++) {
                    if (i != row && j != col && board[i][j] == num) return false;
                }
            }
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Too many or too little inputs");
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("empty file\n");
        return EXIT_FAILURE;
    }

    int board[9][9];
    int finder = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            char c;
            if (fscanf(file, " %c", &c) == 1) {
                if (c == '_') {
                    board[i][j] = 0;
                    finder++;
                } else {
                    board[i][j] = c - '0';
                }
            } else {
                printf("error\n");
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);

    if (finder == 0) {
        if (checkCorrectness(board)) {
            printf("correct\n");
        } else {
            printf("incorrect\n");
        }
    } else if (finder == 1) {
        if (solveSudoku(board)) {
            printf("solvable\n");
        } else {
            printf("unsolvable\n");
        }
    } else {
        printf("error\n");
        return 1;
    }

    return 0;
}
