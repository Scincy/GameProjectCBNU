#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


typedef struct BoardSize
{
    int rows;
    int cols;
}BoardSize;

typedef struct Cell{
    int data;
    bool marked;
}Cell;

typedef struct Board
{
    BoardSize size;
    Cell** cells;
}Board;

typedef struct BingoWinner
{
    bool userWin;
    bool cpuWin;
}BingoWinner;

static Board userBoard;
static Board cpuBoard;


void InitializeBoard(Board* targetBoard,BoardSize gameSize)
{
    targetBoard -> size = gameSize;
    targetBoard -> cells = (Cell**)malloc(sizeof(Cell*) * gameSize.rows);
    for (int i = 0; i < gameSize.rows; i++)
    {
        targetBoard -> cells[i] = (Cell*)malloc(sizeof(Cell) * gameSize.cols);
    }

    for(int i = 0; i < gameSize.rows; i++)
    {
        for(int j = 0; j < gameSize.cols; j++)
        {
            targetBoard -> cells[i][j].marked = false;
        }
    }
}

void printBoard(Board* targetBoard)
{
    for(int i = 0; i< targetBoard -> size.rows; i++)
    {
        for(int j = 0; j < targetBoard -> size.cols; j++)
        {
            if(targetBoard -> cells[i][j].marked)
                printf("%2d ", targetBoard -> cells[i][j].data);
            else printf("?? ");
        }
        printf("\n");
    }
}

int GetBoardCellCount(Board* targetBoard)
{
    return targetBoard->size.cols * targetBoard->size.rows;
}



BoardSize GetBoardSizeInput()
{
    BoardSize size;
    printf("\n빙고를 플레이할 보드의 크기를 입력하세요.(양의 정수로 입력)\n");
    printf("X -> ");
    scanf("%d", &size.cols);
    printf("Y -> ");
    scanf("%d", &size.rows);
    return size;
}
void PrintTitle()
{
    printf("\033[1;31m######    \033[1;32m ######  \033[1;33m ##   ## \033[1;34m    ##### \033[1;35m   #####   \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ###  ## \033[1;34m   ##     \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m #### ## \033[1;34m  ##      \033[1;35m  ##   ##  \n");
    printf("\033[1;31m######    \033[1;32m   ##    \033[1;33m ####### \033[1;34m  ##  ### \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ## #### \033[1;34m  ##   ## \033[1;35m  ##   ##  \n");
    printf("\033[1;31m##   ##   \033[1;32m   ##    \033[1;33m ##  ### \033[1;34m   ##  ## \033[1;35m  ##   ##  \n");
    printf("\033[1;31m######    \033[1;32m ######  \033[1;33m ##   ## \033[1;34m    ##### \033[1;35m   #####   \n");
    printf("                                             \n\033[1;0m");
}
void ResetScreen()
{
    printf("\033[2J\033[1;1H");
    PrintTitle();
}




void GetUserBoardInput()
{
    ResetScreen();
    int cellInputCount = 0;
    do {
        
        printf("보드에 숫자를 채워요!\n");
        printf("이미 입력한 숫자를 제외하고, 1에서 %d까지의 값을 입력해 주세요.\n", GetBoardCellCount(&userBoard));
        
        for(int i = 0; i < userBoard.size.rows; i++)
        {
            for(int j = 0; j < userBoard.size.cols; j++)
            {
                printf((i * userBoard.size.cols + j) == cellInputCount ? "\033[4m" : "\033[24m");
                if(userBoard.cells[i][j].marked)
                {
                    printf("%2d ", userBoard.cells[i][j].data);
                }
                else printf("?? ");
            }
            printf("\n");
        }
        
        printf("\033[24m번호 입력 -> ");
        scanf("%d", &(userBoard.cells[cellInputCount / userBoard.size.cols][cellInputCount % userBoard.size.cols].data));
        
        userBoard.cells[cellInputCount / userBoard.size.cols][cellInputCount % userBoard.size.cols].marked = true;
        cellInputCount++;
        ResetScreen();
        if(cellInputCount == GetBoardCellCount(&userBoard))
        {
            break;
        }
    }
    while(true);
}

void LetComputerPlayBingo()
{
    int cellCount = GetBoardCellCount(&cpuBoard);
    int nums[cellCount];

    for(int i = 0; i < cellCount; i++)
    {
        nums[i] = i+1;
    }

    srand(time(NULL));
    for(int i = 0; i < cpuBoard.size.rows; i++)
    {
        for(int j = 0; j < cpuBoard.size.cols; j++)
        {
            while(true)
            {
                
                int randomIndex = rand() % cellCount;
                if(nums[randomIndex] == -1) continue;
                else
                {
                    cpuBoard.cells[i][j].data = nums[randomIndex];
                    cpuBoard.cells[i][j].marked = true;
                    nums[randomIndex] = -1;
                    break;
                }
            }
        }
    }
}

bool HasSameElement(int* arr1, int compareTarget, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(arr1[i] == compareTarget) return true;
    }
    return false;
}


bool HasBingoHorizontal(Board* targetBoard, int* bingoNums)
{
    int matchCount = 0;
    for(int i = 0; i < targetBoard->size.rows; i++)
    {
        for(int j = 0; j < targetBoard->size.cols; j++)
        {
            if(HasSameElement(bingoNums, targetBoard->cells[i][j].data, targetBoard->size.cols))
                matchCount++;
            
        }
        if(matchCount == targetBoard->size.cols) return true;
    }
}

