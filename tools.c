/*
# File:   Makefile
# Author: J. Walsh and D. Porter
# Date:   15 Oct 2019
# Definition: contains codes for the functions specified in tools.h
*/

#include "tools.h"
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>




//===================================================
//----------------main_init--------------------------
//===================================================

void main_init (void)
{
    system_init ();
    navswitch_init ();
    ir_uart_init ();
}

//===================================================
//----------------screen_init------------------------
//===================================================

void screen_init (int pacer_rate, int message_rate)
{
    tinygl_init (pacer_rate);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (message_rate);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    pacer_init (pacer_rate);
}

void update(void)
{
    pacer_wait ();
    tinygl_update ();
    navswitch_update ();
}

int hold_till_press(int end_state)
{
    while(1) {
        update();
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
    return end_state;
}

//===================================================
//----------------toggle_character-------------------
//===================================================

char toggle_character (int direction, char char_adr)
{
    char letter = 'R';
    /* to scroll through the options R, P and S  */
    if (direction == 1) {
        if (char_adr == 'R') {
            letter = 'P';
        } else if (char_adr == 'P') {
            letter = 'S';
        } else {
            letter = 'R';
        }
    /* to reverse scroll through the options R, P and S */
    } else {
        if (char_adr == 'R') {
            letter = 'S';
        } else if (char_adr == 'P') {
            letter = 'R';
        } else {
            letter = 'P';
        }
    }
    return letter;
}

//===================================================
//----------------display_character------------------
//===================================================

void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

//===================================================
//----------------calc_result------------------------
//===================================================

int calc_result (char player_selection, char opponent_selection)
{
    int won = 3;
    int lost = 4;
    int draw = 2;

    if (player_selection == opponent_selection) { //players have drawn
        return draw;
    } else {
        if (player_selection == 'R') {
            if (opponent_selection == 'S') {
                return won;
            } else { // opponent = 'P'
                return lost;
            }
        } else if (player_selection == 'P') {
            if (opponent_selection == 'R') {
                return won;
            } else { // opponent = 'S'
                return lost;
            }
        } else {
            if (opponent_selection == 'P') {
                return won;
            } else { // opponent = 'R'
                return lost;
            }
        }
    }
}
