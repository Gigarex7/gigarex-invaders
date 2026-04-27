// SPDX-License-Identifier: GPL-2.0-only
/*
Space Invaders (terminal version)
Copyright (C) 2026 Gigarex7
*/

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    ENEMY_NONE, ENEMY_V, ENEMY_M, ENEMY_W
}EnemyType;

typedef struct{
    EnemyType type;
    int points;
    int active;
}Enemy;

typedef struct{
    int x, y;
}Player;

typedef struct Projectile{
    int x, y;
    struct Projectile* next;
}Projectile;

typedef struct{
    char name[4];
    int score;
}Record;

// prototypes go here

int main(){
    // loop goes here
    return 0;
}

// functions go here
