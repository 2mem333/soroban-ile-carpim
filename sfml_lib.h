#include <SFML/Graphics.hpp>

class TextBox {
public:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    bool isSelected = false;
    std::string input;

    TextBox(float x, float y, float width, float height) {
        font.loadFromFile("CenturyGothic.ttf"); // Font dosyasýný uygun dizine koyduðundan emin ol

        box.setPosition(x, y);
        box.setSize(sf::Vector2f(width, height));
        box.setFillColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::Black);

        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x - 2 , y - 5 );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(text);
    }

    void handleEvent(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                isSelected = true;
                box.setOutlineColor(sf::Color::Blue);
            }
            else {
                isSelected = false;
                box.setOutlineColor(sf::Color::Black);
            }
        }

        if (isSelected && event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                input += static_cast<char>(event.text.unicode);
                text.setString(input);
            }
            else if (event.text.unicode == '\b' && !input.empty()) { // Backspace
                input.pop_back();
                text.setString(input);
            }
        }
    }

    int getValue() {
        if (!input.empty()) {
            return std::stoi(input);
        }
        return 0;
    }
};
