// SPDX-License-Identifier: GPL-2.0-only
/*
Space Invaders (terminal version)
Copyright (C) 2026 Gigarex7
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // really old Windows-specific library that enables real-time input and rendering here
#include <synchapi.h> // allows the game speed to lock itself to a "framerate" through Sleep();
#include "game.h"

int main(){
    Player player;
    PlayerProjectile *shots=NULL;
    MovementTimers timers={0}; // I initialized all timers in 0
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
    GameState gameState=PLAYING;
    system("cls");
    while(running){ // note that "==1" is superfluous here because running already "=1"
        timers.enemyFormation++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.enemyFormation)>=10){ // this serves to slow down the game to a playable level
            moveEnemies(enemies, enemyRows, enemyCols, &formationX, &formationY, &enemyDirection, &timers);
            timers.enemyFormation=0;
        }
        timers.shotFrameskip++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.shotFrameskip)>=2){ // this serves to fix ghosting
            moveProjectiles(&shots);
            timers.shotFrameskip=0;
        }
        verifyCollisions(&shots, enemies, enemyRows, enemyCols, formationY, formationX, &score); // enemies doesn't get an & because I'm not modifying it
        if(formationEliminated(enemies, enemyRows, enemyCols)){
            gameState=WON;
            running=0;
        }
        drawScreen(player, shots, enemies, enemyRows, enemyCols, formationY, formationX, score);
        if(_kbhit()){ // if a key is presssed, process input, else continue
            char input=_getch(); 
            inputHandling(input, &player, &shots, &running);
        }
        updateExplosions(enemies, enemyRows, enemyCols);
        if((formationY+enemyRows) >= player.y){ // <= and >= instead of == are defense against bugs, call order errors, and timing errors
            gameState=LOST;
            running=0;
        }
        Sleep(30); // loop pause in milliseconds
    }
    // End
    for(int i=0; i<enemyRows; i++){
        free(enemies[i]);
    }
    free(enemies);
    system("cls");
    drawScreenEnd(gameState, score);
    printf("EXITING...");
    return 0;
}
