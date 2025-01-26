#include "./sudoku.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>

#define CELL_SIZE 80
#define offset 240
sf::Texture texture("assests/bg.png");
sf::Font font("assests/type.ttf");
sf::Text text(font);
std::vector<Cell> cells;
sf::Time elapsed1;
sf::Image image("assests/sudoku.png");
sf::Texture drawing("assests/drawing.png");
sf::Texture notDrawing("assests/not-drawing.png");
sf::Sprite pencilIcon(notDrawing);
void sudokuGenerator(int k) {
  Sudoku *suk = new Sudoku();
  suk->fillDiagonal(cells);
  suk->solveSudoku(cells, 0, 0);
  suk->removeKDigits(cells, k);
  delete suk;
}

/**
 * Checks if puzzle was solved
 */
bool checkForVictory() {
  Sudoku *suk = new Sudoku();
  bool win = suk->isValid(cells);
  delete suk;
  return win;
}

void pencilAppend(std::string numToAdd, int index) {
  cells[index].isPencil = true;
  int found = cells[index].pencilNums.find(numToAdd);
  if (found == -1) {
    cells[index].pencilNums.append(numToAdd);
    int len = cells[index].pencilNums.size();
    if (len == 5 || len == 11) {
      cells[index].pencilNums.append("\n");
    } else {
      cells[index].pencilNums.append(" ");
    }
  } else {

    cells[index].pencilNums = cells[index].pencilNums.erase(found, 2);
  }
}
void clearSelected(int index) { cells[index].isSelected = false; }

/**
 * Generates a puzzle and stores it in `cells`
 */
void fillCell() {
  for (int a = 0; a < 9; a++) {
    for (int b = 0; b < 9; b++) {
      cells.push_back(Cell(b, a));
    }
  }
  sudokuGenerator(10);
}

/**
 * Returns 1D cell index from mouse's relative x y position
 */
int getMousPos(sf::RenderWindow &window) {
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
  mouse_y = mouse_y - offset/CELL_SIZE;
  if (mouse_y < 0) {
    return 0;
  }
  if (cells[mouse_x + mouse_y * 9].canBeChanged)
    cells[mouse_x + mouse_y * 9].isSelected = true;
  return (mouse_x + mouse_y * 9);
}
void highlight(int num) {
  for (int i = 0; i < 81; i++) {
    if (cells[i].number == num)
      cells[i].isHighlight = true;
  }
}

/**
 * Clears highlighting of all cells
 */
void clearHighlight() {
  for (int i = 0; i < 81; i++) {
    cells[i].isHighlight = false;
  }
}

/**
 * Shows initial play game screen
 */
void startScreen(bool startedPlaying, sf::RenderWindow &window) {
  if (!startedPlaying) {
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Cyan);
    text.setString("PLAY");
    text.setPosition({300, 100});
    window.draw(text);
  }
}

/**
 * Draw board
 */
void drawRectangles(sf::RenderWindow &window, bool isWin, bool isStarted) {
  if (isStarted) {
    if (isWin) {
      window.clear();
      int timeTaken = (elapsed1.asSeconds());
      text.setFillColor(sf::Color::White);
      text.setPosition({200, 300});
      text.setString("YOU WIN!\n" + std::to_string(timeTaken) + " seconds");
      window.draw(text);
      return;
    }

    sf::RectangleShape rectangle({CELL_SIZE, CELL_SIZE});
    sf::RectangleShape subgridSeparator({5, 1100});
    subgridSeparator.setFillColor(sf::Color::Black);

    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {

        if (cells[i + j * 9].isSelected) {
          rectangle.setFillColor(sf::Color(99, 99, 99));
        } else if (cells[i + j * 9].isHighlight) {
          rectangle.setFillColor(sf::Color::Magenta);
        } else {
          rectangle.setFillColor(sf::Color::White);
        }

        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition(
            {float(CELL_SIZE * i), float(CELL_SIZE * j) + offset});
        rectangle.setTexture(&texture);
        window.draw(rectangle);

				// draw inner text
        text.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j) + offset});
        if (cells[i + j * 9].canBeChanged && !cells[i + j * 9].isPencil &&
            cells[i + j * 9].number != 0) {
          text.setCharacterSize(64);
          text.setFillColor(sf::Color::Black);
          std::string String = std::to_string(cells[i + j * 9].number);
          String = " " + String;
          text.setString(String);
          window.draw(text);
        } else if (cells[i + j * 9].isPencil) {
          text.setCharacterSize(21);
          text.setFillColor(sf::Color::Black);
          text.setString(cells[i + j * 9].pencilNums);
          window.draw(text);
        } else if (cells[i + j * 9].number != 0) {
          std::string String = std::to_string(cells[i + j * 9].number);
          String = " " + String;
          text.setString(String);
          text.setCharacterSize(64);
          text.setFillColor(sf::Color(99, 62, 21));
          window.draw(text);
        } else {
          // random
        }

				// draw subgrid separator
        if ((i % 3 == 0 && i != 0)) {
					// vertical line
          subgridSeparator.setSize({5, 1100});
          subgridSeparator.setPosition({float(CELL_SIZE * i - 5), 0});
          window.draw(subgridSeparator);
        } else if ((j % 3 == 0 && j != 0)) {
					// horizontal line
          subgridSeparator.setSize({1100, 5});
          subgridSeparator.setPosition({0, float(CELL_SIZE * j) + offset});
          window.draw(subgridSeparator);
        }
      }
    }
  }
}

