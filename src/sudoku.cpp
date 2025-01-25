#include <stdlib.h>
#include <string>
#include <vector>
struct Cell {
  int number = 0;
  int x; // obvious
  int y; // obvious
  bool isSelected;
  bool isPencil;
  bool canBeChanged;
  std::string pencilNums;
  Cell(int x1, int y1) {
    x = x1;
    y = y1;
    isPencil = false;
    isSelected = false;
    canBeChanged = false;
  }
};
class Sudoku {
public:
  bool unUsedInBox(std::vector<Cell> &grid, int rowStart, int colStart,
                   int num) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[(rowStart + i) + (colStart + j) * 9].number == num) {
          return false;
        }
      }
    }
    return true;
  }
  bool unUsedInCol(std::vector<Cell> &grid, int j, int num) {
    for (int i = 0; i < 9; i++) {
      if (grid[i + j * 9].number == num) {
        return false;
      }
    }
    return true;
  }
  bool unUsedInRow(std::vector<Cell> &grid, int i, int num) {
    for (int j = 0; j < 9; j++) {
      if (grid[i + j * 9].number == num) {
        return false;
      }
    }
    return true;
  }
  bool checkIfSafe(std::vector<Cell> &grid, int i, int j, int num) {
    return (unUsedInRow(grid, i, num) && unUsedInCol(grid, j, num) &&
            unUsedInBox(grid, i - i % 3, j - j % 3, num));
  }
  int isValid(std::vector<Cell> &mat) {

    std::vector<std::vector<int>> rows(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> cols(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> subMat(10, std::vector<int>(10, 0));

    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {

        // Skip empty cells
        if (mat[i + j * 9].number == 0)
          return false;

        // Current value
        int val = mat[i + j * 9].number;

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
        if (subMat[idx][val] == 1)
          return false;

        // Mark as seen
        subMat[idx][val] = 1;
      }
    }
    return true;
  }
  bool solveSudoku(std::vector<Cell> &grid, int row, int col) {
    if (row == 9 - 1 && col == 9)
      return true;

    if (col == 9) {
      row++;
      col = 0;
    }

    if (grid[row + col * 9].number > 0)
      return solveSudoku(grid, row, col + 1);

    for (int num = 1; num <= 9; num++) {

      if (checkIfSafe(grid, row, col, num)) {

        grid[row + col * 9].number = num;

        if (solveSudoku(grid, row, col + 1))
          return true;
      }

      grid[row + col * 9].number = 0;
    }
    return false;
  }
  void fillBox(std::vector<Cell> &grid, int row, int col) {
    int num;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        do {
          num = (rand() % 9) + 1;
        } while (!unUsedInBox(grid, row, col, num));
        grid[(row + i) + (col + j) * 9].number = num;
      }
    }
  }

  void fillDiagonal(std::vector<Cell> &grid) {
    for (int i = 0; i < 9; i = i + 3) {
      fillBox(grid, i, i);
    }
  }

  void removeKDigits(std::vector<Cell> &grid, int k) {
    while (k > 0) {

      int cellId = rand() % 81;
      int i = cellId / 9;
      int j = cellId % 9;
      if (grid[i + j * 9].number != 0) {
        grid[i + j * 9].number = 0;
        grid[i + j * 9].canBeChanged = true;
        k--;
      }
    }
  }
};
