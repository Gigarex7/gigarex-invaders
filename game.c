// SPDX-License-Identifier: GPL-2.0-only
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void drawScreen(Player player){
    char screen[HEIGHT][WIDTH];
    printf("\033[J"); // ANSI-CSI: move cursor to top left corner
    printf("\033[H"); // ANSI-CSI: clear screen
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            if((y==0 && x==0) || (y==HEIGHT-1 && x==0) || (y==0 && x==WIDTH-1) || (y==HEIGHT-1 && x==WIDTH-1)){
                screen[y][x]='+';
            }else if(y==HEIGHT-1 || y==0){
                screen[y][x]='-';
            }else if(x==WIDTH-1 || x==0){
                screen[y][x]='|';
            }else if(x==player.x && y==player.y){
                screen[y][x]='^';
            }else{
                screen[y][x]=' ';
            }
            printf("%c", screen[y][x]);
        }
        printf("\n");
    }
}
