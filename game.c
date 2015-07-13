/* Connect 4
 *
 * Terminal 2-player Connect Four game.
 *
 * Written as example for an Intro to C course; teaches arrays, looping,
 * and basic CPP macros.
 *
 * Raises warnings if not compiled with a C89 or newer compiler.
 * Tested with GCC 5.1.
 *
 * Copyright (C) 2015 Joel Burton <joel@joelburton.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game.h"


typedef struct game {
    int board[BOARD_WIDTH][BOARD_HEIGHT];  // 0=empty, 1=X 2=O
    int currentPlayer;                     // 1=X, 2=O
} Game;

Game game;


/* Initialize board to empty and set start player to #1 */

void startGame()
{
    for (int y = 0; y < BOARD_HEIGHT; y++)
        for (int x = 0; x < BOARD_WIDTH; x++)
            game.board[x][y] = 0;

    game.currentPlayer = 1;
}


/* Draw board. */

void showBoard()
{
    putchar('\n');

    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        putchar('|');
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!game.board[x][y]) putchar(' ');
            else putchar(X_OR_O(game.board[x][y]));
            putchar('|');
        }
        putchar('\n');
    }

    // draw bottom border
    for (int x = 0; x < BOARD_WIDTH; x++)
        printf("==");

    puts("=");   // adds \n

    // draw column numbers
    putchar(' ');

    for (int x = 0; x < BOARD_WIDTH; x++)
        printf("%d ", x + 1);

    putchar('\n');

}


/* Return true/false if move is legal. */

bool validateMove(int col)
{
    if (col < 1 || col > BOARD_WIDTH) return 0;

    return game.board[col - 1][BOARD_HEIGHT - 1] == 0;
}


/* Prompt for move and return integer column of move. */

int getMove()
{
    char move[255];
    int col = 0;

    while (!validateMove(col)) {
        printf("\nMove %c (Control-D to quit): ", X_OR_O(game.currentPlayer));
        if (fgets(move, 254, stdin) == NULL) exit(0);
        col = atoi(move);
    }
    return col;
}


/* Place move as far down as possible in column. */

void makeMove(int col)
{
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        if (game.board[col - 1][y] == 0) {
            game.board[col - 1][y] = game.currentPlayer;
            break;
        }
    }
}


/* Check board for winner and return winner
 *
 * If no winner, return 0.
 */

int findWinner()
{
    // To find a winner, it's not necessary to scan from every cell --
    // we only have to scan from the bottom left (x=0, y=0) up to (x=4, y=4) --
    // since we'll look at that cell and three up/right to find horiz,
    // vertical, or diagonal "/" matches (we look at 5*5=25 cells, not 8*8=64,
    // plus we get to skip bounds checks if we tried to go up/right off the
    // edge).

    for (int y = 0; y < BOARD_HEIGHT - 3; y++) {
        for (int x = 0; x < BOARD_WIDTH - 3; x++) {
            if (!game.board[x][y]) continue;

            // horiz
            if (game.board[x][y] == game.board[x + 1][y] &&
                game.board[x][y] == game.board[x + 2][y] &&
                game.board[x][y] == game.board[x + 3][y]) {
                return game.board[x][y];
            }

            // vert
            if (game.board[x][y] == game.board[x][y + 1] &&
                game.board[x][y] == game.board[x][y + 2] &&
                game.board[x][y] == game.board[x][y + 3]) {
                return game.board[x][y];
            }

            // "/"
            if (game.board[x][y] == game.board[x + 1][y + 1] &&
                game.board[x][y] == game.board[x + 2][y + 2] &&
                game.board[x][y] == game.board[x + 3][y + 3]) {
                return game.board[x][y];
            }
        }

        // For diagonal "\" matches, we scan from (x=3, y=0) up to (x=7, y=4);
        // that will let us check all possible wins positioning left and up
        // from that cell (again, we look at 5*5=25, not 8*8=64, plus we get
        // to skip bounds checks)

        for (int x = 3; x < BOARD_WIDTH; x++) {
            if (!game.board[x][y]) continue;

            // "\"
            if (game.board[x][y] == game.board[x - 1][y + 1] &&
                game.board[x][y] == game.board[x - 2][y + 2] &&
                game.board[x][y] == game.board[x - 3][y + 3]) {
                return game.board[x][y];
            }
        }
    }

    return 0;   // no winner
}


int main(int argc, char *argv[])
{
    int winner = 0;
    int move;

    startGame();
    showBoard();

    while (!winner) {
        move = getMove();
        makeMove(move);
        showBoard();
        winner = findWinner();
        game.currentPlayer = game.currentPlayer == 2 ? 1 : 2;
    }

    printf("\nWinner = %c\n", X_OR_O(winner));
    return 0;
}