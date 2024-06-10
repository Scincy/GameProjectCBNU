#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct BoardSize {
    int rows;
    int cols;
} BoardSize;

typedef struct Cell {
    int data;
    bool marked;
} Cell;

typedef struct Board {
    BoardSize size;
    Cell** cells;
} Board;

typedef struct BingoWinner {
    bool userWin;
    bool cpuWin;
} BingoWinner;

static Board userBoard;
static Board cpuBoard;

void InitializeBoard(Board* targetBoard, BoardSize gameSize) {
    targetBoard->size = gameSize;
    targetBoard->cells = (Cell**)malloc(sizeof(Cell*) * gameSize.rows);
    for (int i = 0; i < gameSize.rows; i++) {
        targetBoard->cells[i] = (Cell*)malloc(sizeof(Cell) * gameSize.cols);
    }

    for (int i = 0; i < gameSize.rows; i++) {
        for (int j = 0; j < gameSize.cols; j++) {
            targetBoard->cells[i][j].marked = false;
            targetBoard->cells[i][j].data = 0; // Initialize data to 0
        }
    }
}

void PrintBoard(Board* targetBoard) {
    for (int i = 0; i < targetBoard->size.rows; i++) {
        for (int j = 0; j < targetBoard->size.cols; j++) {
            if (targetBoard->cells[i][j].marked)
                printf("%2d ", targetBoard->cells[i][j].data);
            else
                printf("?? ");
        }
        printf("\n");
    }
}

int GetBoardCellCount(Board* targetBoard) {
    return targetBoard->size.cols * targetBoard->size.rows;
}

BoardSize GetBoardSizeInput() {
    BoardSize size;
    do {
        printf("\nEnter the size of the Bingo board (positive integers):\n");
        printf("X -> ");
        scanf("%d", &size.cols);
        printf("Y -> ");
        scanf("%d", &size.rows);
        if (size.cols <= 0 || size.rows <= 0) {
            printf("Invalid board size. Please enter positive integers.\n");
        }
    } while (size.cols <= 0 || size.rows <= 0);
    return size;
}

void PrintTitle() {
    printf("\033[1;31m######    \033[1;32m ######  \033[1;33m ##   ## \033[1;34m    ##### \033[1;35m   #####   \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ###  ## \033[1;34m   ##     \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m #### ## \033[1;34m  ##      \033[1;35m  ##   ##  \n");
    printf("\033[1;31m######    \033[1;32m   ##    \033[1;33m ####### \033[1;34m  ##  ### \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ## #### \033[1;34m  ##   ## \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ##  ### \033[1;34m   ##  ## \033[1;35m  ##   ##  \n");
    printf("\033[1;31m######    \033[1;32m ######  \033[1;33m ##   ## \033[1;34m    ##### \033[1;35m   #####   \n");
    printf("                                             \n\033[1;0m");
}

void ResetScreen() {
    printf("\033[2J\033[1;1H");
    PrintTitle();
}

void GetUserBoardInput() {
    ResetScreen();
    int cellInputCount = 0;
    int totalCells = GetBoardCellCount(&userBoard);
    do {
        printf("Fill in the board numbers!\n");
        printf("Please enter values from 1 to %d, excluding already entered numbers.\n", totalCells);

        for (int i = 0; i < userBoard.size.rows; i++) {
            for (int j = 0; j < userBoard.size.cols; j++) {
                printf((i * userBoard.size.cols + j) == cellInputCount ? "\033[4m" : "\033[24m");
                if (userBoard.cells[i][j].marked) {
                    printf("%2d ", userBoard.cells[i][j].data);
                }
                else
                    printf("?? ");
            }
            printf("\n");
        }

        printf("\033[24mEnter number -> ");
        int num;
        scanf("%d", &num);

        bool alreadyEntered = false;
        for (int i = 0; i < cellInputCount; i++) {
            if (userBoard.cells[i / userBoard.size.cols][i % userBoard.size.cols].data == num) {
                alreadyEntered = true;
                break;
            }
        }

        if (!alreadyEntered && num >= 1 && num <= totalCells) {
            userBoard.cells[cellInputCount / userBoard.size.cols][cellInputCount % userBoard.size.cols].data = num;
            userBoard.cells[cellInputCount / userBoard.size.cols][cellInputCount % userBoard.size.cols].marked = true;
            cellInputCount++;
        }
        else {
            printf("Invalid number, please enter a unique number within range.\n");
        }
        ResetScreen();
    } while (cellInputCount < totalCells);
}

void LetComputerPlayBingo() {
    int cellCount = GetBoardCellCount(&cpuBoard);
    int* nums = (int*)malloc(sizeof(int) * cellCount);

    for (int i = 0; i < cellCount; i++) {
        nums[i] = i + 1;
    }

    for (int i = 0; i < cpuBoard.size.rows; i++) {
        for (int j = 0; j < cpuBoard.size.cols; j++) {
            while (true) {
                int randomIndex = rand() % cellCount;
                if (nums[randomIndex] == -1)
                    continue;
                else {
                    cpuBoard.cells[i][j].data = nums[randomIndex];
                    cpuBoard.cells[i][j].marked = true;
                    nums[randomIndex] = -1;
                    break;
                }
            }
        }
    }

    free(nums);
}

