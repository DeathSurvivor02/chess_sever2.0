#ifndef RENDER_H
#define RENDER_H

/* ─────────────────────────────────────────────────────────────
 *  Chess Server 2.0 · render.h
 *  Raylib drawing helpers for the board and pieces.
 * ───────────────────────────────────────────────────────────── */

#include "raylib.h"
#include "board.h"

/* ── Layout constants ─────────────────────────────────────────
 * Board occupies an 8×8 grid of CELL_SIZE px squares.
 * Left/top offsets leave room for rank/file labels.          */
#define CELL_SIZE      75
#define BOARD_OFFSET_X 60
#define BOARD_OFFSET_Y 40

/* ── Colour palette ──────────────────────────────────────────── */
#define COL_LIGHT_SQ   CLITERAL(Color){ 240, 217, 181, 255 }  /* cream        */
#define COL_DARK_SQ    CLITERAL(Color){ 181, 136,  99, 255 }  /* brown        */
#define COL_SELECT     CLITERAL(Color){ 100, 200, 100, 170 }  /* green tint   */
#define COL_LAST_MOVE  CLITERAL(Color){ 205, 210,  60, 120 }  /* yellow tint  */
#define COL_BG         CLITERAL(Color){  22,  22,  30, 255 }  /* near-black   */
#define COL_PANEL      CLITERAL(Color){  35,  35,  45, 255 }  /* side panel   */
#define COL_ACCENT     CLITERAL(Color){  80, 160, 220, 255 }  /* blue accent  */

/* ── API ─────────────────────────────────────────────────────── */

/* Draw the 8×8 coloured squares, rank/file labels.
 * selected_row/col: highlighted square (-1 to skip).          */
void render_board(const Board *b, int selected_row, int selected_col,
                  int last_from_row, int last_from_col,
                  int last_to_row,   int last_to_col);

/* Draw each piece as a styled letter glyph on its square. */
void render_pieces(const Board *b);

/* Draw the right-side info panel (mode label, turn indicator). */
void render_panel(const char *mode_name, int turn);

/* Utility: returns the letter symbol for a piece+colour combo.
 * White = uppercase, Black = lowercase  (P N B R Q K / p n b r q k) */
const char *piece_symbol(int piece_type, int color);

#endif /* RENDER_H */
