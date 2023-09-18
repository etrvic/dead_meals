#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;

int KEYS[322];
int game_state = 1;
int movement_speed = 5;

bool init();
bool loadMedia();
void close();

void key_down();
void move_player();

SDL_Window* window = NULL;
SDL_Surface* window_surface = NULL;
SDL_Surface* brick = NULL;
SDL_Surface* player = NULL;

SDL_Rect player_position, temp;

int player_relative_position = 0;

int map[] = {1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1};
int len = 21;

void make_floor();

void move_left();
void move_right();

int main( int argc, char* args[] )
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            temp.y = SCREEN_HEIGHT - 100;
            temp.x = 0;
            player_position.y = SCREEN_HEIGHT - 200;
            player_position.x = 300;
            SDL_BlitSurface( player, NULL, window_surface, &player_position );
            make_floor();
            while(game_state) {
                key_down();
                move_player();
                SDL_BlitSurface( player, NULL, window_surface, &player_position );
                SDL_UpdateWindowSurface( window );
                SDL_Delay(20);
                
            }
        }
    }

    close();

    return 0;
}

void jump() {
    int t = 40;
    float a = 0.33;
    while(t){
        player_position.y -= a * t;
        key_down();
        SDL_FillRect(window_surface, NULL, 0x000000);
        if(KEYS[SDLK_a] ==1 && KEYS[SDLK_m]==0){ move_left();}
        else if(KEYS[SDLK_d] ==1 && KEYS[SDLK_m]==0){ move_right();}
        else{make_floor();}
        SDL_BlitSurface( player, NULL, window_surface, &player_position );
        
        SDL_UpdateWindowSurface( window );
        t--;
        SDL_Delay(20);
        
    }
    while(player_position.y < SCREEN_HEIGHT - 200) {
        if(player_position.y + t * a > 700)
            player_position.y = 700;
        else
            player_position.y += t * a;
        
        key_down();
        SDL_FillRect(window_surface, NULL, 0x000000);
        if(KEYS[SDLK_a] ==1 && KEYS[SDLK_m]==0){ move_left();}
        else if(KEYS[SDLK_d] ==1 && KEYS[SDLK_m]==0){ move_right();}
        else{make_floor();}
        SDL_BlitSurface( player, NULL, window_surface, &player_position );
        make_floor();
        SDL_UpdateWindowSurface( window );
        t++;
        SDL_Delay(20);
    }
}

void make_floor() {
    int start = player_relative_position / 100;
    temp.x = -player_relative_position % 100 -100;
    for(int i = 0; i <= 11; i++) {
        if(map[i + start] == 1)
            SDL_BlitSurface( brick, NULL, window_surface, &temp );
        temp.x =-player_relative_position % 100+ 100*i;
    }
}

void move_right() {
    if(player_relative_position < len*100-movement_speed-SCREEN_WIDTH/3*4 && player_position.x == SCREEN_WIDTH /3) {
        player_relative_position +=5;
        
        SDL_FillRect(window_surface, NULL, 0x000000);
        make_floor();
        
    }
    else if (player_position.x < SCREEN_WIDTH - movement_speed) {
        player_position.x += movement_speed;
        
        SDL_FillRect(window_surface, NULL, 0x000000);
        make_floor();
    }
}

void move_left() {
    if(player_relative_position > movement_speed && player_position.x == SCREEN_WIDTH /3) {
        
        player_relative_position -=5;
        SDL_FillRect(window_surface, NULL, 0x000000);
        make_floor();
        
    }else if(player_position.x > 0 + movement_speed) {
        
        player_position.x -= movement_speed;
        SDL_FillRect(window_surface, NULL, 0x000000);
        make_floor();
    }
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            window_surface = SDL_GetWindowSurface( window );
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    brick = SDL_LoadBMP( "brick5x.bmp" );
    player = SDL_LoadBMP( "mainStill5x.bmp" );

    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface( brick );
    brick = NULL;

    //Destroy window
    SDL_DestroyWindow( window );
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void move_player() {
    if(KEYS[SDLK_a] ==1 && KEYS[SDLK_m]==0){ move_left();}
    if(KEYS[SDLK_d] ==1 && KEYS[SDLK_m]==0){ move_right();}
     if(KEYS[SDLK_w] ==1 && KEYS[SDLK_m]==0){ jump();}
     if(KEYS[SDLK_s] ==1 && KEYS[SDLK_m]==0){ printf("down\n");}
}

void key_down() {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // check for messages
            switch (event.type) {
                // exit if the window is closed
            case SDL_QUIT:
                game_state = 0; // set game state to done,(do what you want here)
                break;
                // check for keypresses
            case SDL_KEYDOWN:
                KEYS[event.key.keysym.sym] = true;
                break;
            case SDL_KEYUP:
                KEYS[event.key.keysym.sym] = false;
                break;
            default:
                break;
            }
        } // end of message processing
}

