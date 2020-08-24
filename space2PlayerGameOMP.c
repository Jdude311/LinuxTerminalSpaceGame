//This file, when compiled with GCC, must use the "-lncurses" and the "-fopenmp" options to compile correctly. Example: gcc -fopenmp -lncurses space2PlayerGame.c -o space2PlayerGame.out
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include <omp.h>

//Define objects' sprites? so they can be used later
#define asteroidChar "#"
#define blockChar "▓"
#define projectile "•"

int winner = 0;

void sleepSeconds(double seconds, double milliseconds) {
  struct timespec ts;
  ts.tv_sec = seconds;
  ts.tv_nsec = milliseconds * 1000000;
  nanosleep(&ts, NULL);
}

void gameOver(int winningPlayer) {
  printf("Player %d Wins!!!", winningPlayer);
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  clear();
  mvprintw(0, 0, "  ________                        ________                     \n /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\\n\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\n \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n        \\/     \\/      \\/     \\/          \\/          \\/       \n");
  refresh();
  sleepSeconds (2, 500);
  clear();
  for (int i = 0; i < 20; i++) {
    if (winningPlayer == 1 && i % 2 != 1)
      {
	clear();
	mvprintw(10, 0, "\n######    #      #     #  ##### #    #   # \n#    #   ##      #     #    #   ##   #  ###\n#    #  # #      #     #    #   # #  #  ###\n######    #      #     #    #   #  # #   # \n#         #      #  #  #    #   #   ##     \n#         #      # # # #    #   #    #   # \n#         #      ##   ##    #   #    #  ###\n#       #####    #     #  ##### #    #   # \n");
	refresh();
	sleepSeconds(0, 800);
      }
    else if (i % 2 == 1)
      {
	clear();
	refresh();
	sleepSeconds(0, 800);
      }
    else if (winningPlayer == 2 && i % 2 != 1)
      {
	clear();
	mvprintw(10, 0, "\n######   ###     #     #  ##### #    #   # \n#    #  #   #    #     #    #   ##   #  ###\n#    #      #    #     #    #   # #  #  ###\n######      #    #     #    #   #  # #   # \n#          #     #  #  #    #   #   ##     \n#         #      # # # #    #   #    #   # \n#        #       ##   ##    #   #    #  ###\n#       #####    #     #  ##### #    #   # \n");
	refresh();
	sleepSeconds(0, 800);
      }
  }
  refresh();
  sleepSeconds(3, 0);
}