bool HasBingoVertical(Board* targetBoard, int* bingoNums)
{
    int matchCount = 0;
    for(int i = 0; i < targetBoard->size.cols; i++)
    {
        for(int j = 0; j < targetBoard->size.rows; j++)
        {
            if(HasSameElement(bingoNums, targetBoard->cells[i][j].data, targetBoard->size.rows))
                matchCount++;

        }
        if(matchCount == targetBoard->size.cols) return true;
    }
    return false;
}

bool HasBingoDiagonal(Board* targetBoard, int* bingoNums)
{
    //직사각형 모양도 지원하도록 가로와 세로 중 더 작은쪽을 기준으로 빙고 확인
    int minSize = targetBoard->size.rows < targetBoard->size.cols ? targetBoard->size.rows : targetBoard->size.cols;

    // 왼쪽 위에서 오른쪽 아래로 향하는 대각선 검사
    int matchCount = 0;
    for (int i = 0; i < minSize; i++)
    {
        if (HasSameElement(bingoNums, targetBoard->cells[i][i].data, minSize))
        {
            matchCount++;
        }
    }
    if (matchCount == minSize)
    {
        return true; // 왼쪽 위에서 오른쪽 아래로 향하는 대각선에 빙고가 있음
    }

    // 오른쪽 위에서 왼쪽 아래로 향하는 대각선 검사
    matchCount = 0;
    for (int i = 0; i < minSize; i++)
    {
        if (HasSameElement(bingoNums, targetBoard->cells[i][targetBoard->size.cols - 1 - i].data, minSize))
        {
            matchCount++;
        }
    }
    if (matchCount == minSize)
    {
        return true;
    }

    return false; // 양쪽 대각선 모두 빙고가 없음
}

BingoWinner PlayGame()
{
    BingoWinner winner = {false,false};
    int cellCount = GetBoardCellCount(&userBoard);
    int nums[cellCount];
    int selectedNums[cellCount];
    for(int i = 0; i < cellCount; i++)
    {
        nums[i] = i+1;
        selectedNums[i] = -1;
    }

    for(int i = 0; i < cellCount; i++)
    {
        ////////// 랜 덤 숫 자 뽑 기 ///////////
        ///////////////////////////////////////
        while(true)
        {
            //번호 선택, 이미 뽑은 숫자는 -1처리
            int randomIndex = rand() % cellCount;
            if(nums[randomIndex] == -1) continue;
            else
            {
                selectedNums[i] = nums[randomIndex];
                break;
            }
        }
        //////////////////////////////////////////
        
    
        //유저 보드에 동일한 숫자 강조 표시
        printf("\n\033[1;0m<<유저의 보드 선택>>\n");
        for(int x = 0; x < userBoard.size.rows; x++)
        {
            for(int y = 0; y < userBoard.size.cols; y++)
            {
                if(HasSameElement(selectedNums, userBoard.cells[x][y].data, cellCount))
                {
                    printf("\033[1;31m");//빨간색 강조하기
                }
                else printf("\033[1;0m");
                printf("%2d ",userBoard.cells[x][y].data);
            }
            printf("\n");
        }
    
        printf("\n<<CPU의 보드 선택>>\n");
        for(int x = 0; x < cpuBoard.size.rows; x++)
        {
            for(int y = 0; y < cpuBoard.size.cols; y++)
            {
                if(HasSameElement(selectedNums, cpuBoard.cells[x][y].data, cellCount))
                {
                    printf("\033[1;31m");//빨간색 강조하기
                }
                else printf("\033[1;0m");
                printf("%2d ",cpuBoard.cells[x][y].data);
            }
            printf("\n");
        }
        
        if(HasBingoHorizontal(&userBoard, selectedNums) ||
            HasBingoVertical(&userBoard, selectedNums) ||
            HasBingoDiagonal(&userBoard, selectedNums))
        {
            winner.userWin = true;
        }
        if(HasBingoHorizontal(&userBoard, selectedNums) ||
            HasBingoVertical(&userBoard, selectedNums) ||
            HasBingoDiagonal(&userBoard, selectedNums))
        {
            winner.cpuWin = true;
        }

        if(winner.userWin || winner.cpuWin) return winner;
        
        printf("\n 진행하려면 엔터를 입력하세요");
        int ch = getchar();
        ResetScreen();
    }
    return winner;
}

void PrintGameResult(BingoWinner winner)
{
    if(!winner.cpuWin || !winner.userWin) printf("!!!!!!!!!!!!  에러 발생, 승리자가 하나도 없어요  !!!!!!!!!!!!!");

    printf("\n\n\033[1;0m<< 게 임 결 과 >>\n");
    if(winner.userWin) printf("유저 : 승리\n");
    else printf("유저 : 패배\n");
    if(winner.cpuWin) printf("CPU : 승리\n");
    else printf("CPU : 패배\n");
    
}

int main(void) {
    
    PrintTitle();
    BoardSize gameSize = GetBoardSizeInput();
    InitializeBoard(&userBoard, gameSize);
    InitializeBoard(&cpuBoard, gameSize);
    GetUserBoardInput();
    LetComputerPlayBingo();
    BingoWinner winner = PlayGame();

    PrintGameResult(winner);

    
}
