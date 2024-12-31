#include "game.h"
#include "config.h"
#include "gui.h"
#include "level.h"
#include "raylib.h"
#include "texmanager.h"
#include "utils.h"
#include <stdio.h>

/// Initializes all the game state.
/// Loads the worldfile, generates the level texture
/// and initializes player and enemies.
/// Returns 1 on success and 0 on failure
int Game_init(Game *game) {
  *game = (Game){0};
  game->state = GAME_TITLE_SCREEN;
  TextureManager_load(&game->tex);
  EnemyManager_init(&game->enemy_mgr, &game->tex);
  if (!Level_init(&game->level, &game->enemy_mgr, WORLDFILE_PATH)) {
    return 0;
  }
  Player_init(&game->player, &game->tex);
  EnemyManager_spawn(&game->enemy_mgr);
  game->player.pos = game->level.spawn_point;
  game->camera.target.y = 0;
  // make camera picture the whole background height.
  game->camera.zoom = (float)WINDOW_WIDTH / (game->level.sprite.height * 2);
  // offset to center in player
  game->camera.offset.x = WINDOW_WIDTH / 2.0;
  // cancel out the level drawing offset
  game->camera.offset.y = game->level.sprite_offset.y * game->camera.zoom;

  printf("Spawn point: x: %f, y: %f", game->level.spawn_point.x,
         game->level.spawn_point.y);
  if (!Leaderboard_init(&game->leaderboard, LEADERBOARD_FILE)) {
    return 0;
  }
  return 1;
}

static void Game_reset(Game *game) {
  EnemyManager_spawn(&game->enemy_mgr);
  Player_init(&game->player, &game->tex);
  game->player.pos = game->level.spawn_point;
}

/// Draws the Game title screen
static void Game_menu_draw(Game *game) {
  Texture logo = game->tex.logo;
  int half_width = WINDOW_WIDTH / 2;
  int title_y = WINDOW_HEIGHT / 3 - logo.height / 2;
  int next_y = title_y + logo.height / 2;
  int step = GUI_FONT_SIZE + GUI_PADDING;

  char *ctrls = "MOVE:\n" MOVE_LEFT_KEY_STR ", " MOVE_RIGHT_KEY_STR
                "\nJUMP: " JUMP_KEY_STR "\nSHOOT: " SHOOT_KEY_STR;

  ClearBackground(BLUE);
  DrawTexture(logo, half_width - logo.width / 2, title_y, WHITE);
  gui_text(ctrls, 50, WINDOW_HEIGHT / 6, 1.0, ALIGN_LEFT);
  if (gui_button("Start", half_width, next_y + step * 2)) {
    game->state = GAME_RUNNING;
  }
  if (gui_button("Leaderboard", half_width, next_y + step * 3)) {
    game->state = GAME_LEADERBOARD;
  }
  if (gui_button("Exit", half_width, next_y + step * 4)) {
    game->state = GAME_CLOSE;
  }
}

/// Draws the leaderboard screen.
static void Game_leaderboard_draw(Game *game) {
  char entry[PLAYER_NAME_MAX + 5] = {0};
  int half_width = WINDOW_WIDTH / 2;
  int next_y = WINDOW_HEIGHT / 8;

  ClearBackground(BLUE);
  gui_text("LEADERBOARD", half_width, next_y, 1.5, ALIGN_CENTER);
  next_y += GUI_FONT_SIZE * 3;
  for (int i = 0; i < LEADERBOARD_NUM; ++i) {
    PlayerRecord *p = &game->leaderboard.players[i];
    /// Write information to a string
    snprintf(entry, PLAYER_NAME_MAX + 5, "%s", p->name);
    gui_text(entry, half_width, next_y, 1.0, ALIGN_RIGHT);
    snprintf(entry, PLAYER_NAME_MAX + 5, " - %d", p->score);
    gui_text(entry, half_width, next_y, 1.0, ALIGN_LEFT);
    next_y += GUI_FONT_SIZE + GUI_PADDING;
  }
  next_y += GUI_FONT_SIZE;
  if (gui_button("Back", half_width, next_y)) {
    game->state = GAME_TITLE_SCREEN;
  }
}

