#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game.h"


typedef struct game {
    int board[BOARD_WIDTH][BOARD_HEIGHT];
    int currentPlayer;
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

    puts("=");

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