# PacMan2024

[![Watch the video](https://github.com/mongoliangull/PacMan2024/raw/main/PacMan2024_thumbnail.png)](https://github.com/mongoliangull/PacMan2024/raw/main/PacMan2024.mp4)

## Introduction
This project is a recreation of the classic Pac-Man game, originally developed and released by Namco in 1980. The game follows the traditional Pac-Man mechanics, where players navigate a maze while avoiding ghosts and consuming dots. The game includes various features such as ghost AI, scoring mechanics, and different game states.

## Game Rules
- The player controls Pac-Man, who moves in four directions (left, right, up, down) at a uniform velocity.
- Pac-Man must eat all dots in the maze to win.
- Pac-Man must avoid ghosts unless they are in the frightened state.
- If Pac-Man collides with a ghost in chase or scatter mode, he loses a life.
- The game ends when Pac-Man loses all lives.

## Game Components
- **Pac-Man:** The main character controlled by the player.
- **Ghosts:** Four enemy characters (Blinky, Pinky, Inky, Clyde) that chase Pac-Man using different AI strategies.
- **Maze:** A structured environment with walls that restrict movement.
- **Dots:** Two types of dots (regular and power pellets) placed in the maze.

## Ghost AI Behavior
Ghosts have different movement patterns depending on their state:
1. **Chase Mode**: Each ghost has a unique targeting method:
   - **Blinky**: Directly follows Pac-Man.
   - **Pinky**: Targets four spaces ahead of Pac-Man.
   - **Inky**: Uses a mirrored position relative to Blinky.
   - **Clyde**: Chases Pac-Man but switches to scatter mode if close.
2. **Scatter Mode**: Each ghost moves toward its designated corner.
3. **Frightened Mode**: Ghosts move randomly when Pac-Man eats a power pellet.
4. **Eaten Mode**: Ghosts return to their initial positions.

## Features
- Classic Pac-Man gameplay
- AI-controlled ghost movement
- Collision detection for eating dots and interacting with ghosts
- Life system for Pac-Man
- Victory condition when all dots are eaten

## Bonus Features (If Implemented)
- 3D rendering with lighting and shading
- Sound effects and background music
- Multiple game levels
- Scoreboard for tracking high scores
- Multiplayer functionality
- Special Pac-Man abilities

## How to Play
1. Start the game and control Pac-Man using the arrow keys.
2. Navigate through the maze while avoiding ghosts.
3. Eat all dots to win the game.
4. Collect power pellets to turn ghosts frightened and eat them.
5. Avoid being caught by ghosts in chase or scatter mode.
6. Try to achieve the highest score possible!

## References
- [Pac-Man Wikipedia](https://en.wikipedia.org/wiki/Pac-Man)
- [Ghost AI Explanation](https://udaqueness.blog/2020/02/11/%ED%8C%A9%EB%A7%A8-%EC%9C%A0%EB%A0%B9%EB%93%A4%EC%9D%98-ai/)

## License
This project is for educational purposes and does not claim ownership of the original Pac-Man game developed by Namco.