/// Game logic function. Runs every frame.
static void Game_running_update(Game *game) {
  float delta = GetFrameTime();

  Player_update(game, delta);
  EnemyManager_update(game, delta);
  Bullets_update(game, delta);

  game->camera.target.x = game->player.pos.x;
  if (Player_is_dead(&game->player)) {
    game->state = GAME_DIED;
    return;
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->state = GAME_PAUSED;
    return;
  }
}

/// Draw everything in the game. Runs every frame.
static void Game_running_draw(Game *game) {
  ClearBackground(WHITE);

  BeginMode2D(game->camera);
  Level_draw(&game->level);
  Player_draw(&game->player);
  EnemyManager_draw(&game->enemy_mgr);
  Bullets_draw(game);
  EndMode2D();
  gui_stats(&game->player, 10, 50);
}

static void Game_paused_draw(Game *game) {
  Game_running_draw(game);
  DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ColorAlpha(BLACK, 0.5));
  int next_y = WINDOW_HEIGHT / 6;
  gui_text("PAUSED", WINDOW_WIDTH / 2, next_y, 1.5, ALIGN_CENTER);
  next_y += GUI_FONT_SIZE * 2;
  gui_text("Score:", WINDOW_WIDTH / 2, next_y, 1, ALIGN_RIGHT);
  gui_number(game->player.record.score, WINDOW_WIDTH / 2 + 10, next_y, 1,
             ALIGN_LEFT);
  next_y += GUI_FONT_SIZE * 2;

  if (gui_button("Resume", WINDOW_WIDTH / 3, next_y)) {
    game->state = GAME_RUNNING;
  }
  if (gui_button("Quit", 2 * WINDOW_WIDTH / 3, next_y)) {
    game->state = GAME_TITLE_SCREEN;
    Game_reset(game);
  }
}
static void Game_died_draw(Game *game) {
  Game_running_draw(game);
  DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ColorAlpha(RED, 0.5));
  DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, ColorAlpha(BLACK, 0.2));
  int next_y = WINDOW_HEIGHT / 6;
  gui_text("YOU DIED", WINDOW_WIDTH / 2, next_y, 1.5, ALIGN_CENTER);
  next_y += GUI_FONT_SIZE * 2;
  gui_text("Score:", WINDOW_WIDTH / 2, next_y, 1, ALIGN_RIGHT);
  gui_number(game->player.record.score, WINDOW_WIDTH / 2 + 10, next_y, 1,
             ALIGN_LEFT);
  next_y += GUI_FONT_SIZE * 2;

  if (gui_button("Retry", WINDOW_WIDTH / 3, next_y)) {
    game->state = GAME_RUNNING;
    Game_reset(game);
  }
  if (gui_button("Quit", 2 * WINDOW_WIDTH / 3, next_y)) {
    game->state = GAME_TITLE_SCREEN;
    Game_reset(game);
  }
}

static void Game_paused_update(Game *game) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->state = GAME_RUNNING;
  }
}

/// Draws the game depending on its game state.
void Game_draw(Game *game) {
  BeginDrawing();
  switch (game->state) {
  case GAME_RUNNING:
    Game_running_draw(game);
    break;
  case GAME_DIED:
    Game_died_draw(game);
    break;
  case GAME_PAUSED:
    Game_paused_draw(game);
    break;
  case GAME_TITLE_SCREEN:
    Game_menu_draw(game);
    break;
  case GAME_LEADERBOARD:
    Game_leaderboard_draw(game);
    break;
  default:
    error_out("not implemented");
  }
  EndDrawing();
}

/// Updates the Game structure.
/// Returns 1 if the game should keep running
/// and 0 if the game should close.
int Game_update(Game *game) {
  switch (game->state) {
  case GAME_RUNNING:
    Game_running_update(game);
    break;
  case GAME_PAUSED:
    Game_paused_update(game);
    break;
  case GAME_CLOSE:
    return 0;
  default:
    break;
  }
  return 1;
}