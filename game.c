/*
# File:   Makefile
# Author: J. Walsh and D. Porter
# Date:   15 Oct 2019
# Definition: game controller for a rock paper scissors game
    that is best of 5 and resets at the end so you can play again
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

#define PACER_RATE 2000
#define MESSAGE_RATE 30


/* calculates if the player lost or won the best of 5 match and displays
    the corresponding message*/
int post_match_annalysis(int player_score, int opponent_score)
{
    if (opponent_score == 3) {
        tinygl_text("Match lost");
    } else if (player_score == 3){
        tinygl_text("Match won");
    }
    int reset_value = -1;
    return reset_value;
}

/*checks if match has been won if not returns int state to go back
    to playing screen*/
int post_round_annalysis(int state, int player_score, int opponent_score)
{
    if (opponent_score >= 2 || player_score >= 2) {
        state = 5; //end match
    } else {
        state = 0; //playing
    }
    return state;
}


int main (void)
{
    char player_selection = 'P';
    char opponent_selection;
    int recieved_selection = 0;
    int state = -1;
    int player_score = 0;
    int opponent_score = 0;
    int end_state;

    main_init ();  //initializes system
    screen_init (PACER_RATE, MESSAGE_RATE);  //initializes led matrix

    while (1)
    {
        update();
        switch (state) {
            case -1: //waiting
                tinygl_text("Paper Scissors Rock Best of 5  ");
                state = hold_till_press(0);
                break; //displays text until navswitch is pressed

            case 0: //selecting
                display_character (player_selection);
                if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                    player_selection = toggle_character(1, player_selection);
                }
                if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                    player_selection = toggle_character(0, player_selection);
                }
                if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                    ir_uart_putc (player_selection);
                    state = 1; //playing
                }
                break; //allows player to scroll through selecions until navswitch is pressed

            case 1: //playing
                if (recieved_selection) {
                    end_state = calc_result(player_selection, opponent_selection);
                    state = hold_till_press(end_state);
                    //2 = draw round, 3 = won round, 4 = lost round
                }
                display_character('?'); //while waiting for opponent_selection displays '?'
                break; //displays '?' character if selection is made first pending opponents selection

            case 2: //draw round
                tinygl_text("round draw");
                end_state = post_round_annalysis(state, player_score, opponent_score);
                state = hold_till_press(end_state);
                recieved_selection = 0;
                break; //displays round draw message and calculates if match is won/lost

            case 3: //won round
                tinygl_text("round won");
                end_state = post_round_annalysis(state, player_score, opponent_score);
                state = hold_till_press(end_state);
                player_score += 1;
                recieved_selection = 0;
                break; //displays round won message and calculates if match is won/lost

            case 4: //lost round
                tinygl_text("round lost");
                end_state = post_round_annalysis(state, player_score, opponent_score);
                state = hold_till_press(end_state);
                opponent_score += 1;
                recieved_selection = 0;
                break; //displays round lost message and calculates if match is won/lost

            case 5: //end match
                end_state = post_match_annalysis(player_score, opponent_score);
                player_score = 0;
                opponent_score = 0;
                recieved_selection = 0;
                state = hold_till_press(end_state);
                break; //displays match won/lost message and resets values
        }

        if (ir_uart_read_ready_p ()) {
            opponent_selection = ir_uart_getc ();
            recieved_selection = 1; //effective boolean that a selection has been buffered
        }


    }
