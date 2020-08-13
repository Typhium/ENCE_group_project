/*
# File:   Makefile
# Author: J. Walsh and D. Porter
# Definition: header file for the tools module that holds all the Utility
    functions needed for game.c
*/

#ifndef TOOLS_H
#define TOOLS_H

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"

/* function for toggling through options from paper (P),
   scissors (S) or rock (R).
   returns the new character the option
   should be in game.c given a direction (North = 1, South = 0) and
   the current char (P, S, R)
*/
char toggle_character (int direction, char char_adr);

/* displays current character on the screen */
void display_character (char character);

/* initialises the backend system, navswitch and ir_uart utilities */
void main_init(void);

/* initialises the front end display from the required message rates */
void screen_init(int pacer_rate, int message_rate);

/* compares the players selected character (R, P or S) to the opponents
   selected letter and returns whether they've won, lost or drawn
   against their opponent.
   function returns an integer:
        3 (if you won)
        4 (if you lost)
        2 (if you drew)
*/
int calc_result (char player_selection, char opponent_selection);

/*updates led matrix display so messages display correctly*/
void update(void);

/*holds the code in a while loop until the navswitch is pushed
    then breaksand returns the int end_state inputed*/
int hold_till_press(int end_state);

#endif //TOOLS_H
