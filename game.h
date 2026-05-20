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

void shootPlayer(PlayerProjectile **shots, Player player);
void moveProjectiles(PlayerProjectile **shots);
void drawScreen(Player player, PlayerProjectile *shots);

#endif