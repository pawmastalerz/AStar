//C++ A* ("A Star") algorithm.
//Under no circumstances don't copy it
//for less than 4 honey - flavoured beers.
//ALL RIGHTS RESERVED to Pawel Mastalerz
//University of Warmia and Mazury, Poland

#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

char nodeValues[20][20];
int hValues[20][20];
int gValues[20][20] = {0};
int fValues[20][20] = {0};
bool openedList[20][20];
int openingOrder[20][20] = {0};
int openingCount = 1;
bool closedList[20][20];
int currentX = 0;
int currentY = 0;
int parentX[20][20] = {0};
int parentY[20][20] = {0};
int lowestFXPos = 0;
int lowestFYPos = 0;

void readNodeValuesFromFile()
{
    FILE *inputGrid;
    inputGrid = fopen("C:\\Users\\Home\\Desktop\\A gwiazdka\\Pawel_Mastalerz\\AStar\\A_Star\\bin\\Debug\\grid.txt", "r");
    if (inputGrid != NULL)
    {
        char gridNumericValue[3];
        for (int i = 19; i >= 0; i--)
        {
            for(int j = 0; j < 20; j++)
            {
                fgets(gridNumericValue, 3, inputGrid);
                nodeValues[j][i] = (gridNumericValue[0]);
            }
        }
    }
    else cout << "Input grid file missing" << endl;
    fclose(inputGrid);
}

void calculateHValues()
{
    for (int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            hValues[i][j] = 38 - j - i;
        }
    }
}

void startListsAndArrays()
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            closedList[i][j] = false;
            openedList[i][j] = false;
            calculateHValues();
        }
    }
    closedList[0][0] = true;
}

void closeNode(int x, int y)
{
    closedList[x][y] = true;
    openedList[x][y] = false;
}

void openNode(int x, int y)
{
    openedList[x][y] = true;
    openingOrder[x][y] = openingCount++;
}

void calculateGValue(int x, int y)
{
    gValues[x][y] = gValues[parentX[x][y]][parentY[x][y]] + 1;
}

void calculateFValue(int x, int y)
{
    fValues[x][y] = gValues[x][y] + hValues[x][y];
}

void checkAlreadyOpened(int x, int y)
{
    if ( (gValues[currentX][currentY] + 1) < gValues[x][y] )
    {
        parentX[x][y] = parentX[currentX][currentY];
        parentY[x][y] = parentY[currentX][currentY];
    }
}

int goUp()
{
    if (
        ((currentY + 1) > 19) ||
        (closedList[currentX][currentY + 1] == true) ||
        (nodeValues[currentX][currentY + 1] == '5')) return 0;
    else if (openedList[currentX][currentY + 1] == true)
    {
        checkAlreadyOpened(currentX, currentY + 1);
    }
    else
    {
        openNode(currentX, currentY + 1);
        parentX[currentX][currentY + 1] = currentX;
        parentY[currentX][currentY + 1] = currentY;
        calculateGValue(currentX, currentY + 1);
        calculateFValue(currentX, currentY + 1);
    }
}

int goDown()
{
    if (
        ((currentY - 1) < 0) ||
        (closedList[currentX][currentY - 1] == true) ||
        (nodeValues[currentX][currentY - 1] == '5')) return 0;
    else if (openedList[currentX][currentY - 1] == true)
    {
        checkAlreadyOpened(currentX, currentY - 1);
    }
    else
    {
        openNode(currentX, currentY - 1);
        parentX[currentX][currentY - 1] = currentX;
        parentY[currentX][currentY - 1] = currentY;
        calculateGValue(currentX, currentY - 1);
        calculateFValue(currentX, currentY - 1);
    }
}

int goLeft()
{
    if (
        ((currentX - 1) < 0) ||
        (closedList[currentX - 1][currentY] == true) ||
        (nodeValues[currentX - 1][currentY] == '5')) return 0;
    else if (openedList[currentX - 1][currentY] == true)
    {
        checkAlreadyOpened(currentX - 1, currentY);
    }
    else
    {
        openNode(currentX - 1, currentY);
        parentX[currentX - 1][currentY] = currentX;
        parentY[currentX - 1][currentY] = currentY;
        calculateGValue(currentX - 1, currentY);
        calculateFValue(currentX - 1, currentY);
    }
}

int goRight()
{
    if (
        ((currentX + 1) > 19) ||
        (closedList[currentX + 1][currentY] == true) ||
        (nodeValues[currentX + 1][currentY] == '5')) return 0;
    else if (openedList[currentX + 1][currentY] == true)
    {
        checkAlreadyOpened(currentX + 1, currentY);
    }
    else
    {
        openNode(currentX + 1, currentY);
        parentX[currentX + 1][currentY] = currentX;
        parentY[currentX + 1][currentY] = currentY;
        calculateGValue(currentX + 1, currentY);
        calculateFValue(currentX + 1, currentY);
    }
}

void calculateLowestFPos()
{
    int lowestF = 9999;
    //Find lowest F amongst opened:
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if ((openedList[i][j] == true) && (fValues[i][j] < lowestF))
            {
                lowestF = fValues[i][j];
                lowestFXPos = i;
                lowestFYPos = j;
            }
        }
    }

    //Find out if there are any duplicates of lowest value:
    int duplicateCount = 0;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if ((openedList[i][j] == true) && (fValues[i][j] == lowestF))
            {
                duplicateCount++;
            }
        }
    }
    // If there are more than 1 duplicates pick the one with the lowest opening count:
    if (duplicateCount > 1)
    {
        int lowestOpeningCount = 9999;
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                if ((openedList[i][j] == true) && (fValues[i][j] == lowestF) && (openingOrder[i][j] < lowestOpeningCount))
                {
                    lowestOpeningCount = openingOrder[i][j];
                    lowestFXPos = i;
                    lowestFYPos = j;
                }
            }
        }
    }
}

void traceRoute()
{
    int selectedX = 19;
    int selectedY = 19;
    nodeValues[0][0] = '3';

    while ((selectedX != 0) || (selectedY != 0) )
    {
        nodeValues[selectedX][selectedY] = '3';
        int tempX = selectedX;
        selectedX = parentX[selectedX][selectedY];
        selectedY = parentY[tempX][selectedY];
    }
}

int main()
{
    readNodeValuesFromFile();
    startListsAndArrays();

    while (!(currentX == 19) || !(currentY == 19))
    {
        closeNode(currentX, currentY);
        goUp();
        goDown();
        goLeft();
        goRight();
        calculateLowestFPos();
        currentX = lowestFXPos;
        currentY = lowestFYPos;
    }

    traceRoute();

    for (int i = 19; i >= 0; i--)
    {
        for(int j = 0; j < 20; j++)
        {
            cout << nodeValues[j][i] << " ";
        }
        cout << endl;
    }

    getchar();
    return 0;
}
