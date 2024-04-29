This project aims to recreate the popular board game and computer game 'battleship' through the c++ terminal. The game has two phases: the first in which both player place all 5 of their ships of varying lengths on a 10x10 grid; and the second, in which the players take turns to hit each others grids until one player has destroyed all of the opponent's ships.


AI:
3 difficulty levels for AI:
Easy: Random selection of coordinates for execution of move
Normal: Random selection of coordinates for execution of move, but no move within one block of a destroyed ship (not possible to have a ship here)
Hard: Random selection of coordinates for execution of move 4/5 of the time, confirmed hit 1/5 of the time. No move within one block of a destroyed ship

True for all difficulty levels:
-) Random selection for placement of ships.
-) If a ship is hit, hit adjacent to the ship until orientation is found. When orientation is found, keeping hitting in the specified orientation until either the ship is totally destroyed, or a miss is reached. In case of miss, go in the opposite direction of the miss until the ship is totally destroyed.
