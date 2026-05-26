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
    LevelConfig level;
    Enemy **enemies;
    int score=0;
    if(!initializeLevel("level.txt", &player, &enemies, &level)){
        printf("ERROR: File 'level.txt' is inaccessible.\n");
        return 1; // force stop due to error
    }
    // Game Loop
    int running=1;
    GameState gameState=PLAYING;
    system("cls");
    while(running){ // note that "==1" is superfluous here because running already "=1"
        timers.enemyFormation++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.enemyFormation)>=level.enemySpeed){ // this serves to slow down the game to a playable level
            moveEnemies(enemies, &level, &timers);
            timers.enemyFormation=0;
        }
        timers.shotFrameskip++;
        // <= and >= instead of == are defense against bugs, call order errors, and timing errors
        if((timers.shotFrameskip)>=2){ // this serves to fix ghosting
            moveProjectiles(&shots);
            timers.shotFrameskip=0;
        }
        verifyCollisions(&shots, enemies, level, &score); // enemies doesn't get an & because I'm not modifying it
        if(formationEliminated(enemies, level)){
            gameState=WON;
            running=0;
        }
        drawScreen(player, shots, enemies, &level, score);
        if(_kbhit()){ // if a key is presssed, process input, else continue
            char input=_getch(); 
            inputHandling(input, &player, &shots, &running);
        }
        updateExplosions(enemies, level);
        if(((level.formationY) + (level.enemyRows)) >= player.y){ // <= and >= instead of == are defense against bugs, call order errors, and timing errors
            gameState=LOST;
            running=0;
        }
        Sleep(30); // loop pause in milliseconds
    }
    // End
    for(int i=0; i<level.enemyRows; i++){
        free(enemies[i]);
    }
    free(enemies);
    system("cls");
    drawScreenEnd(gameState, score);
    printf("EXITING...");
    return 0;
}
