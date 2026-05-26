// SPDX-License-Identifier: GPL-2.0-only
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int initializeLevel(const char *filename, Player *player, Enemy ***enemies, LevelConfig *level){
    player->x=WIDTH/2; // middle
    player->y=HEIGHT-2; // bottom
    FILE *file=fopen(filename, "r");
    if(file==NULL){
        return 0; // signal that it errored out
    }
    fscanf(file, "%d %d", &level->enemyRows, &level->enemyCols);
    fscanf(file, "%d %d", &level->formationY, &level->formationX);
    fscanf(file, "%d", &level->enemyDirection);
    fscanf(file, "%d", &level->enemySpeed);
    *enemies=malloc(level->enemyRows*sizeof(Enemy*)); // note to self: think of this as [pointer] [pointer] [pointer]
    for(int i=0; i<level->enemyRows; i++){
        (*enemies)[i]=malloc(level->enemyCols*sizeof(Enemy)); // note to self: think of this as [struct] [struct] [struct] [struct]
    }
    for(int y=0; y<level->enemyRows; y++){
        for(int x=0; x<level->enemyCols; x++){
            fscanf(file, " %c %d", &level->enemySymbol, &level->enemyPoints);
            if(level->enemySymbol=='W'){
                level->enemyType=ENEMY_WEAK;
            }else if(level->enemySymbol=='N'){
                level->enemyType=ENEMY_NORMAL;
            }else if(level->enemySymbol=='S'){
                level->enemyType=ENEMY_STRONG;
            }else{
                level->enemyType=ENEMY_NONE;
            }
            (*enemies)[y][x].type=level->enemyType;
            (*enemies)[y][x].points=level->enemyPoints;
            (*enemies)[y][x].active=(level->enemyType!=ENEMY_NONE); // yes [1], while not nonexistent
            (*enemies)[y][x].exploding=0;
            (*enemies)[y][x].explosionTimer=0;
        }
    }
    fclose(file);
    return 1; // signal that it went through
}

void moveEnemies(Enemy **enemies, LevelConfig *level, MovementTimers *timers){
    int hitWall=0;
    for(int y=0; y<level->enemyRows; y++){
        for(int x=0; x<level->enemyCols; x++){
            if(enemies[y][x].exploding){
                timers->enemyFormation=0; // enemy elimination pause
            }
            if(enemies[y][x].active){
                int enemyWallX=((level->formationX) + (x*4)); // (x*4)=spacing
                // <= and >= instead of == are defense against bugs, call order errors, and timing errors
                if((level->enemyDirection==1) && ((enemyWallX) >= (WIDTH-2))){ // (WIDTH-2) is the right margin
                    hitWall=1;
                }
                // <= and >= instead of == are defense against bugs, call order errors, and timing errors
                if((level->enemyDirection==-1) && (enemyWallX<=1)){ // (enemyWallX<=1) is the left margin
                    hitWall=1;
                }
            }
        }
    }
    if(hitWall){
        timers->enemyFormation=0; // wall bump pause
        level->formationY+=1;
        level->enemyDirection*=-1; // multiplying 1 by -1 is perfect for switching between them
    }else{
        level->formationX+=level->enemyDirection;
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

void verifyCollisions(PlayerProjectile **shots, Enemy **enemies, LevelConfig level, int *score){
    PlayerProjectile *current=*shots;
    PlayerProjectile *previous=NULL;
    int screenY=0, screenX=0;
    while(current!=NULL){
        int projectileDeleted=0; // nested loops are the enemy of progress I read
        for(int y=0; y<level.enemyRows; y++){
            for(int x=0; x<level.enemyCols; x++){
                if(enemies[y][x].active){
                    screenY=(y+(level.formationY)); // (y+a): vertical margin
                    screenX=((x*4) + (level.formationX)); // (x*a)+b: spacing + horizontal margin
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

void drawScreen(Player player, PlayerProjectile *shots, Enemy **enemies, LevelConfig *level, int score){
    char screen[HEIGHT][WIDTH];
    char bufferFrame[(HEIGHT*(WIDTH+1))+30]; // each row needs HEIGHT + 1 newline, + 30 is padding just in case
    int enemyY=0, enemyX=0, bufferPos=0;
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
    for(int y=0; y<level->enemyRows; y++){
        for(int x=0; x<level->enemyCols; x++){
            if(enemies[y][x].exploding){ // here I don't deactivate the enemy because that's not what this function's role is
                enemyY=(y+(level->formationY)); // (y+a): vertical margin
                enemyX=((x*4) + (level->formationX)); // (x*a)+b: spacing + horizontal margin
                screen[enemyY][enemyX]='*';
            }else if(enemies[y][x].active){
                enemyY=(y+(level->formationY)); // (y+a): vertical margin
                enemyX=((x*4) + (level->formationX)); // (x*a)+b: spacing + horizontal margin
                if(enemies[y][x].type==ENEMY_WEAK){
                    level->enemyChar='V';
                }else if(enemies[y][x].type==ENEMY_NORMAL){
                    level->enemyChar='M';
                }else{
                    level->enemyChar='W';
                }
                screen[enemyY][enemyX]=level->enemyChar;
            }
        }
    }
    /* WORK IN PROGRESS: Enemies drawn over spaces | END */
    // Copy each character into one continous string, add stuff, print
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            bufferFrame[bufferPos]=screen[y][x];
            bufferPos++;
        }
        bufferFrame[bufferPos]='\n';
        bufferPos++;
    }
    bufferPos+=sprintf(&bufferFrame[bufferPos], "SCORE: %d\n", score); // sprintf is specifically for buffers
    bufferFrame[bufferPos]='\0'; // '\0' determines the end of the string in C
    printf("%s", bufferFrame);
}

// This function ensures EXPLOSIONS render correctly instead of lingering or disappearing too early
void updateExplosions(Enemy **enemies, LevelConfig level){
    for(int y=0; y<level.enemyRows; y++){
        for(int x=0; x<level.enemyCols; x++){
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

int formationEliminated(Enemy **enemies, LevelConfig level){
    for(int y=0; y<level.enemyRows; y++){
        for(int x=0; x<level.enemyCols; x++){
            if(enemies[y][x].active){
                return 0; // enemies still active
            }
        }
    }
    return 1; // all enemies eliminated
}

void drawScreenEnd(GameState gameState, int score){
    printf("\033[H"); // ANSI-CSI: move cursor to top left corner
    if(gameState==WON){
        printf("\n");
        printf("    YOU WIN!\n");
    }
    else if(gameState==LOST){
        printf("\n");
        printf("    GAME OVER\n");
    }
    printf("\n");
    printf("FINAL SCORE: %d\n", score);
}
