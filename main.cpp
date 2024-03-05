//  Copyright 2024 Vy Tran

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <level_file>" << std::endl;
        return 1;
    }

    std::string levelFilePath = argv[1];
    std::ifstream levelFile(levelFilePath);
    SB::Sokoban game;

    if (levelFile) {
        levelFile >> game;
    } else {
        std::cerr << "Failed to open level file: " << levelFilePath << std::endl;
        return 1;
    }

    // Print the game board to console to confirm it works
    std::cout << game;

    sf::RenderWindow window(
        sf::VideoMode(game.width() * game.tileSize, game.height() * game.tileSize),
        "Sokoban Game");

    sf::Clock clock;  // Start the clock
    while (window.isOpen()) {
        // bool isWon = game.isWon();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                // always allow restart
                if (event.key.code == sf::Keyboard::R) {
                    game.restart();
                    clock.restart();
                } else if (event.key.code == sf::Keyboard::Right) {
                    game.movePlayer(SB::Direction::Right);
                } else if (event.key.code == sf::Keyboard::Left) {
                    game.movePlayer(SB::Direction::Left);
                } else if (event.key.code == sf::Keyboard::Up) {
                    game.movePlayer(SB::Direction::Up);
                } else if (event.key.code == sf::Keyboard::Down) {
                    game.movePlayer(SB::Direction::Down);
                }
            }
        }

        sf::Time elapsedTime = clock.getElapsedTime();
        int totalSeconds = static_cast<int>(elapsedTime.asSeconds());
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        // Update window title with elapsed time in MM:SS format
        std::stringstream titleStream;
        titleStream << "Sokoban Game - " << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds;
        window.setTitle(titleStream.str());

        window.clear();
        window.draw(game);
        window.display();
    }

    return 0;
}
