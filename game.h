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

void drawScreen(Player player);

#endif