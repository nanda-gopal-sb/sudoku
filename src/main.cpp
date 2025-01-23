#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#define CELL_SIZE 80
#define offset 240
sf::Texture texture("assests/bg.png");
struct Cell {
  int number = 0;
  int x; // obvious
  int y; // obvious
  bool isSelected;
  bool canBeChanged;
  Cell(int x1, int y1) {
    x = x1;
    y = y1;
    isSelected = false;
    canBeChanged = false;
  }
};
bool unUsedInBox(std::vector<Cell> &grid, int rowStart, int colStart, int num) {
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
std::vector<Cell> cells;
sf::Time elapsed1; // this variable for storing the time elapsed
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
void sudokuGenerator(int k) {
  fillDiagonal(cells);
  solveSudoku(cells, 0, 0);
  removeKDigits(cells, 3);
}

void clearSelected(int index) { cells[index].isSelected = false; }
bool checkForVictory() { return isValid(cells); }
void fillCell() {
  for (int a = 0; a < 9; a++) {
    for (int b = 0; b < 9; b++) {
      cells.push_back(
          Cell(b, a)); // inversed as SFML has invered axis for some reason
    }
  }
  sudokuGenerator(10);
}
int getMousPos(sf::RenderWindow &window) {
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
  mouse_y = mouse_y - 3 ;
  if(mouse_y<0){
    return 0;
  }
  if (cells[mouse_x + mouse_y * 9].canBeChanged)
    cells[mouse_x + mouse_y * 9].isSelected = true;
  return (mouse_x + mouse_y * 9);
}

void startScreen(bool startedPlaying, sf::RenderWindow &window) {
  if (!startedPlaying) {
    sf::RectangleShape play({90, 60});
    play.setPosition({300, 300});
    play.setFillColor(sf::Color::White);
    sf::Font font("assests/type.ttf");
    sf::Text text(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setString("PLAY");
    window.draw(play);
    text.setPosition({300, 300});
    window.draw(text);
  }
}
void drawRectangles(sf::RenderWindow &window, bool isWin, bool isStarted) {
  if (isStarted) {
    sf::Font font("assests/type.ttf");
    sf::Text text(font);
    text.setCharacterSize(64);
    text.setFillColor(sf::Color::Black);
    if (isWin) {
      int timeTaken = (elapsed1.asSeconds());
      text.setFillColor(sf::Color::White);
      text.setPosition({200,300});
      text.setString("YOU WIN!\n" + std::to_string(timeTaken)+" seconds");
      window.draw(text);
      return;
    }
    sf::RectangleShape rectangle({CELL_SIZE, CELL_SIZE});
    sf::RectangleShape line({5, 1100});
    line.setFillColor(sf::Color::Black);
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (cells[i + j * 9].isSelected) {
          rectangle.setFillColor(sf::Color(99, 99, 99));
        } else
          rectangle.setFillColor(sf::Color::White);

        /*  rectangle.setOutlineThickness(6);*/
        /*  rectangle.setOutlineColor(sf::Color::Black);*/
        /*  rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE *
         * j)});*/
        /*  window.draw(rectangle);*/
        /*} else {*/
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)+offset});
        rectangle.setTexture(&texture);
        window.draw(rectangle);
        text.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)+offset});
        if (cells[i + j * 9].number != 0) {
          std::string String = std::to_string(cells[i+j*9].number);
          String = " " + String;
          text.setString(String);
          window.draw(text);
        }
        if ((i % 3 == 0 && i != 0)) {
          line.setSize({5,1100});
          line.setPosition({float(CELL_SIZE * i - 5), 0});
          window.draw(line);
        }
        else if ((j % 3 == 0 && j != 0)) {
          line.setSize({1100,5});
          line.setPosition({0, float(CELL_SIZE*j)+offset});
          window.draw(line);
        }
      }
    }
  }
}

int main() {
  bool start = false;
  srand(time(0));
  fillCell();
  sf::Clock clock;
  bool isWin = false;
  auto window = sf::RenderWindow(sf::VideoMode({9 * 80 , 9 * 80 + offset}), "Sudoku");
  window.setFramerateLimit(144);
  int index = -1;
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (checkForVictory()) {
        isWin = true;
        elapsed1 = clock.getElapsedTime();
        clock.stop();
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !start) {
        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;
        if ((x < 380 && x > 300) && (y < 380 && y > 300)) {
          start = true;
        }
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (index != -1)
          clearSelected(index);
        index = getMousPos(window);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
        if (index != -1) {
          cells[index].number = 1;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
        if (index != -1) {
          cells[index].number = 2;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
        if (index != -1) {
          cells[index].number = 3;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4)) {
        if (index != -1) {
          cells[index].number = 4;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5)) {
        if (index != -1) {
          cells[index].number = 5;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num6)) {
        if (index != -1) {
          cells[index].number = 6;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num7)) {
        if (index != -1) {
          cells[index].number = 7;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num8)) {
        if (index != -1) {
          cells[index].number = 8;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num9)) {
        if (index != -1) {
          cells[index].number = 9;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num0)) {
        if (index != -1) {
          cells[index].number = 0;
          clearSelected(index);
          index = -1;
        }
      }
    }
    window.clear();
    startScreen(start, window);
    drawRectangles(window, isWin, start);
    window.display();
  }
}
