/* ─────────────────────────────────────────────────────────────
 *  Chess Server 2.0 · board.c
 *  Board initialisation and basic query helpers.
 * ───────────────────────────────────────────────────────────── */

#include "board.h"
#include <string.h>

/* Standard starting position.
 * Row 0 = rank 8 (black side), Row 7 = rank 1 (white side). */
static const int START_POS[BOARD_SIZE][BOARD_SIZE] = {
    { -ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK },
    { -PAWN,  -PAWN,   -PAWN,  -PAWN, -PAWN,  -PAWN,   -PAWN,  -PAWN },
    {     0,      0,       0,      0,      0,      0,       0,      0  },
    {     0,      0,       0,      0,      0,      0,       0,      0  },
    {     0,      0,       0,      0,      0,      0,       0,      0  },
    {     0,      0,       0,      0,      0,      0,       0,      0  },
    {  PAWN,   PAWN,    PAWN,   PAWN,   PAWN,   PAWN,    PAWN,   PAWN },
    {  ROOK,  KNIGHT,  BISHOP, QUEEN,   KING,  BISHOP,  KNIGHT,  ROOK },
};

/* ── board_init / board_reset ────────────────────────────────── */
void board_init(Board *b)  { board_reset(b); }

void board_reset(Board *b) {
    memcpy(b->pieces, START_POS, sizeof(START_POS));

    b->turn        = PIECE_WHITE;
    b->castling[CASTLE_WK] = 1;
    b->castling[CASTLE_WQ] = 1;
    b->castling[CASTLE_BK] = 1;
    b->castling[CASTLE_BQ] = 1;

    b->ep_row  = -1;
    b->ep_col  = -1;
    b->halfmove = 0;
    b->fullmove = 1;
}

/* ── Query helpers ───────────────────────────────────────────── */

/* Returns the piece TYPE (PAWN..KING) on (row,col), or EMPTY. */
int board_piece_at(const Board *b, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return EMPTY;
    int p = b->pieces[row][col];
    return (p < 0) ? -p : p;
}

/* Returns PIECE_WHITE, PIECE_BLACK, or 0 for an empty square. */

int board_color_at(const Board *b, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return 0;
    int p = b->pieces[row][col];
    if (p == 0) return 0;
    return (p > 0) ? PIECE_WHITE : PIECE_BLACK;
}