bool HasSameElement(int* arr1, int compareTarget, int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] == compareTarget)
            return true;
    }
    return false;
}

bool HasBingoHorizontal(Board* targetBoard, int* bingoNums, int selectedCount) {
    for (int i = 0; i < targetBoard->size.rows; i++) {
        int matchCount = 0;
        for (int j = 0; j < targetBoard->size.cols; j++) {
            if (HasSameElement(bingoNums, targetBoard->cells[i][j].data, selectedCount))
                matchCount++;
        }
        if (matchCount == targetBoard->size.cols)
            return true;
    }
    return false;
}

bool HasBingoVertical(Board* targetBoard, int* bingoNums, int selectedCount) {
    for (int i = 0; i < targetBoard->size.cols; i++) {
        int matchCount = 0;
        for (int j = 0; j < targetBoard->size.rows; j++) {
            if (HasSameElement(bingoNums, targetBoard->cells[j][i].data, selectedCount))
                matchCount++;
        }
        if (matchCount == targetBoard->size.rows)
            return true;
    }
    return false;
}

bool HasBingoDiagonal(Board* targetBoard, int* bingoNums, int selectedCount) {
    int minSize = targetBoard->size.rows < targetBoard->size.cols ? targetBoard->size.rows : targetBoard->size.cols;

    int matchCount = 0;
    for (int i = 0; i < minSize; i++) {
        if (HasSameElement(bingoNums, targetBoard->cells[i][i].data, selectedCount)) {
            matchCount++;
        }
    }
    if (matchCount == minSize) {
        return true;
    }

    matchCount = 0;
    for (int i = 0; i < minSize; i++) {
        if (HasSameElement(bingoNums, targetBoard->cells[i][targetBoard->size.cols - 1 - i].data, selectedCount)) {
            matchCount++;
        }
    }
    if (matchCount == minSize) {
        return true;
    }

    return false;
}

BingoWinner PlayGame() {
    BingoWinner winner = { false, false };
    int cellCount = GetBoardCellCount(&userBoard);
    if (cellCount <= 0) {
        printf("Invalid board size.\n");
        return winner;
    }
    int* nums = (int*)malloc(sizeof(int) * cellCount);
    int* selectedNums = (int*)malloc(sizeof(int) * cellCount);
    for (int i = 0; i < cellCount; i++) {
        nums[i] = i + 1;
        selectedNums[i] = -1;
    }

    for (int i = 0; i < cellCount; i++) {
        while (true) {
            int randomIndex = rand() % cellCount;
            if (nums[randomIndex] == -1)
                continue;
            else {
                selectedNums[i] = nums[randomIndex];
                nums[randomIndex] = -1;
                break;
            }
        }

        printf("\n\033[1;0m<< User's Board >>\n");
        for (int x = 0; x < userBoard.size.rows; x++) {
            for (int y = 0; y < userBoard.size.cols; y++) {
                if (HasSameElement(selectedNums, userBoard.cells[x][y].data, i + 1)) {
                    printf("\033[1;31m");
                }
                else
                    printf("\033[1;0m");
                printf("%2d ", userBoard.cells[x][y].data);
            }
            printf("\n");
        }

        printf("\n\033[1;0m<< CPU's Board >>\n");
        for (int x = 0; x < cpuBoard.size.rows; x++) {
            for (int y = 0; y < cpuBoard.size.cols; y++) {
                if (HasSameElement(selectedNums, cpuBoard.cells[x][y].data, i + 1)) {
                    printf("\033[1;31m");
                }
                else
                    printf("\033[1;0m");
                printf("%2d ", cpuBoard.cells[x][y].data);
            }
            printf("\n");
        }

        if (HasBingoHorizontal(&userBoard, selectedNums, i + 1) ||
            HasBingoVertical(&userBoard, selectedNums, i + 1) ||
            HasBingoDiagonal(&userBoard, selectedNums, i + 1)) {
            winner.userWin = true;
        }
        if (HasBingoHorizontal(&cpuBoard, selectedNums, i + 1) ||
            HasBingoVertical(&cpuBoard, selectedNums, i + 1) ||
            HasBingoDiagonal(&cpuBoard, selectedNums, i + 1)) {
            winner.cpuWin = true;
        }

        if (winner.userWin || winner.cpuWin) {
            free(nums);
            free(selectedNums);
            return winner;
        }

        printf("\nPress Enter to continue...");
        getchar();
        getchar();  // To properly wait for the user input after a number input
        ResetScreen();
    }
    free(nums);
    free(selectedNums);
    return winner;
}

void PrintGameResult(BingoWinner winner) {
    printf("\n\n\033[1;0m<< Game Result >>\n");
    if (winner.userWin)
        printf("User: Win\n");
    else
        printf("User: Lose\n");
    if (winner.cpuWin)
        printf("CPU: Win\n");
    else
        printf("CPU: Lose\n");
}

int main(void) {
    srand(time(NULL));  // Initialize random number generator once at the beginning
    PrintTitle();
    BoardSize gameSize = GetBoardSizeInput();
    InitializeBoard(&userBoard, gameSize);
    InitializeBoard(&cpuBoard, gameSize);
    GetUserBoardInput();
    LetComputerPlayBingo();
    BingoWinner winner = PlayGame();
    PrintGameResult(winner);
    return 0;
}