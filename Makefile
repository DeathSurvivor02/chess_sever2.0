# ──────────────────────────────────────────────────────────────
#  Chess Server 2.0 · Makefile
#  Targets: all (default), clean, run
# ──────────────────────────────────────────────────────────────

CC      = gcc
OUT     = chess.exe

# Raylib pre-built for MinGW-w64
RAYLIB  = raylib/raylib-5.5_win64_mingw-w64

CFLAGS  = -Wall -Wextra -O2 \
          -I$(RAYLIB)/include \
          -Isrc

LDFLAGS = -L$(RAYLIB)/lib \
          -lraylib \
          -lopengl32 \
          -lgdi32 \
          -lwinmm \
          -mwindows

SRC     = src/main.c \
          src/board.c \
          src/render.c

# ── Targets ───────────────────────────────────────────────────
.PHONY: all clean run

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT) $(LDFLAGS)

run: all
	./$(OUT)

clean:
	@if exist $(OUT) del /Q $(OUT)
	@echo Cleaned.
