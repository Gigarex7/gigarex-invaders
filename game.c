// SPDX-License-Identifier: GPL-2.0-only
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void initializeLevel(Player *player, Enemy **enemies, int enemyRows, int enemyCols){
    player->x=WIDTH/2; // middle
    player->y=HEIGHT-2; // bottom
    /* TEMPORARY: Hardcoded Enemies | START */
    for(int y=0; y<enemyRows; y++){
        for(int x=0; x<enemyCols; x++){
            enemies[y][x].type=ENEMY_NORMAL;
            enemies[y][x].points=100;
            enemies[y][x].active=1;
            enemies[y][x].exploding=0;
            enemies[y][x].explosionTimer=0;
        }
    }
    /* TEMPORARY: Hardcoded Enemies | END */
}

void moveEnemies(Enemy **enemies, int enemyRows, int enemyCols, int *formationX, int *formationY, int *enemyDirection){
    int hitWall=0;
    for(int y=0; y<enemyRows; y++){
        for(int x=0; x<enemyCols; x++){
            if(enemies[y][x].active){
                int enemyWallX=(*formationX+(x*4)); // (x*4)=spacing
                // <= and >= instead of == are defense against bugs, call order errors, and timing errors
                if((*enemyDirection==1) && ((enemyWallX) >= (WIDTH-2))){ // (WIDTH-2) is the right margin
                    hitWall=1;
                }
                // <= and >= instead of == are defense against bugs, call order errors, and timing errors
                if((*enemyDirection==-1) && (enemyWallX<=1)){ // (enemyX<=1) is the left margin
                    hitWall=1;
                }
            }
        }
    }
    if(hitWall){
        *formationY+=1;
        *enemyDirection*=-1; // multiplying 1 by -1 is perfect for switching between them
    }else{
        *formationX+=*enemyDirection;
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
            PlayerProjectile *temp=current;
            current=(current->next);
            free(temp);
        }else{
            previous=current;
            current=(current->next);
        }
    }
}

void verifyCollisions(PlayerProjectile **shots, Enemy **enemies, int enemyRows, int enemyCols, int formationY, int formationX, int *score){
    PlayerProjectile *current=*shots;
    PlayerProjectile *previous=NULL;
    int screenY=0, screenX=0;
    while(current!=NULL){
        int projectileDeleted=0; // nested loops are the enemy of progress I read
        for(int y=0; y<enemyRows; y++){
            for(int x=0; x<enemyCols; x++){
                if(enemies[y][x].active){
                    screenY=(y+formationY); // (y+a): vertical margin (formationY=2)
                    screenX=((x*4)+formationX); // (x*a)+b: spacing + horizontal margin (formationX=5)
                    if(((current->y)==(screenY)) && ((current->x)==(screenX))){
                        enemies[y][x].exploding=1;
                        enemies[y][x].explosionTimer=1;
                        enemies[y][x].active=0;
                        *score+=enemies[y][x].points;
                        projectileDeleted=1;
                        break;
                    }
                }
            }
            if(projectileDeleted){
                break;
            }
        }
        if(projectileDeleted){
            if(previous==NULL){
                *shots=(current->next);
            }else{
                (previous->next) = (current->next);
            }
            PlayerProjectile *temp=current;
            current=(current->next);
            free(temp);
        }else{
            previous=current;
            current=(current->next);
        }
    }
}

void drawScreen(Player player, PlayerProjectile *shots, Enemy **enemies, int enemyRows, int enemyCols, int formationY, int formationX, int score){
    char screen[HEIGHT][WIDTH];
    int enemyY=0, enemyX=0;
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
            if(enemies[y][x].exploding){ // here I don't deactivate the enemy because that's not what this function's role is
                enemyY=(y+formationY); // (y+a): vertical margin (formationY=2)
                enemyX=((x*4)+formationX); // (x*a)+b: spacing + horizontal margin (formationX=5)
                screen[enemyY][enemyX]='*';
            }else if(enemies[y][x].active){
                enemyY=(y+formationY); // (y+a): vertical margin (formationY=2)
                enemyX=((x*4)+formationX); // (x*a)+b: spacing + horizontal margin (formationX=5)
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
    printf("SCORE: %d\n", score);
}

// This function ensures EXPLOSIONS render correctly instead of lingering or disappearing too early
void updateExplosions(Enemy **enemies, int enemyRows, int enemyCols){
    for(int y=0; y<enemyRows; y++){
        for(int x=0; x<enemyCols; x++){
            if(enemies[y][x].exploding){
                enemies[y][x].explosionTimer--;
                if(enemies[y][x].explosionTimer<=0){ // <= and >= instead of == are defense against bugs, call order errors, and timing errors
                    enemies[y][x].exploding=0;
                }
            }
        }
    }
}

void inputHandling(char input, Player *player, PlayerProjectile **shots, int *running){
    // Left
    if((input=='a' || input=='A') && ((player->x) > 1)){
        player->x--;
    }
    // Right
    if((input=='d' || input=='D') && ((player->x) < (WIDTH-2))){
        player->x++;
    }
    // Shoot
    if((input=='s' || input=='S')){
        shootPlayer(shots, *player);
    }
    // Quit
    if(input=='q' || input=='Q'){
        *running=0;
    }
}
