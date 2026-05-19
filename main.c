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
    drawScreen(player);
    return 0;
}
