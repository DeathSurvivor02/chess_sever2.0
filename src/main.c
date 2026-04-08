#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

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

/* ── Global State (needed for Emscripten loop) ───────────────── */
static Board     board;
static GameState state   = STATE_MENU;
static GameMode  mode    = MODE_BLITZ;
static GameMode  hovered = MODE_BLITZ;

static int sel_row = -1, sel_col = -1;
static int last_fr = -1, last_fc = -1;
static int last_tr = -1, last_tc = -1;

/* ── Menu drawing helper ─────────────────────────────────────── */
static void draw_menu(GameMode hovered) {
    ClearBackground(COL_BG);
    const char *title = "CHESS SERVER 2.0";
    int         tsz   = 52;
    int         tx    = SCREEN_W / 2 - MeasureText(title, tsz) / 2;
    DrawText(title, tx + 2, 72 + 2, tsz, DARKGRAY);   
    DrawText(title, tx,     72,     tsz, WHITE);

    const char *sub = "Player  vs  Computer";
    DrawText(sub, SCREEN_W / 2 - MeasureText(sub, 20) / 2, 136, 20, GRAY);

    int card_w = 260, card_h = 90;
    int card_x = SCREEN_W / 2 - card_w / 2;

    for (int i = 0; i < MODE_COUNT; i++) {
        int card_y = 210 + i * (card_h + 18);
        Color bg = (i == (int)hovered) ? COL_ACCENT : CLITERAL(Color){ 42, 42, 55, 255 };
        DrawRectangleRounded((Rectangle){card_x, card_y, card_w, card_h}, 0.18f, 8, bg);

        char badge[4];
        snprintf(badge, sizeof(badge), "[%d]", i + 1);
        DrawText(badge, card_x + 14, card_y + card_h / 2 - 10, 18, LIGHTGRAY);

        char name_buf[32];
        snprintf(name_buf, sizeof(name_buf), "%s  ·  %s", MODE_NAMES[i], MODE_LABELS[i]);
        DrawText(name_buf, card_x + card_w / 2 - MeasureText(name_buf, 22) / 2, card_y + card_h / 2 - 11, 22, WHITE);
    }

    DrawText("Press 1, 2, or 3 — or click to select",
             SCREEN_W / 2 - MeasureText("Press 1, 2, or 3 — or click to select", 15) / 2,
             SCREEN_H - 40, 15, DARKGRAY);
}

/* ── Game Loop Component ───────────────────────────────────────── */
void UpdateDrawFrame(void) {
    int card_w = 260, card_h = 90;
    int card_x = SCREEN_W / 2 - card_w / 2;

    /* ══ UPDATE ═════════════════════════════════════════════ */
    if (state == STATE_MENU) {
        if (IsKeyPressed(KEY_ONE))   { mode = MODE_BULLET; state = STATE_PLAYING; board_reset(&board); }
        if (IsKeyPressed(KEY_TWO))   { mode = MODE_BLITZ;  state = STATE_PLAYING; board_reset(&board); }
        if (IsKeyPressed(KEY_THREE)) { mode = MODE_RAPID;  state = STATE_PLAYING; board_reset(&board); }

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
    } else if (state == STATE_PLAYING) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            state   = STATE_MENU;
            sel_row = sel_col = -1;
        }

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
    } else if (state == STATE_PLAYING) {
        ClearBackground(COL_BG);
        render_board(&board, sel_row, sel_col, last_fr, last_fc, last_tr, last_tc);
        render_pieces(&board);
        char mode_label[32];
        snprintf(mode_label, sizeof(mode_label), "%s  (%s)", MODE_NAMES[mode], MODE_LABELS[mode]);
        render_panel(mode_label, board.turn);
    }
    EndDrawing();
}

/* ── Main ────────────────────────────────────────────────────── */
int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(SCREEN_W, SCREEN_H, TITLE);
    
    board_init(&board);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}
