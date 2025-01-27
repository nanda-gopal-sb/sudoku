#include <stdlib.h>
#include <string>
#include <vector>
struct Cell
{
  int number = 0;
  int x; // obvious
  int y; // obvious
  bool isSelected;
  bool isPencil;
  bool canBeChanged;
  bool isHighlight;
  std::string pencilNums;
  Cell(int x1, int y1)
  {
    x = x1;
    y = y1;
    isPencil = false;
    isSelected = false;
    canBeChanged = false;
    isHighlight = false;
    pencilNums = "     \n     \n     ";
  }
};
class Sudoku
{
public:
  /**
   * Checks if given number is used within
   * [rowStart, colStart] to [rowStart + 2, colStart + 2]
   * can be used to check duplicate numbers in a subgrid
   *
   *       colStart
   *         ↓
   *  row  → o o o
   * start   o o o
   *         o o o
   */
  bool unUsedInBox(std::vector<Cell> &grid, int rowStart, int colStart,
                   int num)
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

  // checks if number is already present in the column j
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

  // checks if number is already present in the row i
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

  /**
   * Returns true if number isn't already present in the same subgrid, row and col
   */
  bool checkIfSafe(std::vector<Cell> &grid, int i, int j, int num)
  {
    return (unUsedInRow(grid, i, num) &&
            unUsedInCol(grid, j, num) &&
            unUsedInBox(grid, i - i % 3, j - j % 3, num));
  }

  /**
   * returns whether solution is valid or not
   */
  int isValid(std::vector<Cell> &grid)
  {

    std::vector<std::vector<int>> rows(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> cols(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> subgrid(10, std::vector<int>(10, 0));

    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {

        // empty cells, not fully solved
        if (grid[i + j * 9].number == 0)
          return false;

        // Current value
        int val = grid[i + j * 9].number;

        // Check for duplicates in row
        if (rows[i][val] == 1)
          return false;

        // Mark as seen
        rows[i][val] = 1;

        // Check for duplicates in column
        if (cols[j][val] == 1)
          return false;

        // Mark as seen
        cols[j][val] = 1;

        // Check for duplicates in sub-grid
        int idx = (i / 3) * 3 + j / 3;
        if (subgrid[idx][val] == 1)
          return false;

        // Mark as seen
        subgrid[idx][val] = 1;
      }
    }
    return true;
  }

  bool solveSudoku(std::vector<Cell> &grid, int row, int col)
  {

    // return true if reached last row and overflown cols
    if (row == 9 - 1 && col == 9)
      return true;

    /**
     * wrap col when cols overflow
     *                           currently here
     *                            ↓
     *        0 1 2  3 4 5  6 7 8 x
     * goto → x 1 2  3 4 5  6 7 8
     * here
     *
     */
    if (col == 9)
    {
      row++;
      col = 0;
    }

    /** if no number is filled, goto next column of same row
     *
     *   currently here
     *   | goto here
     *   ↓ ↓
     * 0 1 2  3 4 5  6 7 8
     *
     * */
    if (grid[row + col * 9].number > 0)
      return solveSudoku(grid, row, col + 1);

    // check all possible values for a cell
    for (int num = 1; num <= 9; num++)
    {

      if (checkIfSafe(grid, row, col, num))
      {
        // number doesn't exists in the respective subgrid, row & col
        // add num to that cell and see if it is consistent
        grid[row + col * 9].number = num;
        if (solveSudoku(grid, row, col + 1))
          return true;
      }
      // number doesnt make consistent sudoku, reset the cell
      grid[row + col * 9].number = 0;
    }
    return false;
  }

  /**
   * fill each subgrid by a random number
   * A subgrid denoted by index of top left entry
   */
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

  /**
   * Fills diagonal subgrids with valid random arrangement of values
   *
   *                col index
   *           0 1 2   3 4 5   6 7 8
   *          -----------------------
   *       0   x x x | - - - | - - -
   *       1   x x x | - - - | - - -
   *       2   x x x | - - - | - - -
   *          ------ - ----- - ------
   *  row  3   - - - | x x x | - - -
   * index 4   - - - | x x x | - - -
   *       5   - - - | x x x | - - -
   *          ------ - ----- - ------
   *       6   - - - | - - - | x x x
   *       7   - - - | - - - | x x x
   *       8   - - - | - - - | x x x
   *
   */
  void fillDiagonal(std::vector<Cell> &grid)
  {
    for (int i = 0; i < 9; i = i + 3)
    {
      fillBox(grid, i, i);
    }
  }

  /**
   * Removes k number of random cells
   */
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
        grid[i + j * 9].canBeChanged = true;
        k--;
      }
    }
  }
};
