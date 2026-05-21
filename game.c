// SPDX-License-Identifier: GPL-2.0-only
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void initializeLevel(Enemy **enemies, int enemyRows, int enemyCols){
    for(int y=0; y<enemyRows; y++){
        for(int x=0; x<enemyCols; x++){
            enemies[y][x].type=ENEMY_NORMAL;
            enemies[y][x].points=100;
            enemies[y][x].active=1;
        }
    }
}

void shootPlayer(PlayerProjectile **shots, Player player){
    PlayerProjectile *newProjectile=malloc(sizeof(PlayerProjectile));
    newProjectile->x=player.x;
    newProjectile->y=(player.y-1);
    newProjectile->next=*shots;
    *shots=newProjectile;
}

void moveProjectiles(PlayerProjectile **shots){
    PlayerProjectile *current=*shots;
    PlayerProjectile *previous=NULL;
    while(current!=NULL){
        current->y--;
        if((current->y) <= (0)){
            if(previous==NULL){
                *shots=(current->next);
            }else{
                (previous->next) = (current->next);
            }
            PlayerProjectile* temp=current;
            current=(current->next);
            free(temp);
        }else{
            previous=current;
            current=(current->next);
        }
    }
}

void drawScreen(Player player, PlayerProjectile *shots, Enemy** enemies, int enemyRows, int enemyCols){
    char screen[HEIGHT][WIDTH];
    printf("\033[J"); // ANSI-CSI: clear screen
    printf("\033[H"); // ANSI-CSI: move cursor to top left corner
    // Initial screen filled with spaces
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            screen[y][x]=' ';
        }
    }
    // Borders drawn over spaces
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            if((y==0 && x==0) || (y==HEIGHT-1 && x==0) || (y==0 && x==WIDTH-1) || (y==HEIGHT-1 && x==WIDTH-1)){
                screen[y][x]='+';
            }else if(y==0 || y==HEIGHT-1){
                screen[y][x]='-';
            }else if(x==0 || x==WIDTH-1){
                screen[y][x]='|';
            }
        }
    }
    // Player drawn over spaces
    screen[player.y][player.x]='^';
    // Projectiles drawn over spaces
    PlayerProjectile *current=shots;
    while(current!=NULL){
        screen[current->y][current->x]='.';
        current=(current->next);
    }
    /* WORK IN PROGRESS: Enemies drawn over spaces | START */
    for(int y=0; y<enemyRows; y++){
        for(int x=0; x<enemyCols; x++){
            if(enemies[y][x].active){
                int enemyY=(y+2); // (y+a): vertical margin
                int enemyX=((x*4)+5); // (x*a)+b: spacing + horizontal margin
                screen[enemyY][enemyX]='M';
            }
        }
    }
    /* WORK IN PROGRESS: Enemies drawn over spaces | END */
    // Print complete frame
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}
