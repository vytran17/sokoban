//  Copyright 2024 Vy Tran

#ifndef SOKOBAN_H
#define SOKOBAN_H

#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace SB {

enum class Direction { Up, Down, Left, Right };
enum class Tile { Empty, Wall, Box, Storage };

// Sokoban class
class Sokoban : public sf::Drawable {
 public:
    Sokoban();

    // Override the draw method from sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Returns the width of the game board
    int width() const;

    // Returns the height of the game board
    int height() const;

    // Returns the player's current position
    sf::Vector2i playerLoc() const;

    // Moves the player in the specified direction
    void movePlayer(Direction direction);

    // Checks if the game is won
    bool isWon() const;

    // restart the game
    void restart();

    // Reads the level from a stream
    friend std::istream &operator>>(std::istream &in, Sokoban &game);

    // Writes the level to a stream (optional, for your convenience)
    friend std::ostream& operator<<(std::ostream& out, const Sokoban& game);

    static const int tileSize = 48;

 private:
    int boardWidth;
    int boardHeight;
    std::vector<Tile> gameBoard;
    std::vector<Tile> originalGameBoard;
    sf::Vector2i playerPosition;
    sf::Vector2i originalPlayerPosition;
    std::vector<sf::Texture> tileTextures;
    sf::Texture playerTextureRight;
    sf::Texture playerTextureLeft;
    sf::Texture playerTextureUp;
    sf::Texture playerTextureDown;
    sf::Texture winTexture;
    sf::SoundBuffer winSoundBuffer;
    sf::Sound winSound;
    Direction latestMove = Direction::Down;
    bool isOutOfBounds(int x, int y);
};

}  // namespace SB

#endif  // SOKOBAN_H
