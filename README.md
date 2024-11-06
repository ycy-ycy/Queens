# Simulation of Queens Problems

This project explores several "Queens Problems" involving the placement of queens on an `m x n` chessboard:

1. **Minimum Queens to Cover the Board**  
   Determine the minimum number of queens required to place on an `m x n` chessboard such that every cell on the board is attacked by a queen.

2. **Maximum Non-Attacking Queens**  
   Find the maximum number of queens that can be placed on an `m x n` chessboard such that no two queens threaten each other.

#### Configuration

In `config.txt`, specify the size of the chessboard and the size of squares and symbols:

```
ROWS
COLS
SQUARE_SIZE
QUEEN_SIZE
ATTACKED_SYMBOL_SIZE
```

#### Running the simulation

Run the `exe` file. Left click to place/remove queens. Press `esc` to exit, `z` to undo, and `r` to reset the board.

## N Queens Problem Demo

Run the `NQueensDEMO.exe` file. Left click to show next move. Press `spacebar` to show next case. Press `f` to show next solution. Press `esc` to exit and `r` to reset.

`COLS` will be forced to set equal to `ROWS` for the N Queens problem. Otherwise, no solution will be found (obviously).