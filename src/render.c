/* ─────────────────────────────────────────────────────────────
 *  Chess Server 2.0 · render.c
 *  All Raylib drawing routines: board squares, pieces, panel.
 * ───────────────────────────────────────────────────────────── */

#include "render.h"
#include <stdio.h>

/* ── Piece-symbol tables ─────────────────────────────────────── */
static const char *WHITE_SYM[] = { " ", "P", "N", "B", "R", "Q", "K" };
static const char *BLACK_SYM[] = { " ", "p", "n", "b", "r", "q", "k" };

const char *piece_symbol(int piece_type, int color) {
    if (piece_type < 0 || piece_type > KING) return " ";
    return (color == PIECE_WHITE) ? WHITE_SYM[piece_type] : BLACK_SYM[piece_type];
}

/* ── render_board ────────────────────────────────────────────── */
void render_board(const Board *b,
                  int sel_r,  int sel_c,
                  int last_fr, int last_fc,
                  int last_tr, int last_tc)
{
    (void)b; /* reserved for future per-square state */

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int x = BOARD_OFFSET_X + col * CELL_SIZE;
            int y = BOARD_OFFSET_Y + row * CELL_SIZE;

            /* Base colour */
            Color sq = ((row + col) % 2 == 0) ? COL_LIGHT_SQ : COL_DARK_SQ;
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, sq);

            /* Last-move highlight (subtle yellow) */
            if ((row == last_fr && col == last_fc) ||
                (row == last_tr && col == last_tc)) {
                DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, COL_LAST_MOVE);
            }

            /* Selected-square highlight (green) */
            if (row == sel_r && col == sel_c) {
                DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, COL_SELECT);
            }
        }
    }

    /* ── Rank labels (8 → 1) on the left ── */
    for (int row = 0; row < BOARD_SIZE; row++) {
        char label[3];
        snprintf(label, sizeof(label), "%d", 8 - row);
        int lx = BOARD_OFFSET_X - 20;
        int ly = BOARD_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2 - 8;
        DrawText(label, lx, ly, 16, LIGHTGRAY);
    }

    /* ── File labels (a → h) below the board ── */
    for (int col = 0; col < BOARD_SIZE; col++) {
        char label[2] = { 'a' + col, '\0' };
        int lx = BOARD_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2 - 5;
        int ly = BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 6;
        DrawText(label, lx, ly, 16, LIGHTGRAY);
    }

    /* ── Board border ── */
    DrawRectangleLinesEx(
        (Rectangle){ BOARD_OFFSET_X, BOARD_OFFSET_Y,
                     BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE },
        2, DARKGRAY);
}

/* ── render_pieces ───────────────────────────────────────────── */
void render_pieces(const Board *b) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int raw = b->pieces[row][col];
            if (raw == 0) continue;

            int piece_type = (raw > 0) ? raw : -raw;
            int color      = (raw > 0) ? PIECE_WHITE : PIECE_BLACK;

            const char *sym = piece_symbol(piece_type, color);

            /* Centre the glyph in its cell */
            int font_size = 44;
            int tx = BOARD_OFFSET_X + col * CELL_SIZE
                     + (CELL_SIZE - MeasureText(sym, font_size)) / 2;
            int ty = BOARD_OFFSET_Y + row * CELL_SIZE
                     + (CELL_SIZE - font_size) / 2;

            Color shadow = (color == PIECE_WHITE)
                           ? CLITERAL(Color){80,80,80,200}
                           : CLITERAL(Color){200,200,200,200};
            Color fill   = (color == PIECE_WHITE)
                           ? CLITERAL(Color){255,255,255,255}
                           : CLITERAL(Color){20,20,20,255};

            DrawText(sym, tx + 2, ty + 2, font_size, shadow);
            DrawText(sym, tx,     ty,     font_size, fill);
        }
    }
}

/* ── render_panel ────────────────────────────────────────────── */
void render_panel(const char *mode_name, int turn) {
    int px = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 20;
    int pw = 180;

    /* Panel background */
    DrawRectangleRounded(
        (Rectangle){ px - 8, BOARD_OFFSET_Y - 8, pw, BOARD_SIZE * CELL_SIZE + 16 },
        0.06f, 6, COL_PANEL);

    /* Mode title */
    DrawText("MODE", px, BOARD_OFFSET_Y + 10, 13, GRAY);
    DrawText(mode_name, px, BOARD_OFFSET_Y + 26, 17, COL_ACCENT);

    /* Divider */
    DrawLine(px, BOARD_OFFSET_Y + 56, px + pw - 16, BOARD_OFFSET_Y + 56, DARKGRAY);

    /* Turn indicator */
    DrawText("TURN", px, BOARD_OFFSET_Y + 68, 13, GRAY);
    const char *turn_str  = (turn == PIECE_WHITE) ? "White" : "Black";
    Color        turn_col = (turn == PIECE_WHITE)
                            ? CLITERAL(Color){255,255,255,255}
                            : CLITERAL(Color){180,180,180,255};
    DrawText(turn_str, px, BOARD_OFFSET_Y + 84, 22, turn_col);

    /* Colour swatch */
    Color swatch = (turn == PIECE_WHITE)
                   ? CLITERAL(Color){240,240,240,255}
                   : CLITERAL(Color){50,50,60,255};
    DrawRectangleRounded(
        (Rectangle){ px, BOARD_OFFSET_Y + 112, 28, 28 }, 0.3f, 6, swatch);
    DrawRectangleRoundedLinesEx(
        (Rectangle){ px, BOARD_OFFSET_Y + 112, 28, 28 }, 0.3f, 6, 2, GRAY);

    /* Controls hint */
    DrawText("[ESC] Menu", px, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE - 30, 13, DARKGRAY);
}
