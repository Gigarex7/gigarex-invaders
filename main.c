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
    // bottom middle initial position
    player.x=WIDTH/2;
    player.y=HEIGHT-2;
    // Game Loop
    int running=1;
    system("cls");
    while(running){ // whatever will I do with all the time I saved by not writing it "==1"
        drawScreen(player);
        char input; // note that making the program not require an Enter press after each input is advanced
        scanf(" %c", &input); // the space in " %c" ignores the newline, there's a lot of newline
        // Left
        if((input=='a' || input=='A') && (player.x > 1)){
            player.x--;
        }
        // Right
        if((input=='d' || input=='D') && (player.x < (WIDTH-2))){
            player.x++;
        }
        // Quit
        if(input=='q' || input=='Q'){
            running=0;
        }
    }
    // End (WIP)
    system("cls");
    printf("EXITING...");
    return 0;
}

/* I have this idea where "EXITING..." remains at the bottom of the
screen while all other elements are wiped line by line before the
program shuts down. Might become part of the cutting room floor. */
