#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <random>
#include <string>
#include <vector>
#include <iostream>

#define N 9 
#define M 9
#define CELL_SIZE 80



struct Cell{
  int number = 1;
  int x;         // obvious
  int y;         // obvious
  Cell(int x1, int y1)
  {
    x = x1;
    y = y1;
  }
};

std::vector<Cell> cells;


sf::Text DrawTextForBoard()
{
  sf::Font font("assests/EmojiFont.ttf");
  sf::Text text(font);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  return text;
}
void generateSudoku(){
    std::random_device rd;
    std::mt19937 gen(rd());                         // seeding the number generator
    std::uniform_int_distribution<> distrib(1, 8);
    for(int i = 0 ; i<81; i++){
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

void getMousPos(sf::RenderWindow &window)
{
  int mouse_x = sf::Mouse::getPosition(window).x / CELL_SIZE;
  int mouse_y = sf::Mouse::getPosition(window).y / CELL_SIZE;
  std::cout<<cells[mouse_x + mouse_y * 9].number;
}
void drawRectangles(sf::RenderWindow &window){
  sf::Font font("assests/Greek-Freak.ttf");
  sf::Text text(font);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
    sf::RectangleShape rectangle({CELL_SIZE, CELL_SIZE});
    for(int i = 0 ; i<9 ; i++){
        for(int j = 0 ; j<9 ; j++){
            text.setPosition({float(CELL_SIZE*i),float(CELL_SIZE*j)});
            text.setString(std::to_string(cells[i+j*9].number));
            rectangle.setPosition({float(CELL_SIZE*i) ,float(CELL_SIZE*j) });
            rectangle.setOutlineThickness(10);
            rectangle.setOutlineColor(sf::Color::Black);            
            window.draw(rectangle);
            window.draw(text);
        }
    }
}


int main()
{
   fillCell();
    auto window = sf::RenderWindow(sf::VideoMode({9*80u, 9*80u}), "Sudoku");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            while (const std::optional event = window.pollEvent())
            {
                // "close requested" event: we close the window
                if (event->is<sf::Event::Closed>()){
                    window.close();
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                    getMousPos(window);
                }
            }
            window.clear();
            drawRectangles(window);
            window.display();

        }
    }
}