int main() {
  unsigned char kbhit();
  setlocale(LC_ALL, "");
  struct block {
    int health;
    int x;
    int y;
  };
  struct block blocks[160];
  struct asteroid {
    int hsp;
    int vsp;
    int x;
    int y;
  };
  struct asteroid asteroids[6];
  struct bullet {
    int hsp;
    int vsp;
    int x;
    int y;
    int health;
  };
  struct bullet p1bullets[1000];
  struct bullet p2bullets[1000];
  struct player {
    int hsp;
    int vsp;
    int x;
    int y;
    int health;
    int canShoot;
  };
  struct player player1;
  player1.hsp = 0;
  player1.vsp = 0;
  player1.x = 3;
  player1.y = 20;
  player1.health = 2;
  player1.canShoot = 2;

  struct player player2;
  player2.hsp = 0;
  player2.vsp = 0;
  player2.x = 74;
  player2.y = 20;
  player2.health = 2;
  player2.canShoot = 2;

  for (int i = 0; i < 1000; i++) {
    p1bullets[i].x = -1;
    p1bullets[i].y = -1;
    p2bullets[i].x = -1;
    p2bullets[i].y = -1;
  }
  for (int i = 0; i < 160; i++) {
    blocks[i].health = 1;
    if (i < 40) {
      blocks[i].x = 1;
      blocks[i].y = i + 1;
    } else if (i < 80) {
      blocks[i].x = 2;
      blocks[i].y = i - 39;
    } else if (i < 120) {
      blocks[i].x = 79;
      blocks[i].y = i - 79;
    } else if (i < 160) {
      blocks[i].x = 80;
      blocks[i].y = i - 119;
    }
  }
  int totalFramesCounter = 1;
  int p1bulletNumber = 0;
  int p2bulletNumber = 0;
  while (1) {
    sleepSeconds(0, 200);
    for (int i = 0; i < 8; i++) {
      switch(getch()) {
      case 119:
	player1.vsp = -1;
	break;
      case 115:
	player1.vsp = 1;
	break;
      case 259:
	player2.vsp = -1;
	break;
      case 258:
	player2.vsp = 1;
	break;
      case 100:
	if (player1.canShoot >= 4) {
	  p1bullets[p1bulletNumber].x = player1.x + 4;
	  p1bullets[p1bulletNumber].y = player1.y;
	  p1bullets[p1bulletNumber].hsp = 1;
	  p1bullets[p1bulletNumber].health = 1;
	  p1bulletNumber += 1;
	  player1.canShoot = 0;
	}
      break;
      case 260:
	if (player2.canShoot >= 4) {
	  p2bullets[p2bulletNumber].x = player2.x;
	  p2bullets[p2bulletNumber].y = player2.y;
	  p2bullets[p2bulletNumber].hsp = -1;
	  p2bullets[p2bulletNumber].health = 1;
	  p2bulletNumber += 1;
	  player2.canShoot = 0;
	}
	break;
      default:
	break;
      }
    }
    
    if ((player1.y == 1 && player1.vsp == -1) || (player1.y == 40 && player1.vsp == 1)) {
      player1.vsp = 0;
    }
    if ((player2.y == 1 && player2.vsp == -1) || (player2.y == 40 && player2.vsp == 1)) {
      player2.vsp = 0;
    }
    
    player1.y += player1.vsp;
    player2.y += player2.vsp;
    for (int i = 0; i < p1bulletNumber + 1; i++) {
      p1bullets[i].x += p1bullets[i].hsp;
    }
    for (int i = 0; i < p2bulletNumber + 1; i++) {
      p2bullets[i].x += p2bullets[i].hsp;
    }
    
    for (int i = p1bulletNumber-24; i < p1bulletNumber; i++)
      {
	if (p1bullets[i].x == 79 && p1bullets[i].health == 1)
	  {
	    winner = 1;
	  }
#pragma omp parallel
	{
	  #pragma omp for
	  for (int a = p2bulletNumber-24; a < p2bulletNumber; a++)
	    {
	      if (p2bullets[a].x == 2 && p2bullets[a].health == 1)
		{
		  winner = 2;
		}
	      if (/*Check health of both bullets*/(p1bullets[i].health != 0 && p2bullets[a].health != 0) && /*Check if the y value is the same*/(p1bullets[i].y == p2bullets[a].y) && /*Check collision if the bullets' healths are nonzero*/(/*Check if they don't overlap but are right next to one another*/(p1bullets[i].x + 1 == p2bullets[a].x) || /*Check if bullets are overlapping*/(p1bullets[i].x == p2bullets[a].x)))
		{
		  p1bullets[i].health = 0;
		  p2bullets[a].health = 0;
		}
	      sleepSeconds(0, 1);
	    }
	}
      }
    
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    clear();
    for (int i = 0; i < 160; i++) { //Prints out all blocks
      if (blocks[i].health != 0) {
	mvprintw(blocks[i].y, blocks[i].x, blockChar);
      }
    }
    for (int i = 0; i < p1bulletNumber + 1; i++) {
      if (p1bullets[i].health != 0) {
	mvprintw(p1bullets[i].y, p1bullets[i].x, ">");
      }
    }
    for (int i = 0; i < p2bulletNumber + 1; i++) {
      if (p2bullets[i].health != 0) {
	mvprintw(p2bullets[i].y, p2bullets[i].x, "<");
      }
    }
    //mvprintw(p1bullets[p1bulletNumber].y, p1bullets[p1bulletNumber].x, "•");
    if (player1.canShoot == 0)
    {
      mvprintw(player1.y, player1.x, "8>===");
    } else if (player1.canShoot == 1) {
      mvprintw(player1.y, player1.x, "8=>==");
    } else if (player1.canShoot == 2) {
      mvprintw(player1.y, player1.x, "8==>=");
    } else if (player1.canShoot >= 3) {
      mvprintw(player1.y, player1.x, "8===>");
    }
    if (player2.canShoot == 0)
    {
      mvprintw(player2.y, player2.x, "===<8");
    } else if (player2.canShoot == 1) {
      mvprintw(player2.y, player2.x, "==<=8");
    } else if (player2.canShoot == 2) {
      mvprintw(player2.y, player2.x, "=<==8");
    } else if (player2.canShoot >= 3) {
      mvprintw(player2.y, player2.x, "<===8");
    }
    mvprintw(3, 1, "%d", player1.canShoot);
    mvprintw(4, 1, "%d", player2.canShoot);
    mvprintw(3, 3, "%d", 3);
    refresh();
    totalFramesCounter++;
    player1.canShoot += 1;
    player2.canShoot += 1;
    if (winner != 0) {
      break;
    }
  }
  gameOver(winner);
  endwin();
  
  
  return 0;
}
