# PS3b: Sokoban

## Contact
Name: Vy Tran
Section: 201
Time to Complete: 7 hours

## Description
The program is a simple implementation of the Sokoban game using the SFML library on C++. Sokoban is a puzzle game where the player, represented by a character, must push boxes onto a storage locations. The player can move in four cardinal directions, and the goal is to place all boxes on the designated storage locations. The players can move in four cardinal directions, and the goal is to place all boxes on the designated storage locations to win the game. The game features different textures for walls, empty spaces, boxes, and the player, creating a visual representation of the game board.

### Features
- Game Board/Tile = The game board is represented by a two-dimensional matrix grid, where each character corresponds to a specific element (image).
- Player movement = The player can move to the right, left, up and down and the image of the player follows the direction.
- Box pushing: The player can push boxes on all directions and from all directions with the exception to when there is a wall on the way.
- Storage areas: certain locations on the grid act as storage areas.
- Victory: The game is won is all boxes are pushed onto a storage space.

### Memory
The project uses a two-dimensional grid to store the game board, and the level data is represented by characters within this grid. The grid is implemented as a vector of vectors, providing a flexible data structure for managing the level layout. In this implementation, smart pointers are not used; instead, standard data structures, such vectors, are employed to manipulate and manage the game data.

### Lambdas
I decided to not use lambda function.

### Issues
- Data Structure and Logic: Implementing the data structure for the game board and the game logic itself seemed to be one of the initial challenges. Ensuring that the player's position, box positions, and storage locations are accurately represented and updated was a key issue.
- Loading images and textures: Loading and displaying images and textures in the game was another recurring challenge. Handling image loading, texture management, and displaying sprites with the apporpriate textures required multiple iterations to get right.
- Refactoring and redundancy: refactoring the code to eliminate redundancy and ensure that it adheres to best practices was an ongoing process. This included addressing issues realted to texture loading and using the appropriate data structures.
- Missing "1 A box that is already in a storage location" feature. I can't figure out how to do it. And I assume test case Missing symbol and TestSwap are to test that feature. So I'm not able to write these 2 test cases as well.

### Extra Credit
- Player changes direction while moving
- Plays victory sound: Used the SFML sf::Sound class to add sound feature. Though, the win sound doesn't work for autowin.lvl and so far I still can't figure out how to make it work for autowin.lvl

## Acknowledgements
class' materials and videos, google

### Sources
- Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban
- win.png: Canva https://www.canva.com/
- win.wav: Mixkit https://mixkit.co/free-sound-effects/win/