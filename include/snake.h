/**
 * \file            snake.h
 * \brief           header file for snake.c
 */

/*
 * Copyright (c) 2023 Jesse Haynie
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of SnakeGame.
 *
 * Author:          Jesse Haynie <jessehaynie02@gmail.com>
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <SDL_ttf.h>
#include <SDL_main.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define UP_ARROW_KEY    82
#define DOWN_ARROW_KEY  81
#define LEFT_ARROW_KEY  80
#define RIGHT_ARROW_KEY 79
#define ENTER_KEY       40
#define ESCAPE_KEY      41
#define GAME_DELAY      100

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief snake_segment is a struct that is used in a doubly linked list
 *
 * snake_segment contains the integer x and y coordinates as well as the pointers to the 
 * next and previous body segment.
 */
typedef struct snake_segment {
    int16_t x;     /**< The x coordinate of the snake segment */
    int16_t y;     /**< The y coordinate of the snake segment */
    struct snake_segment* next;     /**< The pointer that holds the location of the next snake segment */
    struct snake_segment* prev;     /**< The pointer that holds the location of the previous snake segment */
} snake_segment;

snake_segment* head = NULL;     /**< head pointer holds the location of the head of the snake*/
snake_segment* tail = NULL;     /**< tail pointer holds the location of the tail of the snake*/

/**
 * @brief This function creates a struct type snake segment.
 *
 * This function will take in an x and y coordinate that contains the location
 * of a snake segment and allocate memory for the snake segment that is of type struct.
 * 
 * @param x The x coordinate of the snake segment.
 * @param y The y coordinate of the snake segment.
 * @return This function returns the snake segment that was created.
 */
snake_segment* create(int16_t x, int16_t y);

/**
 * @brief This function inserts a newly created snake segment and inserts at the tail of the list.
 *
 * insert_tail takes in an x and y coordinates that describes the location of the snake segment
 * and use the create function to create a new snake segment. When the new segment is created 
 * it will then be placed at the end of the doubly linked list and if the list is empty it will
 * be placed at the head of the snake.
 * 
 * @param x The x coordinate of the snake segment.
 * @param y The y coordinate of the snake segment.
 * @return This function return is void.
 */
void insert_tail(int16_t x, int16_t y);

/**
 * @brief This function returns the size of the snake that is a doubly linked list.
 *
 * snake_size iterates through the doubly linked list counting each segment of the snake
 * and then it returns the size of the snake.
 * 
 * @return This function return the size of the doubly linked list aka the snake as an integer.
 */
int16_t snake_size();

/**
 * @brief This function deallocates/deletes the memory used for the snake list.
 *
 * delete_snake frees all memory used for the doubly linked list as when this function is called
 * the snake is no longer needed.
 * 
 * @return This function return is void.
 */
void delete_snake();

/**
 * @brief This function generates the new location for the red cube for the snake to find.
 *
 * gen_randome_red_cube takes in the pointers that contain the address of the variables that hold
 * the positions of the current red cubes and regenerates new values randomly.
 * 
 * @param redx_ptr The x coordinate of the red cube.
 * @param redy_ptr The y coordinate of the red cube.
 * @return This function return is void.
 */
void gen_random_red_cube(int16_t* const redx_ptr, int16_t* const redy_ptr);

/**
 * @brief This function moves the snake to its updated position.
 *
 * move_snake increments each segment of the snake to its updated position to simulate a moving snake.
 * renderer_ptr is passed into the function to be able to put the image onto the screen.
 * 
 * @param renderer_ptr This is a pointer to the render struct.
 * @return This function return is void.
 */
void move_snake(SDL_Renderer** renderer_ptr);

/**
 * @brief This function checks to make sure the head of the snake did not collide with itself.
 *
 * check_for_collision takes the coordinates of the head of the snake and compares those values with
 * the rest of the snakes positions and if a collision is detected a 1 is returned, otherwise 0.
 * 
 * @return This function returns a 1 when a collision is detected, otherwise 0.
 */
int16_t check_for_collision();

/**
 * @brief This function checks to see if a newly generated red cube did not generate on the snakebody.
 *
 * check_red_cube_valid makes sure when a new red cube is generated it does not spawn on the snake and if 
 * it does then it returns 0, otherwise 1.
 * 
 * @param redx_ptr The x coordinate of the red cube.
 * @param redy_ptr The y coordinate of the red cube.
 * @return This function returns a 0 when the red cube is not valid and 1 when the red cube is valid.
 */
int16_t check_red_cube_valid(int16_t* const redx_ptr, int16_t* const redy_ptr);

/**
 * @brief This function renders and outputs the snake onto the screen.
 *
 * This creates all the objects that need to be on the screen.
 * 
 * @param renderer_ptr This is a pointer to the render struct.
 * @return This function return is void.
 */
void render_snake(SDL_Renderer** renderer_ptr);

/**
 * @brief This function resets the game to the starting position.
 *
 * restart_game gets called when the snake collides either with a border or itself and resets the game
 * to the starting position with the snake back to size of 1.
 * 
 * @param renderer_ptr This is a pointer to the render struct.
 * @param last_key_pressed_ptr This is a pointer to change the last key pressed.
 * @param x_ptr The x coordinate of the snake segment.
 * @param y_ptr The y coordinate of the snake segment.
 * @param redx_ptr The x coordinate of the red cube.
 * @param redy_ptr The y coordinate of the red cube.
 * @return This function returns the newest highscore.
 */
int16_t restart_game(SDL_Renderer** renderer_ptr, int16_t* const last_key_pressed_ptr, int16_t* const x_ptr, int16_t* const y_ptr, int16_t* const redx_ptr, int16_t* const redy_ptr);

#ifdef __cplusplus
}
#endif

#endif