/**
 * shows time on the screen
 */
void drawTime(sf::RenderWindow &window, sf::Time time, bool isStart) {
  if (isStart) {
		// show pencil icon
    pencilIcon.scale({1, 1});
    pencilIcon.setPosition({30, 100});
    window.draw(pencilIcon);


		////// show time ///////
    text.setPosition({320, 0});
    text.setFillColor(sf::Color::White);
    std::string str = std::to_string(time.asSeconds());
    std::string newStr = "";

		// skip decimal part
    for (int i = 0; i < str.length(); i++) {
      if (str[i] == '.')
        break;
      newStr.append(1, str[i]);
    }
    text.setString(newStr);
    window.draw(text);
  }
}

int main() {
  bool isPencil = false;
  bool start = false;
  bool isWin = false;

  srand(time(0));
  fillCell();

  sf::Clock clock;
  clock.stop();

  sf::RenderWindow window = sf::RenderWindow(
			sf::VideoMode({9 * CELL_SIZE, 9 * CELL_SIZE + offset}),
			"Sudoku",
			sf::Style::Titlebar | sf::Style::Close
	);

  window.setIcon(image.getSize(), image.getPixelsPtr());
  window.setFramerateLimit(144);
  int index = -1;
  while (window.isOpen()) {
    elapsed1 = clock.getElapsedTime();
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
        if ((x < 400 && x > 300) && (y < 200 && y > 100)) {
          start = true;
          index = -1;
          clock.reset();
          clock.start();
          clearHighlight();
        }
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        clearHighlight();
        if (index != -1)
          clearSelected(index);
        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;
        if ((x >= 30 && y >= 100 && y <= 200 && x <= 120)) {
          if (isPencil) {
            pencilIcon.setTexture(notDrawing);
            isPencil = false;
          } else {
            pencilIcon.setTexture(drawing);
            isPencil = true;
          }
        }
        index = getMousPos(window);
        if(index!=-1 && !cells[index].canBeChanged){
          highlight(cells[index].number);
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("1", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 1;
            highlight(1);
            cells[index].isPencil = false;
            cells[index].pencilNums = "";
            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("2", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 2;
            highlight(2);
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("3", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 3;
            cells[index].isPencil = false;
            highlight(3);
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("4", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 4;
            cells[index].isPencil = false;
            highlight(4);
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("5", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 5;
            clearSelected(index);
            highlight(5);
            cells[index].isPencil = false;
            cells[index].pencilNums = "";

            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num6)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("6", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 6;
            cells[index].isPencil = false;
            highlight(6);
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num7)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("7", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 7;
            cells[index].isPencil = false;
            highlight(7);
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num8)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("8", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 8;
            cells[index].isPencil = false;
            cells[index].pencilNums = "";
            highlight(8);

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num9)) {
        if (index != -1 && cells[index].canBeChanged) {
          if (isPencil) {
            pencilAppend("9", index);
            clearSelected(index);
            index = -1;
          } else {
            cells[index].number = 9;
            cells[index].isPencil = false;
            highlight(9);
            cells[index].pencilNums = "";

            clearSelected(index);
            index = -1;
          }
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num0)) {
        if (index != -1) {
          cells[index].number = 0;
          cells[index].isPencil = false;
          clearHighlight();
          cells[index].pencilNums = "";
          clearSelected(index);
          index = -1;
        }
      }
    }
    window.clear();
		
    startScreen(start, window);
    drawTime(window, elapsed1, start);
    drawRectangles(window, isWin, start);
    window.display();
  }
}
