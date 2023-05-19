# -------------- AVOID CONSOLE GAME --------------- #



## TABLE OF CONTENTS
 * [General Information]
 * [Compiler Information]
 * [Controls]
 * [Game Rules]
 * [File Information]
 * [Creator Information]



## GENERAL INFORMATION
The program is an interactive console game written in the C language. To operate the program (game), a keyboard is required as the program takes input from it. The program only displays ASCII characters.




## COMPILER INFORMATION
 * gcc (Rev10, Built by MSYS2 project) 11.2.0
 * Copyright (C) 2021 Free Software Foundation, Inc.




## CONTROLS
 * Use arrow keys or 'WASD' keys to move your character.
 * Press 'R' to restart the level /* This costs -1 health */
 * Press 'x' to exit the game and terminate the program.




## GAME RULES
The objective of the game is to score as many points as possible. The points counter is located at the top of the screen. Points are earned by reaching the next level: to do so, the player must reach the "house" marked as the finish line.

At the beginning of each level, the player has 5 health points, which are displayed in the top left corner of the screen. Each level contains enemies that make the gameplay challenging. If the player is hit by an enemy, they lose one health point. If the player's health level reaches zero, the player loses. The game automatically starts a new gameplay session after 5 seconds, recording the highest score achieved in that session.

After pressing a directional key, the player character will start "flying" in that direction and cannot change direction until the character reaches a wall. Upon contact with a wall, the wall block will be damaged, and when the player moves away, it will disappear completely. This allows for creating new paths.

 * The player earns as many points as their remaining health when completing a level.
 * Restarting a level restores the player's starting position and all destroyed walls to their original state.
 * The game can only be exited by pressing the 'x' key.
 * The maximum score is shown within a session.



## FILE INFORMATION (for custom compilation)
The main file containing main.c is avoid.c. The remaining files contain modules and libraries. Due to the presence of the windows.h library, the program must be compiled on a Windows system.



## CREATOR INFORMATION
 * Miłosz Skobejko - Gdańsk University of Technology - 12.06.2022
