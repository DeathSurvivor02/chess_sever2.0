/* ═══════════════════════════════════════════════════════════════
 *  Chess Server 2.0 · main.c
 *  Entry point — window management, game loop, state machine.
 *
 *  Phase 1: Board rendering + mode selection menu.
 *           (Move generation, clock, and bot come in later phases.)
 * ═══════════════════════════════════════════════════════════════ */

#include "raylib.h"
#include "board.h"
#include "render.h"

#include <stdio.h>
#include <string.h>

/* ── Window ──────────────────────────────────────────────────── */
#define SCREEN_W  880
#define SCREEN_H  720
#define TITLE     "Chess Server 2.0"

/* ── Game modes ──────────────────────────────────────────────── */
typedef enum { MODE_BULLET = 0, MODE_BLITZ, MODE_RAPID, MODE_COUNT } GameMode;
typedef enum { STATE_MENU = 0, STATE_PLAYING, STATE_GAMEOVER }        GameState;

static const char *MODE_NAMES[]   = { "Bullet",  "Blitz",   "Rapid"   };
static const char *MODE_LABELS[]  = { "2 min",   "5 min",   "15 min"  };
/* MODE_SECS (seconds per side): used in Phase 4 when the clock is added.
 * static const int MODE_SECS[] = { 120, 300, 900 }; */

/* ── Menu drawing helper ─────────────────────────────────────── */
static void draw_menu(GameMode hovered) {
    /* Background */
    ClearBackground(COL_BG);

    /* Title */
    const char *title = "CHESS SERVER 2.0";
    int         tsz   = 52;
    int         tx    = SCREEN_W / 2 - MeasureText(title, tsz) / 2;
    DrawText(title, tx + 2, 72 + 2, tsz, DARKGRAY);   /* shadow */
    DrawText(title, tx,     72,     tsz, CLITERAL(Color){255,255,255,255});

    const char *sub = "Player  vs  Computer";
    DrawText(sub,
             SCREEN_W / 2 - MeasureText(sub, 20) / 2,
             136, 20, GRAY);

    /* Mode cards */
    int card_w = 260, card_h = 90;
    int card_x = SCREEN_W / 2 - card_w / 2;

    for (int i = 0; i < MODE_COUNT; i++) {
        int card_y = 210 + i * (card_h + 18);

        Color bg = (i == (int)hovered)
                   ? COL_ACCENT
                   : CLITERAL(Color){ 42, 42, 55, 255 };

        DrawRectangleRounded((Rectangle){card_x, card_y, card_w, card_h},
                             0.18f, 8, bg);

        /* Key hint badge */
        char badge[4];
        snprintf(badge, sizeof(badge), "[%d]", i + 1);
        DrawText(badge, card_x + 14, card_y + card_h / 2 - 10, 18, LIGHTGRAY);

        /* Mode name */
        char name_buf[32];
        snprintf(name_buf, sizeof(name_buf), "%s  ·  %s",
                 MODE_NAMES[i], MODE_LABELS[i]);
        DrawText(name_buf,
                 card_x + card_w / 2 - MeasureText(name_buf, 22) / 2,
                 card_y + card_h / 2 - 11,
                 22, CLITERAL(Color){255,255,255,255});
    }

    /* Footer */
    DrawText("Press 1, 2, or 3 — or click to select",
             SCREEN_W / 2 - MeasureText("Press 1, 2, or 3 — or click to select", 15) / 2,
             SCREEN_H - 40, 15, DARKGRAY);
}

/* ── Main ────────────────────────────────────────────────────── */
int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_W, SCREEN_H, TITLE);
    SetTargetFPS(60);

    Board     board;
    board_init(&board);

    GameState state   = STATE_MENU;
    GameMode  mode    = MODE_BLITZ;   /* last selected mode   */
    GameMode  hovered = MODE_BLITZ;

    /* Square selection (Phase 2 will use these for move input) */
    int sel_row = -1, sel_col = -1;

    /* Last move squares (highlighted in yellow — filled in Phase 2) */
    int last_fr = -1, last_fc = -1;
    int last_tr = -1, last_tc = -1;

    /* ── Card click boundaries (recomputed per frame is fine at 60 fps) */
    int card_w = 260, card_h = 90;
    int card_x = SCREEN_W / 2 - card_w / 2;

    while (!WindowShouldClose()) {

        /* ══ UPDATE ═════════════════════════════════════════════ */
        if (state == STATE_MENU) {
            /* Keyboard shortcuts */
            if (IsKeyPressed(KEY_ONE))   { mode = MODE_BULLET; state = STATE_PLAYING; board_reset(&board); }
            if (IsKeyPressed(KEY_TWO))   { mode = MODE_BLITZ;  state = STATE_PLAYING; board_reset(&board); }
            if (IsKeyPressed(KEY_THREE)) { mode = MODE_RAPID;  state = STATE_PLAYING; board_reset(&board); }

            /* Mouse hover / click on cards */
            Vector2 mp = GetMousePosition();
            for (int i = 0; i < MODE_COUNT; i++) {
                int cy = 210 + i * (card_h + 18);
                Rectangle card = { card_x, cy, card_w, card_h };
                if (CheckCollisionPointRec(mp, card)) {
                    hovered = (GameMode)i;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        mode  = (GameMode)i;
                        state = STATE_PLAYING;
                        board_reset(&board);
                        sel_row = sel_col = -1;
                        last_fr = last_fc = last_tr = last_tc = -1;
                    }
                }
            }
        }

        if (state == STATE_PLAYING) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                state   = STATE_MENU;
                sel_row = sel_col = -1;
            }

            /* Square click — Phase 2 will wire up actual move logic here */
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mp  = GetMousePosition();
                int clicked_col = (int)((mp.x - BOARD_OFFSET_X) / CELL_SIZE);
                int clicked_row = (int)((mp.y - BOARD_OFFSET_Y) / CELL_SIZE);
                if (clicked_col >= 0 && clicked_col < BOARD_SIZE &&
                    clicked_row >= 0 && clicked_row < BOARD_SIZE) {
                    sel_row = clicked_row;
                    sel_col = clicked_col;
                }
            }
        }

        /* ══ DRAW ════════════════════════════════════════════════ */
        BeginDrawing();

        if (state == STATE_MENU) {
            draw_menu(hovered);
        }

        if (state == STATE_PLAYING) {
            ClearBackground(COL_BG);

            /* Chessboard + pieces */
            render_board(&board, sel_row, sel_col,
                         last_fr, last_fc, last_tr, last_tc);
            render_pieces(&board);

            /* Side panel */
            char mode_label[32];
            snprintf(mode_label, sizeof(mode_label),
                     "%s  (%s)", MODE_NAMES[mode], MODE_LABELS[mode]);
            render_panel(mode_label, board.turn);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
