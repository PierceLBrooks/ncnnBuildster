#include "MainState.hpp"
#include <SFML/System/Vector2.hpp>

int main(int argc, char *argv[])
{
    sf::VideoMode screen(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    sf::View view = window.getDefaultView();

    sf::Color background = sf::Color::White;

    // We shouldn't try drawing to the screen while in background
    // so we'll have to track that. You can do minor background
    // work, but keep battery life in mind.
    bool active = true;
	
	sf::Vector2i cursor = sf::Vector2i(-1, -1);
	sfml::MainState* mainState = new sfml::MainState();
	mainState->window = sf::Vector2f(window.getSize());
	mainState->enter();

    while (window.isOpen())
    {
        sf::Event event;

        while (active ? window.pollEvent(event) : window.waitEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width/2, event.size.height/2);
                    window.setView(view);
                    break;
                case sf::Event::LostFocus:
                    background = sf::Color::Black;
                    break;
                case sf::Event::GainedFocus:
                    background = sf::Color::White;
                    break;
                // On Android MouseLeft/MouseEntered are (for now) triggered,
                // whenever the app loses or gains focus.
                case sf::Event::MouseLeft:
                    active = false;
                    break;
                case sf::Event::MouseEntered:
                    active = true;
                    break;
                case sf::Event::TouchBegan:
					mainState->drawing = true;
					cursor.x = event.touch.x;
					cursor.y = event.touch.y;
					break;
                case sf::Event::TouchMoved:
					cursor.x = event.touch.x;
					cursor.y = event.touch.y;
					break;
				case sf::Event::TouchEnded:
					mainState->drawing = false;
					cursor.x = event.touch.x;
					cursor.y = event.touch.y;
					break;
				case sf::Event::MouseMoved:
				    cursor.x = event.mouseMove.x;
				    cursor.y = event.mouseMove.y;
				    break;
				case sf::Event::MouseButtonPressed:
				    if (event.mouseButton.button == sf::Mouse::Button::Left) {
				        mainState->drawing = true;
				    }
				    cursor.x = event.mouseButton.x;
				    cursor.y = event.mouseButton.y;
				    break;
				case sf::Event::MouseButtonReleased:
				    if (event.mouseButton.button == sf::Mouse::Button::Left) {
				        mainState->drawing = false;
				    }
				    cursor.x = event.mouseButton.x;
				    cursor.y = event.mouseButton.y;
				    break;
            }
        }

        if (active) {
			mainState->cursor = sf::Vector2f(cursor);
			mainState->update();
			if (!mainState->drawing) {
			    cursor = sf::Vector2i(-1, -1);
			}
            window.clear(background);
            window.draw(*(mainState->drawableStack));
            window.display();
        } else {
            sf::sleep(sf::milliseconds(100));
        }
    }
	
	mainState->exit();
	delete mainState;

    return 0;
}
