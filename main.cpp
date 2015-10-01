/* 
 * File:   main.cpp
 * Author: iCheater
 *
 * Created on June 28, 2015, 8:05 PM
 */

#include <cstdlib>
#include <ctype.h> // cin.get(),
#include <iostream> // cin, cout , printf,
using namespace std;

#include <unistd.h> //getch() 
#include <termios.h> //getch() 
#include <cstdlib> // clear, 



char getch() 
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

const int rowCount = 10;
const int columnCount = 15;

const char symbolHero = '1';
const char symbolWall = 'H';
const char symbolBall = 'O';
const char symbolExit = '2';
const char symbolBox = 'X';
const char levelData0 [rowCount][columnCount + 1] = {
                                                        "#####2#########",
                                                        "#  X   #   X ##",
                                                        "# X ### X  #  #",
                                                        "#X X  X  ## X #",
                                                        "# X    ##  #  #",
                                                        "#######    # X#",
                                                        "#   X   XX #X #",
                                                        "#XXX # # X   ##",
                                                        "#1 X #   X X  #",
                                                        "###############",
                                                     };

bool isGameActive = true;
char levelData [rowCount][columnCount];
int heroRow = 0;
int heroColumn = 0;

int testR;
int testC;



void SetupSystem()
{
    srand( time( 0 ) );
}
void Initialise()
{
    for(int r = 0; r < rowCount; r++)
    {
        for(int c = 0; c < columnCount; c++)
        {
            char symbol = levelData0[r][c];
        
            switch ( symbol )
            {
                case '#':
                {
                    levelData[r][c] = symbolWall;
                    break;
                }
                case '1':
                {
                    levelData[r][c] = symbolHero;
                    heroRow = r;
                    heroColumn = c;
                    break;
                }
                case '2':
                {
                    levelData[r][c] = symbolExit;
                    break;
                }
                case 'X':
                {
                    levelData[r][c] = symbolBox;
                    break;
                }
                default:
                {
                    levelData[r][c] = symbol;
                    break;
                }
            }
        }
    }
}
void Render()
{
    system("clear");
//    printf("r=%d c%d",testR,testC);
    printf("\n\n\t");
    
    
    for(int r = 0; r < rowCount; r++)
    {
        for(int c = 0; c < columnCount; c++)
        {
            char symbol = levelData[r][c];
            printf("%c",symbol);
        }
        printf("\n\t");
    }
    
}

void MoveHeroTo(int row, int column)
{
    unsigned char destinationCell = levelData[row][column];
    bool canMoveCell = false ;
    
    switch (destinationCell)
    {
        case ' ':
        {
            canMoveCell = true;
            break;
        }
        case symbolExit:
        {
            isGameActive = false;
            break;
        }
        case symbolBox:
        {
            int heroDirectionR = row - heroRow;
            int heroDirectionC = column - heroColumn;
//            testR = heroDirectionR;
//            testC = heroDirectionC;
            if (levelData[row + heroDirectionR][column+heroDirectionC] == ' ')
            {
                canMoveCell = true;
            levelData[row][column]= ' ';
            levelData[row + heroDirectionR][column+heroDirectionC] = symbolBox;
            }
            break;
            
        }
    }
    if (canMoveCell)
    {
        levelData [heroRow][heroColumn] = ' ';
        heroRow = row;
        heroColumn = column;
        levelData [heroRow][heroColumn] = symbolHero;
    }
}

void Update()
{
    char inputChar = getch();
    inputChar = tolower(inputChar);

    switch ( inputChar )
    {
        case 'a':
        {
            cout <<  "ты шо a едешь? ";
            MoveHeroTo(heroRow,heroColumn-1);
            break;
        }
        case 'w':
        {
            cout <<  "ты шо w едешь? ";
            MoveHeroTo(heroRow-1,heroColumn);
            break;
        }
        case 'd':
        {
            cout <<  "ты шо d едешь? ";
            MoveHeroTo(heroRow,heroColumn+1);
            break;
        }
        case 's':
        {
            cout <<  "ты шо s едешь? ";
            MoveHeroTo(heroRow+1,heroColumn);
            break;
        }
         case 'r':
        {
            Initialise();
            break;
        }
        
    }
}
void Shutdown()
{
    system("clear");
    cout << "Нажми для продолжения";
    cin.get();
}

int main() 
{

    SetupSystem();
    Initialise();
    do
    {
        Render();
        Update();
    }
    while (isGameActive);
    Shutdown();
    return 0;
}

