// SPDX-License-Identifier: GPL-2.0-only
/*
Space Invaders (terminal version)
Copyright (C) 2026 Gigarex7
*/

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(){
    Player player;
    PlayerProjectile *shots=NULL;
    int score=0;
    /* TEMPORARY (?): Formation Position | START */
    int formationY=2;
    int formationX=5;
    int enemyDirection=1; // 1=right, -1=left
    /* TEMPORARY (?): Formation Position | END*/
    /* TEMPORARY: Hardcoded Enemies | START */
    Enemy **enemies;
    int enemyRows=3;
    int enemyCols=8;
    enemies=malloc(enemyRows*sizeof(Enemy*)); // note to self: think of this as [pointer] [pointer] [pointer]
    for(int i=0; i<enemyRows; i++){
        enemies[i]=malloc(enemyCols*sizeof(Enemy)); // note to self: think of this as [struct] [struct] [struct] [struct]
    }
    initializeLevel(&player, enemies, enemyRows, enemyCols);
    /* TEMPORARY: Hardcoded Enemies | END */
    // Game Loop
    int running=1;
    system("cls");
    while(running){ // note that "==1" is superfluous here because running already "=1"
        moveEnemies(enemies, enemyRows, enemyCols, &formationX, &formationY, &enemyDirection);
        moveProjectiles(&shots);
        verifyCollisions(&shots, enemies, enemyRows, enemyCols, formationY, formationX, &score); // enemies doesn't get an & because I'm not modifying it
        drawScreen(player, shots, enemies, enemyRows, enemyCols, formationY, formationX, score);
        char input; // note that making the program not require an Enter press after each input is advanced
        scanf(" %c", &input); // note that the space in " %c" ignores the newline, and there's a lot of newline
        inputHandling(input, &player, &shots, &running);
        updateExplosions(enemies, enemyRows, enemyCols);
        if((formationY+enemyRows) >= player.y){ // <= and >= instead of == are defense against bugs, call order errors, and timing errors
            running=0;
        }
    }
    // End
    for(int i=0; i<enemyRows; i++){
        free(enemies[i]);
    }
    free(enemies);
    system("cls");
    printf("EXITING...");
    return 0;
}
