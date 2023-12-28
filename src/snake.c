#include "snake.h"

int main(int argc, char* argv[]) {
    int16_t x = (SCREEN_WIDTH / 2), y = (SCREEN_HEIGHT / 2), running = 1, last_key_pressed = 0, startup = 1, restart_flag = 0;
    int16_t redx = 0, redy = 0, highscore = 0;
    int16_t* const redx_ptr = &redx;
    int16_t* const redy_ptr = &redy;
    int16_t* const last_key_pressed_ptr = &last_key_pressed;
    clock_t start_time = 0;

    insert_tail(x, y);

    int16_t* const x_ptr = &head->x;
    int16_t* const y_ptr = &head->y;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("Error: SDL failed to initialize ttf\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    gen_random_red_cube(redx_ptr, redy_ptr);

    SDL_Renderer** renderer_ptr = &renderer;

    TTF_Font* font_one = TTF_OpenFont("ttf/RubikDoodleShadow-Regular.ttf", 50);
    if (font_one == NULL) {
        printf("ERROR: TTF_OpenFont\n");
        return 1;
    }

    TTF_Font* font_two = TTF_OpenFont("ttf/Roboto-Regular.ttf", 25);
    if (font_two == NULL) {
        printf("ERROR: TTF_OpenFont\n");
        return 1;
    }

    SDL_Color color = {0, 255, 0, 255};

    SDL_Surface* solid_one = TTF_RenderText_Solid(font_one, "Snake", color);
    if (solid_one == NULL) {
        printf("ERROR: TTF_RenderText_Solid\n");
        return 1;
    }

    SDL_Texture* text_image_one = SDL_CreateTextureFromSurface(renderer, solid_one);
    int16_t alignment_x = (SCREEN_WIDTH - solid_one->w) / 2;
    int16_t alignment_y = (SCREEN_HEIGHT - solid_one->h) / 4;
    SDL_Rect pos_one = {alignment_x, alignment_y, solid_one->w, solid_one->h};
    SDL_FreeSurface(solid_one);

    SDL_Surface* solid_two = TTF_RenderText_Solid(font_two, "Press ENTER to play.", color);
    if (solid_two == NULL) {
        printf("ERROR: TTF_RenderText_Solid\n");
        return 1;
    }

    SDL_Texture* text_image_two = SDL_CreateTextureFromSurface(renderer, solid_two);
    alignment_x = (SCREEN_WIDTH - solid_two->w) / 2;
    alignment_y = SCREEN_HEIGHT - ((SCREEN_HEIGHT - solid_two->h) / 3);
    SDL_Rect pos_two = {alignment_x, alignment_y, solid_two->w, solid_two->h};
    SDL_FreeSurface(solid_two);

    while (running) {
        SDL_Event event;
        while (startup) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case ENTER_KEY:
                            startup = 0;
                            SDL_DestroyTexture(text_image_one);
                            SDL_DestroyTexture(text_image_two);
                            break;
                        case ESCAPE_KEY:
                            SDL_DestroyTexture(text_image_one);
                            SDL_DestroyTexture(text_image_two);
                            TTF_Quit();
                            SDL_Quit();
                            return 0;
                            break;
                    default:
                        break;
                    }
                    break;
                case SDL_QUIT:
                    SDL_DestroyTexture(text_image_one);
                    SDL_DestroyTexture(text_image_two);
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                    break;
                default:
                    break;
                }
            }
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, text_image_one, NULL, &pos_one);
            SDL_RenderCopy(renderer, text_image_two, NULL, &pos_two);
            SDL_RenderPresent(renderer);
        }

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if ((event.key.keysym.scancode == UP_ARROW_KEY) || (event.key.keysym.scancode == DOWN_ARROW_KEY) ||
                        (event.key.keysym.scancode == LEFT_ARROW_KEY) || (event.key.keysym.scancode == RIGHT_ARROW_KEY)) {
                        if ((head->y >= 0) && (head->y < SCREEN_HEIGHT)  && (head->x >= 0) && (head->x < SCREEN_WIDTH)) {
                            last_key_pressed = event.key.keysym.scancode;
                        } else {
                            restart_flag = 1;
                        }
                    }
                    else if (event.key.keysym.scancode == ESCAPE_KEY) {
                        SDL_DestroyTexture(text_image_one);
                        SDL_DestroyTexture(text_image_two);
                        restart_game(renderer_ptr, last_key_pressed_ptr, x_ptr, y_ptr, redx_ptr, redy_ptr);
                        TTF_Quit();
                        SDL_Quit();
                        return 0;
                    }
                    break;
                case SDL_QUIT:
                    running = 0;
                    delete_snake();
                    break;
                default:
                    break;
            }
        }
        if (head->x == redx && head->y == redy) {
            gen_random_red_cube(redx_ptr, redy_ptr);

            if (snake_size() == 1) {
                switch (last_key_pressed) {
                    case UP_ARROW_KEY:
                        insert_tail(head->x, head->y - 10);
                        break;
                    case DOWN_ARROW_KEY:
                        insert_tail(head->x, head->y + 10);
                        break;
                    case LEFT_ARROW_KEY:
                        insert_tail(head->x - 10, head->y);
                        break;
                    case RIGHT_ARROW_KEY:
                        insert_tail(head->x + 10, head->y);
                    default:
                        break;
                }
            } else {
                if (tail->x == tail->prev->x) {
                    if (tail->y < tail->prev->y) {
                        insert_tail(tail->x, tail->y - 10);
                    } else if (tail->y > tail->prev->y) {
                        insert_tail(tail->x, tail->y + 10);
                    }
                } else if (tail->y == tail->prev->y) {
                    if (tail->x < tail->prev->x) {
                        insert_tail(tail->x - 10, tail->y);
                    } else if (tail->x > tail->prev->x) {
                        insert_tail(tail->x + 10, tail->y);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        switch (last_key_pressed) {
            case UP_ARROW_KEY:
                if (head->y >= 0) {
                    move_snake(renderer_ptr);
                } else {
                    restart_flag = 1;
                }
                head->y -= 10;
                break;
            case DOWN_ARROW_KEY:
                if (head->y < SCREEN_HEIGHT) {
                    move_snake(renderer_ptr);
                } else {
                    restart_flag = 1;
                }
                head->y += 10;
                break;
            case LEFT_ARROW_KEY:
                if (head->x >= 0) {
                    move_snake(renderer_ptr);
                } else {
                    restart_flag = 1;
                }
                head->x -= 10;
                break;
            case RIGHT_ARROW_KEY:
                if (head->x < SCREEN_WIDTH) {
                    move_snake(renderer_ptr);
                } else {
                    restart_flag = 1;
                }
                head->x += 10;
                break;
            default:
                break;
        }

        if (restart_flag) {
            int16_t latest_score = 0;
            char latest_score_str[50] = "", highscore_str[50] = "", current_score[50] = "", high_score[50] = "";

            latest_score = snake_size();
            highscore = restart_game(renderer_ptr, last_key_pressed_ptr, x_ptr, y_ptr, redx_ptr, redy_ptr);

            sprintf(latest_score_str, "%d", latest_score);
            strcat(current_score, "Score: ");
            strcat(current_score, latest_score_str);

            sprintf(highscore_str, "%d", highscore);
            strcat(high_score, "High Score: ");
            strcat(high_score, highscore_str);

            SDL_Surface* solid_three = TTF_RenderText_Solid(font_two, high_score, color);
            if (solid_three == NULL) {
                printf("ERROR: TTF_RenderText_Solid\n");
                return 1;
            }

            SDL_Texture* text_image_three = SDL_CreateTextureFromSurface(renderer, solid_three);
            if (text_image_three == NULL) {
                printf("text_image_three failed to initialize\n");
                return 1;
            }
            alignment_x = (SCREEN_WIDTH - solid_three->w) / 2;
            alignment_y = (SCREEN_HEIGHT - solid_three->h) / 2;
            SDL_Rect pos_three = {alignment_x, alignment_y, solid_three->w, solid_three->h};
            SDL_FreeSurface(solid_three);

            SDL_Surface* solid_four = TTF_RenderText_Solid(font_two, current_score, color);
            if (solid_four == NULL) {
                printf("ERROR: TTF_RenderText_Solid\n");
                return 1;
            }

            SDL_Texture* text_image_four = SDL_CreateTextureFromSurface(renderer, solid_four);
            if (text_image_four == NULL) {
                printf("text_image_four failed to initialize\n");
                return 1;
            }
            alignment_x = (SCREEN_WIDTH - solid_four->w) / 2;
            alignment_y = (SCREEN_HEIGHT - solid_four->h) / 6;
            SDL_Rect pos_four = {alignment_x, alignment_y, solid_four->w, solid_four->h};
            SDL_FreeSurface(solid_four);

            SDL_Surface* solid_five = TTF_RenderText_Solid(font_two, "Press ENTER to play.", color);
            if (solid_five == NULL) {
                printf("ERROR: TTF_RenderText_Solid\n");
                return 1;
            }

            SDL_Texture* text_image_five = SDL_CreateTextureFromSurface(renderer, solid_five);
            if (text_image_five == NULL) {
                printf("text_image_five failed to initialize\n");
                return 1;
            }
            alignment_x = (SCREEN_WIDTH - solid_five->w) / 2;
            alignment_y = SCREEN_HEIGHT - ((SCREEN_HEIGHT - solid_five->h) / 3);
            SDL_Rect pos_five = {alignment_x, alignment_y, solid_five->w, solid_five->h};
            SDL_FreeSurface(solid_five);

            while (restart_flag) {
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            switch (event.key.keysym.scancode) {
                                case ENTER_KEY:
                                    restart_flag = 0;
                                    SDL_DestroyTexture(text_image_three);
                                    SDL_DestroyTexture(text_image_four);
                                    SDL_DestroyTexture(text_image_five);
                                    break;
                                case ESCAPE_KEY:
                                    SDL_DestroyTexture(text_image_three);
                                    SDL_DestroyTexture(text_image_four);
                                    SDL_DestroyTexture(text_image_five);
                                    TTF_Quit();
                                    SDL_Quit();
                                    return 0;
                                    break;
                            default:
                                break;
                            }
                            break;
                        case SDL_QUIT:
                            SDL_DestroyTexture(text_image_three);
                            SDL_DestroyTexture(text_image_four);
                            SDL_DestroyTexture(text_image_five);
                            TTF_Quit();
                            SDL_Quit();
                            return 0;
                            break;
                        default:
                            break;
                    }
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, text_image_three, NULL, &pos_three);
                SDL_RenderCopy(renderer, text_image_four, NULL, &pos_four);
                SDL_RenderCopy(renderer, text_image_five, NULL, &pos_five);
                SDL_RenderPresent(renderer);
            }   
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        render_snake(renderer_ptr);

        int16_t check = check_for_collision();
        if (check) {
            restart_flag = 1;
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect redCube = {redx, redy, 10, 10};
        SDL_RenderFillRect(renderer, &redCube);

        SDL_RenderPresent(renderer);
        start_time = clock();
        while (clock() < start_time + GAME_DELAY) {}
    }

    TTF_Quit();
    SDL_Quit();

    return 0;
}

snake_segment* create(int16_t x, int16_t y) {
    snake_segment* new_segment = (snake_segment*) malloc(sizeof(snake_segment));
    if (new_segment == NULL) {
        printf("Error allocating memory for new snake segment\n");
        return NULL;
    }

    new_segment->x = x;
    new_segment->y = y;
    new_segment->prev = NULL;
    new_segment->next = NULL;

    return new_segment;
}

void insert_tail(int16_t x, int16_t y) {
    snake_segment* new_segment = create(x, y);

    if (new_segment != NULL) {
        if (head == NULL) {
            new_segment->prev = NULL;
            new_segment->next = NULL;
            head = new_segment;
            tail = new_segment;
            return;
        } else {
            tail->next = new_segment;
            new_segment->prev = tail;
            new_segment->next = NULL;
            tail = new_segment;
        }
    }
}

int16_t snake_size() {
    snake_segment* temp = head;
    int16_t count = 1;

    if (head == NULL) {
        return 0;
    }

    while (temp->next != NULL) {
        temp = temp->next;
        ++count;
    }

    return count;
}

void delete_snake() {
    snake_segment* temp = head;

    if (head == NULL && tail == NULL) {
        printf("Exiting delete_snake()\n");
        return;
    }

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
        temp = NULL;
    }
    head = NULL;
    tail = NULL;
}

void gen_random_red_cube(int16_t* const redx_ptr, int16_t* const redy_ptr) {
    srand(time(NULL));

    *redx_ptr = (rand() % (SCREEN_WIDTH / 10)) * 10;
    *redy_ptr = (rand() % (SCREEN_HEIGHT / 10)) * 10;

    while (check_red_cube_valid(redx_ptr, redy_ptr) == 0) {
        printf("invalid cube\n");
        gen_random_red_cube(redx_ptr, redy_ptr);
    }
}

void move_snake(SDL_Renderer** renderer_ptr) {
    snake_segment* temp = head;
    int16_t tempx_1 = temp->x, tempy_1 = temp->y, tempx_2 = 0, tempy_2 = 0;

    if (head == NULL) {
        printf("Error: Snake is null\n");
        return;
    }

    for (uint8_t i = 0; i < snake_size(); ++i) {
        if (temp->next != NULL) {
            tempx_2 = temp->next->x;
            tempy_2 = temp->next->y;
            temp->next->x = tempx_1;
            temp->next->y = tempy_1;
            tempx_1 = tempx_2;
            tempy_1 = tempy_2;
            temp = temp->next;
        }
    }
}

int16_t check_for_collision() {
    snake_segment* temp = head;

    if (head == NULL) {
        return 0;
    }

    for (uint8_t i = 0; i < snake_size(); ++i) {
        if (temp->next != NULL) {
            temp = temp->next;
            if ((head->x == temp->x) && (head->y == temp->y)) {
                return 1;
            }
        }
    }
    return 0;
}

int16_t check_red_cube_valid(int16_t* const redx_ptr, int16_t* const redy_ptr) {
    snake_segment* temp = head;

    if (head == NULL) {
        return 0;
    }

    for (uint8_t i = 0; i < snake_size(); ++i) {
        if (temp->x == *redx_ptr && temp->y == *redy_ptr) {
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

void render_snake(SDL_Renderer** renderer_ptr) {
    snake_segment* temp = head;

    if (head == NULL) {
        printf("Error: Snake is null\n");
        return;
    }

    for (uint8_t i = 0; i < snake_size(); ++i) {
        // printf("temp->x = %d temp->y = %d\n", temp->x, temp->y);
        SDL_Rect rect = {temp->x, temp->y, 10, 10};
        SDL_RenderFillRect(*renderer_ptr, &rect);
        temp = temp->next;
    }
}

int16_t restart_game(SDL_Renderer** renderer_ptr, int16_t* const last_key_pressed_ptr, int16_t* const x_ptr, int16_t* const y_ptr, int16_t* const redx_ptr, int16_t* const redy_ptr) {
    FILE* fp;
    int16_t latest_score = 0, highscore = 0;

    latest_score = snake_size();

    fp = fopen("highscore.txt", "r+");
    if(fp == NULL) {
        printf("Failed to open highscore.txt\n");
        return 1;
    }

    fscanf(fp, "%hu", &highscore);

    if(latest_score > highscore) {
        highscore = latest_score;

        fseek(fp, 0, SEEK_SET);
        fprintf(fp, "%hu", highscore);
    }

    fclose(fp);
    
    delete_snake();

    insert_tail((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2));

    *last_key_pressed_ptr = 0;

    SDL_Rect rect = {*x_ptr, *y_ptr, 10, 10};
    SDL_RenderFillRect(*renderer_ptr, &rect);

    gen_random_red_cube(redx_ptr, redy_ptr);

    return highscore;
}