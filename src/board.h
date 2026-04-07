#ifndef BOARD_H
#define BOARD_H

/* ─────────────────────────────────────────────────────────────
 *  Chess Server 2.0 · board.h
 *  Core board representation and game state.
 * ───────────────────────────────────────────────────────────── */

#define BOARD_SIZE 8

/* Piece type constants (always positive; sign encodes color) */
#define EMPTY   0
#define PAWN    1
#define KNIGHT  2
#define BISHOP  3
#define ROOK    4
#define QUEEN   5
#define KING    6

/* Color tokens (prefixed to avoid clash with Raylib's Color macros) */
#define PIECE_WHITE   1
#define PIECE_BLACK  -1

/* Castling-rights indices into Board.castling[] */
#define CASTLE_WK 0   /* white kingside  */
#define CASTLE_WQ 1   /* white queenside */
#define CASTLE_BK 2   /* black kingside  */
#define CASTLE_BQ 3   /* black queenside */

/* ── Main game-state struct ── */
typedef struct {
    /*
     * pieces[row][col]
     *   row 0 = rank 8 (black's back rank)
     *   row 7 = rank 1 (white's back rank)
     *
     *  Positive value → white piece
     *  Negative value → black piece
     *  0              → empty square
     *
     *  e.g. pieces[0][4] == -KING   (black king on e8 at start)
     *       pieces[7][4] ==  KING   (white king on e1 at start)
     */
    int pieces[BOARD_SIZE][BOARD_SIZE];

    int turn;           /* WHITE or BLACK — whose move it is   */
    int castling[4];    /* 1 if right still available, else 0  */
    int ep_row;         /* en-passant target square (-1 = none)*/
    int ep_col;
    int halfmove;       /* half-move clock for 50-move rule     */
    int fullmove;       /* full-move counter                    */
} Board;

/* ── API ── */
void board_init(Board *b);           /* set up starting position */
void board_reset(Board *b);          /* alias — same as init     */
int  board_piece_at(const Board *b, int row, int col);  /* type only  */
int  board_color_at(const Board *b, int row, int col);  /* WHITE/BLACK/0 */

#endif /* BOARD_H */
