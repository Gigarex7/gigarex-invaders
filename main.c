// SPDX-License-Identifier: GPL-2.0-only
/*
Gigarex Invaders
Copyright (C) 2026 Gigarex7
*/

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // really old Windows-specific library that enables real-time input and rendering here
#include <synchapi.h> // allows the game speed to lock itself to a "framerate" through Sleep();
#include "game.h"

int main(){
    // Main Variables
    Player player;
    PlayerProjectile *shots=NULL;
    MovementTimers timers={0};
    LevelConfig level;
    Enemy **enemies;
    int score=0;
    // Load Level
    if(!initializeLevel(&player, &enemies, &level, "level.txt")){
        printf("ERROR: File 'level.txt' is inaccessible.\n");
        return 1; // force stop due to error
    }
    int running=0;
    GameState gameState=START;
    while(gameState==START){
        drawScreenStart(&gameState, &running, score);
    }
    // Game Loop
    while(running){ // note that "==1" is superfluous here because running already "=1"
        timers.enemyFormation++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.enemyFormation)>=level.enemySpeed){ // this serves to slow down the game to a playable level
            moveEnemies(enemies, &timers, &level);
            timers.enemyFormation=0;
        }
        timers.shotFrameskip++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.shotFrameskip)>=2){ // this serves to fix ghosting
            moveProjectiles(&shots);
            timers.shotFrameskip=0;
        }
        verifyCollisions(&shots, enemies, level, &score);
        if(formationEliminated(enemies, level)){
            gameState=WON;
            running=0;
        }
        drawScreen(player, shots, enemies, &level, score);
        if(_kbhit()){ // if a key is presssed, process input, else continue
            char input=_getch();
            inputHandling(&player, &shots, &gameState, &running, input);
        }
        updateExplosions(enemies, level);
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if(((level.formationY) + (level.enemyRows)) >= player.y){
            gameState=LOST;
            running=0;
        }
        Sleep(30); // loop pause in milliseconds
    }
    // End Game & Cleanup
    drawScreenEnd(gameState, score);
    for(int i=0; i<level.enemyRows; i++){
        free(enemies[i]);
    }
    free(enemies);
    return 0;
}
