#include <bits/stdc++.h>
#include <cstdlib>
#include <vector>

struct Cell
{
  int number = 1;
  int x; // obvious
  int y; // obvious
  bool isSelected;
  Cell(int x1, int y1)
  {
    x = x1;
    y = y1;
    isSelected = false;
  }
};
bool isSafe(std::vector<Cell> &grid, int row, int col, int num)
{

    for (int x = 0; x <= 8; x++)
        if (grid[row + x * 9].number == num)
            return false;

    for (int x = 0; x <= 8; x++)
        if (grid[x + col * 9].number == num)
            return false;

    int startRow = row - row % 3,
        startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[(i + startRow) + (j + startCol) * 9].number == num)
                return false;

    return true;
}
bool solveSudoku(std::vector<Cell> &grid, int row, int col)
{
    if (row == 9 - 1 && col == 9)
        return true;

    if (col == 9)
    {
        row++;
        col = 0;
    }

    if (grid[row + col * 9].number > 0)
        return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= 9; num++)
    {

        if (isSafe(grid, row, col, num))
        {

            grid[row + col * 9].number = num;

            if (solveSudoku(grid, row, col + 1))
                return true;
        }

        grid[row + col * 9].number = 0;
    }
    return false;
}
bool unUsedInBox(std::vector<Cell> &grid, int rowStart, int colStart, int num)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[(rowStart + i) + (colStart + j) * 9].number == num)
            {
                return false;
            }
        }
    }
    return true;
}
void fillBox(std::vector<Cell> &grid, int row, int col)
{
    int num;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            do
            {
                num = (rand() % 9) + 1;
            } while (!unUsedInBox(grid, row, col, num));
            grid[(row + i) + (col + j) * 9].number = num;
        }
    }
}

bool unUsedInRow(std::vector<Cell> &grid, int i, int num)
{
    for (int j = 0; j < 9; j++)
    {
        if (grid[i + j * 9].number == num)
        {
            return false;
        }
    }
    return true;
}
bool unUsedInCol(std::vector<Cell> &grid, int j, int num)
{
    for (int i = 0; i < 9; i++)
    {
        if (grid[i + j * 9].number == num)
        {
            return false;
        }
    }
    return true;
}
bool checkIfSafe(std::vector<Cell> &grid, int i, int j, int num)
{
    return (unUsedInRow(grid, i, num) && unUsedInCol(grid, j, num) &&
            unUsedInBox(grid, i - i % 3, j - j % 3, num));
}
void fillDiagonal(std::vector<Cell> &grid)
{
    for (int i = 0; i < 9; i = i + 3)
    {

        fillBox(grid, i, i);
    }
}

bool fillRemaining(std::vector<Cell> &grid, int i, int j)
{
    if (j >= 9 && i < 8)
    {

        j = 0;
        i = i + 1;
    }
    if (i >= 9 && j >= 9)
    {

        return true;
    }
    if (i < 3)
    {
        if (j < 3)
        {

            j = 3;
        }
    }
    else if (i < 6)
    {
        if (j == (i / 3) * 3)
        {

            j = j + 3;
        }
    }
    else
    {
        if (j == 6)
        {
            i = i + 1;
            if (i >= 9)
            {
                return true;
            }
        }
    }

    for (int num = 1; num <= 9; num++)
    {
        if (checkIfSafe(grid, i, j, num))
        {
             grid[i + j * 9].number = num;
            if (fillRemaining(grid, i, j + 1))
            {

                return true;
            }

            grid[i + j * 9].number = 0;
        }
    }
    return false;
}
void removeKDigits(std::vector<Cell> &grid, int k)
{
    while (k > 0)
    {

        int cellId = rand() % 81;

        int i = cellId / 9;

        int j = cellId % 9;

        if (grid[i + j * 9].number != 0)
        {

            grid[i + j * 9].number = 0;

            k--;
        }
    }
}

std::vector<Cell> sudokuGenerator(int k)
{

    std::vector<Cell> grid(81);

    fillDiagonal(grid);

    fillRemaining(grid, 0, 3);

    removeKDigits(grid, k);
    return grid;
}
