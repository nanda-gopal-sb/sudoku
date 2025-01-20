#include <bits/stdc++.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#define N 9
#define M 9
#define CELL_SIZE 80

struct Cell
{
  int number = 0;
  int x; // obvious
  int y; // obvious
  bool isSelected;
  bool isHighlighted;
  bool canBeChanged;
  Cell(int x1, int y1)
  {
    x = x1;
    y = y1;
    isSelected = false;
    isHighlighted = false;
    canBeChanged = false;
  }
};
std::vector<Cell> cells;
std::vector<Cell> solution;
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
            grid[i+j*9].canBeChanged = true;
            k--;
        }
    }
}
void highlight(int num){
    for(auto nums: cells){
        if(nums.number == num){
            nums.isHighlighted = true;
        }
    }
}
void clearHighlight(){
for(auto nums: cells){
        nums.isHighlighted = false;
    }

}
void sudokuGenerator(int k)
{
    fillDiagonal(cells);
    solveSudoku(cells, 0,0);
    solution = cells;
    removeKDigits(cells, 50);
}

void clearSelected(int index)
{
  cells[index].isSelected = false;
}
bool checkForVictory(){
    for(auto nums: cells){
        if(nums.number == 0){
            return false ; 
        }
    }
    for(int i = 0 ; i<81 ; i++){
        if(cells[i].number != solution[i].number)
            return false;
    }
    return true;
}
void fillCell()
{
  for (int a = 0; a < 9; a++)
  {
    for (int b = 0; b < 9; b++)
    {
      cells.push_back(Cell(b, a)); // inversed as SFML has invered axis for some reason
    }
  }
  sudokuGenerator(10);
}
int getMousPos(sf::RenderWindow &window)
{
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
  if(cells[mouse_x+mouse_y*9].canBeChanged)
    cells[mouse_x + mouse_y * 9].isSelected = true;
  return (mouse_x + mouse_y * 9);
}
void drawRectangles(sf::RenderWindow &window)
{
  sf::Font font("assests/Greek-Freak.ttf");
  sf::Text text(font);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  sf::RectangleShape rectangle({CELL_SIZE, CELL_SIZE});
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
        if (cells[i + j * 9].isSelected){
            rectangle.setFillColor(sf::Color(99,99,99));
        }
        else
            rectangle.setFillColor(sf::Color::White);
      if ((j % 3 == 0 && j != 0) || (i % 3 == 0 && i != 0))
      {
        rectangle.setOutlineThickness(6);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
        window.draw(rectangle);
      }
      else
      {
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
        window.draw(rectangle);
      }
      text.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
      if(cells[i+j*9].number!=0){
      text.setString(std::to_string(cells[i + j * 9].number));
      window.draw(text);
      }
    }
  }
}

int main()
{
  srand(time(0));
  fillCell();
  auto window = sf::RenderWindow(sf::VideoMode({9 * 80u, 9 * 80u}), "Sudoku");
  window.setFramerateLimit(144);
  int index = -1;
  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      {
        if(index!=-1)
            clearSelected(index);
        index = getMousPos(window);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1))
      {
        if (index != -1)
        {
          cells[index].number = 1;
          highlight(1);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2))
      {
        if (index != -1)
        {
          cells[index].number = 2;highlight(2);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3))
      {
        if (index != -1)
        {
          cells[index].number = 3;highlight(3);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4))
      {
        if (index != -1)
        {
          cells[index].number = 4;highlight(4);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5))
      {
        if (index != -1)
        {
          cells[index].number = 5;highlight(5);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num6))
      {
        if (index != -1)
        {
          cells[index].number = 6;highlight(6);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num7))
      {
        if (index != -1)
        {
          cells[index].number = 7;highlight(7);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num8))
      {
        if (index != -1)
        {
          cells[index].number = 8;highlight(8);
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num9))
      {
        if (index != -1)
        {
          cells[index].number = 9;highlight(9);
          clearSelected(index);
          index = -1;
        }
      }
    }
    window.clear();
    drawRectangles(window);
    window.display();
  }
}
