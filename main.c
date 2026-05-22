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
    player.x=WIDTH/2; // middle
    player.y=HEIGHT-2; // bottom
    PlayerProjectile *shots=NULL;
    int score=0;
    /* TEMPORARY: Hardcoded Enemies | START */
    Enemy **enemies;
    int enemyRows=3;
    int enemyCols=8;
    enemies=malloc(enemyRows*sizeof(Enemy*)); // [pointer] [pointer] [pointer]
    for(int i=0; i<enemyRows; i++){
        enemies[i]=malloc(enemyCols*sizeof(Enemy)); // [struct] [struct] [struct] [struct]
    }
    initializeLevel(enemies, enemyRows, enemyCols);
    /* TEMPORARY: Hardcoded Enemies | END */
    // Game Loop
    int running=1;
    system("cls");
    while(running){ // note that "==1" is superfluous here because running already "=1"
        moveProjectiles(&shots);
        verifyCollisions(&shots, enemies, enemyRows, enemyCols, &score); // enemies doesn't get an & because I'm not modifying it
        drawScreen(player, shots, enemies, enemyRows, enemyCols, score);
        char input; // note that making the program not require an Enter press after each input is advanced
        scanf(" %c", &input); // note that the space in " %c" ignores the newline, and there's a lot of newline
        // Left
        if((input=='a' || input=='A') && (player.x > 1)){
            player.x--;
        }
        // Right
        if((input=='d' || input=='D') && (player.x < (WIDTH-2))){
            player.x++;
        }
        // Shoot
        if((input=='s' || input=='S')){
            shootPlayer(&shots, player);
        }
        // Quit
        if(input=='q' || input=='Q'){
            running=0;
        }
        updateExplosions(enemies, enemyRows, enemyCols);
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
