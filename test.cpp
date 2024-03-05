//  Copyright 2024 Vy Tran

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/test/unit_test.hpp>

#include "Sokoban.hpp"

namespace SB {

BOOST_AUTO_TEST_CASE(playerMovement) {
    Sokoban sb;
    std::ifstream levelFile("assets/level1.lvl");
    levelFile >> sb;

    int initialPlayerX = sb.playerLoc().x;
    int initialPlayerY = sb.playerLoc().y;

    sb.movePlayer(Direction::Up);

    BOOST_CHECK_EQUAL(sb.playerLoc().y, initialPlayerY - 1);

    sb.movePlayer(Direction::Down);
    BOOST_CHECK_EQUAL(sb.playerLoc().y, initialPlayerY);

    sb.movePlayer(Direction::Left);

    BOOST_CHECK_EQUAL(sb.playerLoc().x, initialPlayerX - 1);

    sb.movePlayer(Direction::Right);
    BOOST_CHECK_EQUAL(sb.playerLoc().x, initialPlayerX);
}

BOOST_AUTO_TEST_CASE(victoryTest) {
    Sokoban sb;
    std::ifstream levelFile("assets/level1.lvl");
    levelFile >> sb;

    BOOST_CHECK(!sb.isWon());

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);

    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Down);

    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);

    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Up);

    BOOST_CHECK(sb.isWon());
}

// Move off-screen Test
BOOST_AUTO_TEST_CASE(offScreenTest) {
    Sokoban sb;
    std::ifstream levelFile("assets/level4.lvl");
    levelFile >> sb;

    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);

    auto playerPosition = sb.playerLoc();

    BOOST_CHECK_EQUAL(playerPosition.x, 11);
    BOOST_CHECK_EQUAL(playerPosition.y, 4);
}

// Push box off-screen Test
BOOST_AUTO_TEST_CASE(pushOffBox) {
    Sokoban sb;
    std::ifstream levelFile("assets/level4.lvl");
    levelFile >> sb;

    // This move attempts to push the box off-screen
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);

    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);

    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Down);

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Down);

    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Left);

    // Check player's positions - it should not
    // have pushed box off edge and stepped on edge itself
    auto playerPosition = sb.playerLoc();
    BOOST_CHECK_EQUAL(playerPosition.x, 1);
    BOOST_CHECK_EQUAL(playerPosition.y, 4);
}

// Push box into wall
BOOST_AUTO_TEST_CASE(boxWallTest) {
    Sokoban sb;
    std::ifstream levelFile("assets/level1.lvl");
    levelFile >> sb;

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);

    auto playerPosition = sb.playerLoc();

    // Box cannot push wall, therefore player still unmoved.
    BOOST_CHECK_EQUAL(playerPosition.x, 7);
    BOOST_CHECK_EQUAL(playerPosition.y, 6);
}

// Push box into box
BOOST_AUTO_TEST_CASE(boxBoxTest) {
    Sokoban sb;
    std::ifstream levelFile("assets/level1.lvl");
    levelFile >> sb;

    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Left);

    auto playerPosition = sb.playerLoc();

    // Box cannot push box, therefore player still unmoved.
    BOOST_CHECK_EQUAL(playerPosition.x, 7);
    BOOST_CHECK_EQUAL(playerPosition.y, 2);
}

// test more boxes than storage rooms
BOOST_AUTO_TEST_CASE(manyBoxes) {
    Sokoban sb;
    std::ifstream levelFile("assets/level5.lvl");
    levelFile >> sb;

    BOOST_CHECK(!sb.isWon());

    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);

    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Up);

    BOOST_CHECK(sb.isWon());
}

// test more storage than boxes
BOOST_AUTO_TEST_CASE(manyStorages) {
    Sokoban sb;
    std::ifstream levelFile("assets/level6.lvl");
    levelFile >> sb;

    BOOST_CHECK(!sb.isWon());

    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Up);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Right);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Left);
    sb.movePlayer(Direction::Down);
    sb.movePlayer(Direction::Right);

    BOOST_CHECK(sb.isWon());
}

}  // namespace SB
