#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <random>
#include <string>
#include <vector>

#define N 9
#define M 9
#define CELL_SIZE 80

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

std::vector<Cell> cells;

void clearSelected(int index)
{
  cells[index].isSelected = false;
}
sf::Text DrawTextForBoard()
{
  sf::Font font("assests/EmojiFont.ttf");
  sf::Text text(font);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  return text;
}
void generateSudoku()
{
  std::random_device rd;
  std::mt19937 gen(rd()); // seeding the number generator
  std::uniform_int_distribution<> distrib(1, 8);
  for (int i = 0; i < 81; i++)
  {
    cells[i].number = distrib(gen);
  }
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
  generateSudoku();
}

int getMousPos(sf::RenderWindow &window)
{
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
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
      if (cells[i + j * 9].isSelected)
        rectangle.setFillColor(sf::Color::Cyan);
      else
        rectangle.setFillColor(sf::Color::White);
      if ((j % 3 == 0 && j != 0) || (i % 3 == 0 && i != 0))
      {
        rectangle.setOutlineThickness(6);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
        window.draw(rectangle);
      }
      /*else if (i%3==0 && i!=0) {*/
      /*rectangle.setOutlineThickness(6);*/
      /*rectangle.setOutlineColor(sf::Color::Black);*/
      /*rectangle.setPosition({float(CELL_SIZE*i),float(CELL_SIZE*j)});*/
      /*window.draw(rectangle);*/
      /*}*/
      else
      {
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
        window.draw(rectangle);
      }
      text.setPosition({float(CELL_SIZE * i), float(CELL_SIZE * j)});
      text.setString(std::to_string(cells[i + j * 9].number));
      window.draw(text);
    }
  }
}

int main()
{
  fillCell();
  auto window = sf::RenderWindow(sf::VideoMode({9 * 80u, 9 * 80u}), "Sudoku");
  window.setFramerateLimit(144);
  int index = -1;
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = window.pollEvent())
    {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
      {
        index = getMousPos(window);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1))
      {
        if (index != -1)
        {
          cells[index].number = 1;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2))
      {
        if (index != -1)
        {
          cells[index].number = 2;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3))
      {
        if (index != -1)
        {
          cells[index].number = 3;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num4))
      {
        if (index != -1)
        {
          cells[index].number = 4;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num5))
      {
        if (index != -1)
        {
          cells[index].number = 5;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num6))
      {
        if (index != -1)
        {
          cells[index].number = 6;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num7))
      {
        if (index != -1)
        {
          cells[index].number = 7;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num8))
      {
        if (index != -1)
        {
          cells[index].number = 8;
          clearSelected(index);
          index = -1;
        }
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num9))
      {
        if (index != -1)
        {
          cells[index].number = 9;
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
