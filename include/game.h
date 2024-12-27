#pragma once
#include "leaderboard.h"
#include "level.h"
#include "player.h"
#include "raylib.h"

typedef enum GameState {
  GAME_TITLE_SCREEN,
  GAME_LEADERBOARD,
  GAME_PAUSED,
  GAME_RUNNING,
  GAME_DIED,
  GAME_CLOSE,
} GameState;

typedef struct Game {
  GameState state;
  Level level;
  Player player;
  Camera2D camera;
  Texture title_sprite;
  Leaderboard leaderboard;
} Game;

/// Initializes a Game struct.
/// Returns 1 on success and 0 on failure.
int Game_init(Game *game);

int Game_update(Game *game);
void Game_draw(Game *game);

void Game_collide(Game *game);