# Reversi (Othello)

A command-line Reversi (Othello) game written in C++, where you play against a simple AI that picks moves based on position weights.

## Features

- Standard 8×8 Reversi board
- Choose to play Black (`$`, moves first) or White (`*`)
- Legal move validation: only moves that flip at least one opponent piece are accepted
- Automatic flipping of sandwiched opponent pieces
- Simple AI: the computer picks the move with the highest position weight (corners best, edges next)
- A turn is skipped automatically when a player has no valid moves
- Game ends and scores are tallied when neither player has a valid move
- Robust input handling: invalid input is rejected with a prompt to retry; EOF (Ctrl+C / Ctrl+Z) exits cleanly

## Rules Overview

- Black moves first; players alternate turns.
- A move must sandwich one or more opponent pieces in any of the eight directions; those pieces are flipped to your color.
- If a player has no valid move, their turn is skipped.
- The game ends when the board is full or neither player can move; the player with more pieces wins.

## Building

Requires a C++11 or later compiler (the code uses `<thread>` and `<chrono>`).

### Windows (MSVC / cl.exe)

```bat
cl.exe /Zi /EHsc /nologo /FeReversi.exe main.cpp
```

### Linux / macOS (g++ / clang++)

```bash
g++ -std=c++11 -O2 -o reversi main.cpp
```

> Note: On Linux/macOS you may need to add `-pthread` to link the thread library.

## Running

```text
./reversi        # Linux/macOS
Reversi.exe      # Windows
```

## How to Play

When the program starts, it asks you to pick a side:

```text
=====================
Choose your side:
1. Black ($ moves first)
2. White (* moves first)
Your choice:
```

Enter `1` to play Black (first), or `2` to play White (second).

When it is your turn, enter the **row and column** (0–7, separated by a space) where you want to place a piece, for example:

```text
You are '$'. It's your turn.
2 3
```

This places a piece at row 2, column 3 (coordinates are 0-based, with the origin at the top-left corner).

Example board display (`.` is empty, `$` is Black, `*` is White):

```text
  0 1 2 3 4 5 6 7
0 . . . . . . . .
1 . . . . . . . .
2 . . . * . . . .
3 . . . $ * . . .
4 . . * * $ . . .
5 . . . . . . . .
6 . . . . . . . .
7 . . . . . . . .
```

When the game ends, the program prints both players' piece counts and the result:

```text
=== Game Over
Black ($):34, White(*):30
Black($) wins!
```

## Project Structure

```text
Reversi/
├── main.cpp        # All game code
├── README.md       # This file
└── LICENSE         # License
```

## Code Layout (main.cpp)

| Function | Description |
| --- | --- |
| `main` | Game main loop; handles turn switching and player input |
| `initBoard` | Initializes the 8×8 board and the four starting pieces |
| `chooseSides` | Lets the player pick Black or White |
| `displayBoard` | Prints the current board |
| `isValidMove` | Checks whether a position is a legal move for a player |
| `canFlipInDirection` | Checks whether opponent pieces can be flipped in one direction |
| `placePieceAndFlip` | Places a piece and flips all sandwiched opponent pieces |
| `flipPieceInDirection` | Flips pieces along one direction |
| `hasValidMoves` | Checks whether a player has any legal move |
| `computerMakeMove` | AI selects and plays the best move |
| `getPositionScore` | Returns the position weight score of a cell |
| `announceWinner` | Counts pieces and announces the winner |

## AI Strategy

The computer uses a **position-weight greedy** strategy: it scores every legal move and plays the one with the highest weight. Corners are worth the most (50), edges next (30), while cells adjacent to corners score lowest (5/10) because they let the opponent take a corner.

## License

See [LICENSE](LICENSE).
