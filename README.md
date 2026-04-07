# Chess Server 2.0

A timed **Player vs Computer** chess game written in C using [Raylib](https://www.raylib.com/) for 2D rendering.

## Game Modes

| Mode | Time Per Side | Description |
|------|--------------|-------------|
| **Bullet** | 2 min | Lightning-fast play |
| **Blitz** | 5 min | Fast, tactical games |
| **Rapid** | 15 min | Thoughtful, strategic play |

The computer opponent uses a **predefined chess bot** — an opening book of well-known sequences, strategic pattern matching, and endgame plans — rather than a search-tree AI.

## Building

### Requirements
- GCC (MinGW-w64 recommended on Windows)
- [Raylib 5.5 MinGW build](https://github.com/raysan5/raylib/releases/tag/5.5)

### Setup
```bash
# Download Raylib 5.5 for MinGW and extract to:
#   chess_sever2.0/raylib/raylib-5.5_win64_mingw-w64/

make        # build chess.exe
make run    # build and launch
make clean  # remove build artifacts
```

## Project Structure

```
src/
  main.c      — game loop, state machine, menu
  board.c/h   — 8×8 board state and piece definitions
  render.c/h  — Raylib drawing (board, pieces, panel)
  move.c/h    — legal move generation (Phase 2)
  clock.c/h   — chess countdown timer (Phase 4)
  bot.c/h     — predefined chess bot (Phase 5)
```

## Development Roadmap

- [x] Phase 1 — Board rendering, mode menu
- [ ] Phase 2 — Move input, legal moves, piece movement
- [ ] Phase 3 — Check, checkmate, stalemate detection
- [ ] Phase 4 — Chess clock (Bullet / Blitz / Rapid)
- [ ] Phase 5 — Predefined bot (opening book + patterns)
- [ ] Phase 6 — Polish (captured pieces, move history)
- [ ] Phase 7 — v1.0 release

## License

Mozilla Public License 2.0 — see [LICENSE](LICENSE).
