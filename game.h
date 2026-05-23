// SPDX-License-Identifier: GPL-2.0-only
#ifndef GAME_H
#define GAME_H

// Screen
#define WIDTH 40
#define HEIGHT 20

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

void initializeLevel(Player *player, Enemy **enemies, int enemyRows, int enemyCols);
void moveEnemies(Enemy **enemies, int enemyRows, int enemyCols, int *formationX, int *formationY, int *enemyDirection);
void shootPlayer(PlayerProjectile **shots, Player player);
void moveProjectiles(PlayerProjectile **shots);
void verifyCollisions(PlayerProjectile **shots, Enemy **enemies, int enemyRows, int enemyCols, int formationY, int formationX, int *score);
void drawScreen(Player player, PlayerProjectile *shots, Enemy **enemies, int enemyRows, int enemyCols, int formationY, int formationX, int score);
void inputHandling(char input, Player *player, PlayerProjectile **shots, int *running);
void updateExplosions(Enemy **enemies, int enemyRows, int enemyCols);

#endif