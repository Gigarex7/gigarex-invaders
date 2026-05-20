// SPDX-License-Identifier: GPL-2.0-only
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void drawScreen(Player player){
    system("cls"); // janky refresh
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            if((y==0 && x==0) || (y==HEIGHT-1 && x==0) || (y==0 && x==WIDTH-1) || (y==HEIGHT-1 && x==WIDTH-1)){
                printf("+");
            }else if(y==HEIGHT-1 || y==0){
                printf("-");
            }else if(x==WIDTH-1 || x==0){
                printf("|");
            }else if(x==player.x && y==player.y){
                printf("^");
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
}
