#pragma once

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define GAME_SCALE ((float)WINDOW_WIDTH / (LEVEL_HEIGHT * TILE_SIZE * 2))

#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 10
#define TILE_SIZE 16

#define GRAVITY (30 * PLAYER_SIZE)

#define INITIAL_HEALTH 3
#define PLAYER_SIZE 24
#define PLAYER_SPEED (PLAYER_SIZE * 5.5)
#define PLAYER_NAME_MAX 10
#define PLAYER_NAME_BUF (PLAYER_NAME_MAX + 1)
#define PLAYER_JUMP_FORCE 11
#define PLAYER_JUMP_DIVIDER 2.0
#define PLAYER_INVINCIBILITY 1.0
#define PLAYER_SHOOTING_TIME 0.5
#define PLAYER_DAMAGE_RECOIL 5
#define PLAYER_LIFE_PTS 200

#define MAX_BULLETS 5
#define BULLET_WIDTH 6
#define BULLET_HEIGHT 3
#define BULLET_SPEED (PLAYER_SPEED * 2)
#define BULLET_FIRE_DELAY 0.3
#define BULLET_ALIVE_TIME 1.5

#define MAX_ENEMIES 10
#define ENEMY_SIZE 16
#define ENEMY_SPEED (PLAYER_SPEED * 1.2)
#define ENEMY_ATTACK_SPEED (ENEMY_SPEED * 1.5)
#define ENEMY_PTS 100
#define ENEMY_UPDATE_DIST (14 * TILE_SIZE)
#define ENEMY_ATTACK_DIST (3 * TILE_SIZE)
#define ENEMY_ATTACK_DELAY 1

#define JUMP_KEY KEY_X
#define MOVE_LEFT_KEY KEY_LEFT
#define MOVE_RIGHT_KEY KEY_RIGHT
#define SHOOT_KEY KEY_Z
#define MOVE_LEFT_KEY_STR "<-"
#define MOVE_RIGHT_KEY_STR "->"
#define JUMP_KEY_STR "x"
#define SHOOT_KEY_STR "z"

#define WORLDFILE_PATH "level.world"
#define LEADERBOARD_FILE "top_scores.bin"

#define LEADERBOARD_NUM 5

#define GUI_FONT_SIZE 40
#define GUI_PADDING 10

#define DEBUG_MODE 0

#define ASSETS "assets/"

#define SPRITE_PLAYER ASSETS "player-atlas.png"
#define SPRITE_BLOCK ASSETS "tile1.png"
#define SPRITE_OBSTACLE ASSETS "spike.png"
#define SPRITE_BACKGROUND ASSETS "background.png"
#define SPRITE_ENEMY ASSETS "enemy-atlas.png"
#define SPRITE_LOGO ASSETS "inf_man.png"