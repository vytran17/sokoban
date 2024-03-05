//  Copyright 2024 Vy Tran

#include "Sokoban.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <SFML/Audio.hpp>

namespace SB {

Sokoban::Sokoban() : boardWidth(0), boardHeight(0), playerPosition(0, 0) {
    tileTextures.resize(static_cast<size_t>(4));  // We have 4 total types of texture.

    // Load textures corresponding to each Tile type
    if (!tileTextures[static_cast<size_t>(Tile::Wall)].loadFromFile("assets/block_06.png")) {
        std::cerr << "Warning: Failed to load wall texture." << std::endl;
    }
    if (!tileTextures[static_cast<size_t>(Tile::Box)].loadFromFile("assets/crate_03.png")) {
        std::cerr << "Warning: Failed to load box texture." << std::endl;
    }
    if (!tileTextures[static_cast<size_t>(Tile::Empty)].loadFromFile("assets/ground_01.png")) {
        std::cerr << "Warning: Failed to load empty texture." << std::endl;
    }
    if (!tileTextures[static_cast<size_t>(Tile::Storage)].loadFromFile("assets/ground_04.png")) {
        std::cerr << "Warning: Failed to load storage texture." << std::endl;
    }
    if (!playerTextureLeft.loadFromFile("assets/player_20.png")) {
        std::cerr << "Warning: Failed to load player L texture." << std::endl;
    }
    if (!playerTextureRight.loadFromFile("assets/player_17.png")) {
        std::cerr << "Warning: Failed to load player R texture." << std::endl;
    }
    if (!playerTextureUp.loadFromFile("assets/player_08.png")) {
        std::cerr << "Warning: Failed to load player UP texture." << std::endl;
    }
    if (!playerTextureDown.loadFromFile("assets/player_05.png")) {
        std::cerr << "Warning: Failed to load player UP texture." << std::endl;
    }
    if (!winTexture.loadFromFile("assets/win.png")) {
        std::cerr << "Warning: Failed to load win note." << std::endl;
    }
    if (!winSoundBuffer.loadFromFile("assets/win.wav")) {
        std::cerr << "Warning: Failed to load win sound." << std::endl;
    } else {
        winSound.setBuffer(winSoundBuffer);
    }
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw tiles
    for (int y = 0; y < boardHeight; ++y) {
        for (int x = 0; x < boardWidth; ++x) {
            Tile tileType = gameBoard[y * boardWidth + x];
            sf::Sprite tileSprite;
            tileSprite.setTexture(tileTextures[static_cast<size_t>(tileType)]);

            // Calculate the scale factors to fit the texture within tileSize
            float scaleX = static_cast<float>(tileSize) / tileSprite.getTexture()->getSize().x;
            float scaleY = static_cast<float>(tileSize) / tileSprite.getTexture()->getSize().y;
            tileSprite.setScale(scaleX, scaleY);

            tileSprite.setPosition(
                static_cast<float>(x * tileSize),
                static_cast<float>(y * tileSize));
            target.draw(tileSprite, states);
        }
    }

    // Draw the player
    sf::Sprite playerSprite;
    switch (latestMove) {
        case Direction::Up:
            playerSprite.setTexture(playerTextureUp);
            break;
        case Direction::Down:
            playerSprite.setTexture(playerTextureDown);
            break;
        case Direction::Left:
            playerSprite.setTexture(playerTextureLeft);
            break;
        case Direction::Right:
            playerSprite.setTexture(playerTextureRight);
            break;
    }

    // Calculate the scale factors for the player texture
    float playerScaleX = static_cast<float>(tileSize) / playerSprite.getTexture()->getSize().x;
    float playerScaleY = static_cast<float>(tileSize) / playerSprite.getTexture()->getSize().y;
    playerSprite.setScale(playerScaleX, playerScaleY);

    playerSprite.setPosition(static_cast<float>(
        playerPosition.x * tileSize),
        static_cast<float>(playerPosition.y * tileSize));
    target.draw(playerSprite, states);

    // Draw win if won.
    if (isWon()) {
        sf::Sprite winSprite;
        winSprite.setTexture(winTexture);

        // draw in the middle
        sf::FloatRect spriteRect = winSprite.getLocalBounds();
        winSprite.setOrigin(spriteRect.width / 2.0f, spriteRect.height / 2.0f);
        sf::Vector2u targetSize = target.getSize();
        winSprite.setPosition(targetSize.x / 2.0f, targetSize.y / 2.0f);
        target.draw(winSprite, states);
    }
}

int Sokoban::width() const {
    return boardWidth;
}

int Sokoban::height() const {
    return boardHeight;
}

sf::Vector2i Sokoban::playerLoc() const {
    return playerPosition;
}

void Sokoban::movePlayer(Direction direction) {
    if (isWon()) {
        return;
    }
    latestMove = direction;

    int deltaX = 0;
    int deltaY = 0;
    switch (direction) {
      case SB::Direction::Right:
        deltaX = 1;
        break;
      case SB::Direction::Left:
        deltaX = -1;
        break;
      case SB::Direction::Up:
        deltaY = -1;
        break;
      case SB::Direction::Down:
        deltaY = 1;
    }

    // Check if Player is out of bounds.
    int newPlayerX = playerPosition.x + deltaX;
    int newPlayerY = playerPosition.y + deltaY;
    if (isOutOfBounds(newPlayerX, newPlayerY)) {
        return;
    }

    // Check if new position is empty or box.
    Tile targetTile = gameBoard[newPlayerY * boardWidth + newPlayerX];
    if (targetTile == Tile::Empty || targetTile == Tile::Storage) {
        playerPosition.x = newPlayerX;
        playerPosition.y = newPlayerY;
    } else if (targetTile == Tile::Box) {
        // Push box if there's space behind it.
        int newBoxX = newPlayerX + deltaX;
        int newBoxY = newPlayerY + deltaY;
        // Check if tile behind box is out of bounds.
        if (isOutOfBounds(newBoxX, newBoxY)) {
            return;
        }

        Tile tileBehindBox = gameBoard[newBoxY * boardWidth + newBoxX];
        if (tileBehindBox == Tile::Empty || tileBehindBox == Tile::Storage) {
            // The tile behind box is now the box.
            gameBoard[newBoxY * boardWidth + newBoxX] = Tile::Box;

            // The tile where box was should get reverted to it's original tile.
            Tile original = originalGameBoard[newPlayerY * boardWidth + newPlayerX];
            if (original == Tile::Storage) {
                gameBoard[newPlayerY * boardWidth + newPlayerX] = Tile::Storage;
            } else if (original == Tile::Empty) {
                gameBoard[newPlayerY * boardWidth + newPlayerX] = Tile::Empty;
            } else if (original == Tile::Box) {
                // if it was box from the very start make it empty space
                gameBoard[newPlayerY * boardWidth + newPlayerX] = Tile::Empty;
            }
            // Finally move player to where box was.
            playerPosition.x = newPlayerX;
            playerPosition.y = newPlayerY;

            // If we won play sound.
            if (isWon() && winSound.getStatus() != sf::Sound::Playing) {
                winSound.play();
            }
        }
    }
}

bool Sokoban::isWon() const {
    int storageCount = 0;
    int boxOnStorageCount = 0;
    int boxCount = 0;

    for (auto i = 0u; i < gameBoard.size(); ++i) {
        // Count how many storage locations are in the original game board
        if (originalGameBoard[i] == Tile::Storage) {
            ++storageCount;
            // Check if a box is placed on a storage location in the current game state
            if (gameBoard[i] == Tile::Box) {
                ++boxOnStorageCount;
            }
        }
        // Count the total number of boxes in the current game state
        if (gameBoard[i] == Tile::Box) {
            ++boxCount;
        }
    }

    // The game is won if all storage locations have boxes OR all boxes are on storage locations
    return boxOnStorageCount == storageCount || boxOnStorageCount == boxCount;
}

void Sokoban::restart() {
    playerPosition = originalPlayerPosition;
    gameBoard = originalGameBoard;
    latestMove = Direction::Down;
}

std::istream& operator>>(std::istream& in, Sokoban& game) {
    std::string line;

    // First line contains the dimensions
    if (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> game.boardHeight >> game.boardWidth;

        // Resize the game board to hold the level layout
        game.gameBoard.resize(game.boardWidth * game.boardHeight);

        // Read the level layout line by line
        for (int y = 0; y < game.boardHeight && std::getline(in, line); ++y) {
            int lineSize = line.size();
            for (int x = 0; x < game.boardWidth && x < lineSize; ++x) {
                char tile = line[x];
                switch (tile) {
                    case '#':  // Wall
                        game.gameBoard[y * game.boardWidth + x] = Tile::Wall;
                        break;
                    case '@':  // Player
                        // Player position is marked as empty because player can move
                        game.playerPosition = sf::Vector2i(x, y);
                        game.gameBoard[y * game.boardWidth + x] = Tile::Empty;
                        break;
                    case '.':  // Empty space
                        game.gameBoard[y * game.boardWidth + x] = Tile::Empty;
                        break;
                    case 'A':  // Box
                        game.gameBoard[y * game.boardWidth + x] = Tile::Box;
                        break;
                    case 'a':  // Storage location
                        game.gameBoard[y * game.boardWidth + x] = Tile::Storage;
                        break;
                    default:  // For any unrecognized character, show a warning
                        std::cerr << "Invalid tile character: " << std::string(1, tile);
                        break;
                }
            }
        }

        // Preserve a copy of original game board & player location so we can reset later.
        game.originalGameBoard = game.gameBoard;
        game.originalPlayerPosition = game.playerPosition;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Sokoban& game) {
    for (int y = 0; y < game.boardHeight; ++y) {
        for (int x = 0; x < game.boardWidth; ++x) {
            if (game.playerPosition == sf::Vector2i(x, y)) {
                out << '@';  // Player character
            } else {
                // Output a character based on the tile type
                switch (game.gameBoard[y * game.boardWidth + x]) {
                    case Tile::Wall:
                        out << '#';
                        break;
                    case Tile::Box:
                        out << 'A';
                        break;
                    case Tile::Storage:
                        out << 'a';
                        break;
                    case Tile::Empty:
                    default:
                        out << '.';
                        break;
                }
            }
        }
        out << '\n';  // New line at the end of each row
    }
    return out;
}

bool Sokoban::isOutOfBounds(int x, int y) {
    return (x < 0 || y < 0 || x >= boardWidth || y >= boardHeight);
}

}  // namespace SB
