// SPDX-License-Identifier: GPL-2.0-only
#ifndef GAME_H
#define GAME_H

// Screen Parameters

#define WIDTH 40
#define HEIGHT 20

// Structures

typedef enum{
    START, PLAYING, WON, LOST
}GameState;

typedef struct{
    // Configurable
    int enemyRows; // default 3
    int enemyCols; // default 8
    int formationY; // default 2
    int formationX; // default 5
    int enemyDirection;  // default 1, 1=right, -1=left
    int enemySpeed; // default 10
    // Internal Setup
    char enemySymbol;
    char enemyChar;
    int enemyType;
    int enemyPoints;
}LevelConfig;

typedef struct{
    char name[4];
    int highScore;
}HighScore;

typedef struct{
    int x;
    int y;
}Player;

typedef struct PlayerProjectile{
    int x;
    int y;
    struct PlayerProjectile *next;
}PlayerProjectile;

typedef enum{
    ENEMY_NONE, ENEMY_WEAK, ENEMY_NORMAL, ENEMY_STRONG
}EnemyType;

typedef struct{
    EnemyType type;
    int points;
    int active;
    int exploding;
    int explosionTimer;
}Enemy;

typedef struct{
    int enemyFormation;
    int shotFrameskip;
}MovementTimers;

// Prototypes

int initializeLevel(Player *player, Enemy ***enemies, LevelConfig *level, const char *filename);
void moveEnemies(Enemy **enemies, MovementTimers *timers, LevelConfig *level);
void shootPlayer(Player player, PlayerProjectile **shots);
void moveProjectiles(PlayerProjectile **shots);
void verifyCollisions(PlayerProjectile **shots, Enemy **enemies, LevelConfig level, int *score);
void drawScreen(Player player, PlayerProjectile *shots, Enemy **enemies, LevelConfig *level, int score);
void inputHandling(Player *player, PlayerProjectile **shots, GameState *gameState, int *running, char input);
void updateExplosions(Enemy **enemies, LevelConfig level);
int formationEliminated(Enemy **enemies, LevelConfig level);
void scoreHandling(int score);
void drawScreenStartEnd(GameState gameState, int score);

#endif