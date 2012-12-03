#include"Copter.h"

void start();                       // prototype for start function
void pause();                       //prototype for pause function
void gameend();                     // declaration for ending function
void rothel();                      // function for rotation of the wings of helicopter
void cleanup();
void level();
void load_image();
void rectangle();
void init();
void replay();                      // for replaying game
void apply_surface( int, int, SDL_Surface*, SDL_Surface* );    // for blitting surfaces
void apply_message( int , int , char [], SDL_Surface* ,SDL_Color ,TTF_Font* );
int collision(SDL_Rect,SDL_Rect);
void variable_init();

SDL_Rect rect;                     // rectangle for holding helicopter
SDL_Rect rect1;                    // for holding first obstacle
SDL_Rect rect2;                    // for holding second obstacle
SDL_Rect rect3;                    // for holding third obstacle
SDL_Rect rect4;                    // for holding wall
SDL_Rect rect_bonus[2];            // for holding the bonus images

SDL_Surface* screen = NULL;        // for screen
SDL_Surface* optbg = NULL;         // for optimized bg
SDL_Surface* oimage = NULL;        // for optimised helicopter
SDL_Surface* oobs = NULL;          // for optimised obstacle
SDL_Surface* oend = NULL;          // for optimised image of game end
SDL_Surface* osuru = NULL;         // for optimised image of game start
SDL_Surface* oscore = NULL;        // for optimised image of score board bg
SDL_Surface* temp = NULL;          // for temporarily holding images
SDL_Surface* wall = NULL;         // for upper and down walls
SDL_Surface* sound_playing = NULL;         // for playing sound image
SDL_Surface* sound_muted = NULL;           // for muted sound image
SDL_Surface* bonus_image[2];               // for saving bonus images
Mix_Music *music = NULL;                   //The music that will be played
int high_score(int);


int quit=0;                       // variable to terminate main game loop
int r=1;                          // variable used in rothel to rotate helicopter
int x=0;                          // variable for changing x coordinates of wall used
int sound_var = 1;                // for saving the currently sound is playing or paused  0- paused ,1-playing
int life_var=3;                   // for life
int speed = 5;                    // variable for determining the speed of the helicopter
int speedobs = 4;                 // variable for determining the speed of the obstacle
char level_var_char[5];           // for storing level
int score_var=1;                  // for saving the score
int score_var_temp=0;             // for counting the number of loops completed
int started_ticks=0;              // for countng time elapsed tillnow
char score_var_char[5];           // for storing score
char time_str[10];                // for storing time elapsed in character format from started_ticks
int level_var =1;                 // for saving the level
char life_var_char[5];            // for storing life in character format from life_var
int current_time;                 // for saving direct value fron getticks function
int position_bonus;               // for determining the random position of the bonus images
int k;


TTF_Font *font[3] ;                        // declaration of fonts used
SDL_Color textcolor0 = {0,0,0} ;           // 0 for score board
SDL_Color textcolor1 = {0,0,0} ;           // 1 for last display message
SDL_Color textcolor2 = {255,255,255} ;     // 2 for level increase message

SDL_Event event;

void init()
{
    Mix_OpenAudio( 22050                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  , MIX_DEFAULT_FORMAT, 2, 4096 ); // initialise sound.
    TTF_Init();                                          // initialise ttf of sdl
    font[0] = TTF_OpenFont( "font2.ttf", 18 );           // 0 for score board
    font[1] = TTF_OpenFont( "font2.ttf", 50 );           // 1 for last display message
    font[2] = TTF_OpenFont( "font2.ttf", 30 );           // 2 for level increase message
    SDL_Init(SDL_INIT_EVERYTHING);                       // initialize SDL
    srand(time(NULL));                                   // for changing the rand() according to time
    SDL_WM_SetIcon(SDL_LoadBMP("images/icon.bmp"), NULL);                                    // for icon on display window
//    screen = SDL_SetVideoMode(Screen_Width,Screen_Height,Screen_BBP,SDL_FULLSCREEN );
    screen = SDL_SetVideoMode(Screen_Width,Screen_Height,Screen_BBP,SDL_SWSURFACE );         // creating playing window (noframe ,resizable,fullscreen)
    SDL_WM_SetCaption("The Copter Game",NULL);                                               // for giving title for window
    music = Mix_LoadMUS( "beat.wav" );
}
int main(int argc,char* argv[])
{
    Uint8 *keys;                                      // declaring for key
    init();                                           // function to initialise different functions of SDL
    load_image();                                     // function to load images
    start();                                          // calling start() function for displayin startup image
    rectangle();                                      // function for declaring different coordinates of rectangle

//********** main game loop*******************//
    while (quit==0)                                              // game ocntinues till quit=0
    {
          FILE *fp;
          char c[10];
          int i=0;
          fp = fopen("hi.dat", "r");
          do
          {
                c[i] = getc(fp);
                i++;
          }
          while (c[i] != EOF);
          k=atoi(c);
          fclose(fp);

          srand(time(NULL));                                     // for changing the rand() according to time
          if ( Mix_PausedMusic() == 1 )
          {
                 sound_var=0;
          }
          else if(Mix_PlayingMusic() == 1)
          {
                  sound_var=1;
          }

          rothel();                                              // calling rothel() function different images each time
          apply_surface(0,0,optbg,screen);                       // for displaying the background image
          score_var_temp++;

//          itoa(life_var,life_var_char,10);                       // converting the int life_var into char life_var_char
  //        itoa(level_var,level_var_char,10);                     // converting the int level into char

          current_time = SDL_GetTicks();
          started_ticks = (current_time)/1000;
         // itoa(started_ticks, time_str, 10);                     // 10 - decimal;
        //  itoa(score_var, score_var_char, 10);                   // 10 - decimal;

          SDL_BlitSurface(oimage,NULL,screen,&rect);             // for blitting the helicopter
          SDL_Delay(5);

          rect1.x=rect1.x-speedobs;
          SDL_BlitSurface(oobs,NULL,screen,&rect1);              // for blitting first obstacle
          SDL_Delay(5);

          rect2.x=rect2.x-speedobs;
          SDL_BlitSurface(oobs,NULL,screen,&rect2);              // for blitting second obstacle
          SDL_Delay(5);

          rect3.x=rect3.x-speedobs;
          SDL_BlitSurface(oobs,NULL,screen,&rect3);              // for blitting third obstacle
          SDL_Delay(5);

          rect_bonus[0].x -= speedobs;
          SDL_BlitSurface(bonus_image[0],NULL,screen,&rect_bonus[0]);   // for blitting bonus images
          SDL_Delay(5);

          rect_bonus[1].x -= speedobs;
          SDL_BlitSurface(bonus_image[1],NULL,screen,&rect_bonus[1]);

          // for displaying walls
          apply_surface(x,0,wall,screen);
          apply_surface(x,570,wall,screen);
          x-=speedobs;
          if(x<=-1310)
          {x=0;}
          apply_surface(655,0,oscore,screen);             // for score board

          apply_message(665,20, "LEVEL ",screen ,textcolor0,font[0]);
          apply_message(743,20, level_var_char,screen ,textcolor0,font[0]);

          apply_message(665,50, "LIFE  ",screen ,textcolor0,font[0]);
          apply_message(743,50,life_var_char,screen ,textcolor0,font[0]);

          apply_message(665,80, "SCORE ",screen ,textcolor0,font[0]);
          apply_message(743,80, score_var_char,screen ,textcolor0,font[0]);

          apply_message(665,110,"TIME  ",screen ,textcolor0 ,font[0]);
          apply_message(743,110, time_str,screen ,textcolor0, font[0]);

          apply_message(665,140,"SOUND  ",screen ,textcolor0 ,font[0]);
          if(sound_var==1)
          {
               apply_surface(743,140,sound_playing,screen);
          }

          if(sound_var==0)
          {
               apply_surface(743,140,sound_muted,screen);
          }

          while(SDL_PollEvent(&event))
          {
                  if(keys[SDLK_s])     //escape key to quit
                  {

                        if ( Mix_PausedMusic() == 1 )
                        {
                            //Resume the music
                            Mix_ResumeMusic();
                        }
                        else  if(Mix_PlayingMusic() == 1)
                        {
                           Mix_PauseMusic();
                        }

                   }

                   if(keys[SDLK_p])     //escape key to quit
                   {
                           pause();
                   }

                   if (event.type==SDL_QUIT)
                   {
                            quit=1;
                   }
          }

          keys=SDL_GetKeyState(NULL);
          if(keys[SDLK_ESCAPE])     //escape key to quit
          {
                quit = 1;
          }

          else if (keys[SDLK_UP])
          {
                rect.y -= speed;
          }

          else
           {
                rect.y+=speed;
                if(rect.y + rect.h >= Screen_Height)
                {
                         rect.y=Screen_Height - rect.h;
                }
                //If there is no music playing
                if( Mix_PlayingMusic() == 0 && sound_var==1)
                {
                //Play the music
                if( Mix_PlayMusic( music, -1 ) == -1 )
                {
                          return 1;
                }
           }
          }

          if ( Mix_PausedMusic() == 1 )
          {
                 sound_var=0;
          }
          else if(Mix_PlayingMusic() == 1)
          {
                  sound_var=1;
          }

          if(rect1.x<=0)
          {
           rect1.x=702;
           rect1.y=rand()%150;
          }

          if(rect2.x<=0)
          {
           rect2.x=752;
           rect2.y=rand()%125+250;
          }

          if(rect3.x<=0)
          {
           rect3.x=800;
           rect3.y=rand()%75+475;
          }

          SDL_Flip(screen);                          // swaps screen buffer

          ///////////////////////////////////////////// collision detection//////////////////////////////

          if( collision( rect,rect1 ) == 1 )
          {
               if(life_var<1)
                   gameend();
               else
                   replay();
          }

          if( collision( rect,rect2 ) == 1 )
          {
              if(life_var<1)
                 gameend();
              else
                 replay();
          }

          if( collision( rect,rect3 ) == 1 )
          {
               if(life_var<1)
                   gameend();
               else
                   replay();
          }

          if( collision( rect,rect_bonus[0] ) == 1  )
          {
               rect_bonus[0].y = position_bonus;
               rect_bonus[0].x=2000;
               life_var--;
          }

          if( collision( rect,rect_bonus[1] ) == 1  )
          {
               rect_bonus[1].x=1000;
               rect_bonus[1].y = position_bonus;
               life_var++;
          }

          if( rect_bonus[0].x<= 0 )
          {
               rect_bonus[0].x=2000;
               if( (rand()%10) %2 == 0)
               {
                   rect_bonus[0].y=rand()%30+425; ;
               }
               else
                   rect_bonus[0].y=rand()%30+200;
          }

          if( rect_bonus[1].x<= 0 )
          {
               rect_bonus[1].x=1000;
               if( (rand()%10) %2 == 0)
               {
                   rect_bonus[1].y=rand()%30+425; ;
               }
               else
                   rect_bonus[1].y=rand()%30+200;
          }

          /////// colission with walls /////////////////
          if( (rect.y+rect.h) >= (Screen_Height- wall->h)||(rect.y <= wall->h) )

          {
               if(life_var<=1)
                   gameend();
                else
                   replay();
          }

          if( (rect_bonus[0].x <= 0) || (rect_bonus[1].x <= 0) )
          {
              if( 10 %2 == 0)
              {
                   position_bonus = 50;
              }
              else
                   position_bonus = rand()%30+200;
          }

          // calling level function for playing in different levels
          if(score_var%20==0)
          {
                  level();
                  score_var+=1;      // else this condition will be ture for continious 62 loops
          }

          if(life_var<1)
          {
                 gameend();
          }

          if(score_var_temp%31==0)
          {
              score_var+=1;
          }

          SDL_FreeSurface(oimage);
    }
    cleanup();
    return 0;
}

void start()
{
             Uint8 *keys;
             int quit_start=0;
             keys=SDL_GetKeyState(NULL);
             while(quit_start==0)
             {
                   SDL_BlitSurface(osuru,NULL,screen,NULL);
                   SDL_Flip(screen);

                   while(SDL_PollEvent(&event))
                   {
                         if(keys[SDLK_KP_ENTER])     //detecting numerical enter key to quit
                         {
                             quit_start=1;
                         }

                         if (event.type==SDL_QUIT)                          // for close button on the display window
                         {
                               quit_start=1;
                               quit=1;
                         }

                         switch(event.type)
                         {
                             case SDL_KEYDOWN:
                             switch(event.key.keysym.sym)
                             {
                                case '\r':                                        // for main enter key
                                quit_start=1;
                             }
                         }

                  }
             }
             SDL_FreeSurface(osuru);
}

void pause()
{
       int pau=0;
       Mix_PauseMusic();
       while(pau==0)
       {
           apply_message(250,275,"PAUSED",screen,textcolor2,font[2]);
           SDL_Flip(screen);
           while(SDL_PollEvent(&event))
           {
                switch(event.type)
                {
                   case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                            case 'P':
                            case 'p':
                                 {
                                  pau=1;
                                 }
                        }
                }
           }
           if(event.type==SDL_QUIT)                          // for close button on the display window
           {
                    pau=1;
                    quit=1;
           }
       }
       if(sound_var==1)
       {
            Mix_ResumeMusic();
       }
}

void gameend()
{

             Mix_PauseMusic();
             temp=IMG_Load("images/end.bmp");
             oend=SDL_DisplayFormat(temp);
             SDL_FreeSurface(temp);

             char a;
             Uint8 *keys;
             keys=SDL_GetKeyState(NULL);
             int quit1=0;

             SDL_BlitSurface(oend,NULL,screen,NULL);
             apply_message(100,325,"Your Score is : ",screen,textcolor1,font[1]);
             apply_message(570,325,score_var_char,screen,textcolor1,font[1]);

             if(score_var>k)
             {
                     apply_message(100,500,"CONGRATULATION HIGH SCORE",screen,textcolor1,font[2]);
                     FILE *fp;
                     fp = fopen("hi.dat","w");
                     fprintf(fp,"%d",score_var);
                     fclose(fp);
             }

             while(quit1==0)
             {
                 SDL_Flip(screen);
                 if(keys[SDLK_ESCAPE])     //escape key to quit
                 {
                     quit = 1;
                     quit1 = 1;

                 }

                 while(SDL_PollEvent(&event))
                 {
                       if (event.type==SDL_QUIT)                          // for close button on the display window
                       {
                               quit1=1;
                               quit=1;
                       }

                       if(keys[SDLK_KP_ENTER])     //detecting numerical enter key to quit
                       {
                               variable_init();
                               quit1=1;
                       }

                       switch(event.type)
                       {
                            case SDL_KEYDOWN:
                            switch(event.key.keysym.sym)
                            {
                                  case '\r':        // for detecting alphabetical enter
                                  {
                                       variable_init();
                                       quit1=1;
                                  }
                            }
                       }
                 }
             }



             if(sound_var==1)
             {
                  Mix_ResumeMusic();
             }
            SDL_FreeSurface(oend);
}


void rothel()
{
     if(r==1)
     {
             temp=IMG_Load("images/hel1.bmp");
             oimage=SDL_DisplayFormat(temp);
             SDL_FreeSurface(temp);
             r=2;
     }
     else if(r==2)
     {
             temp=IMG_Load("images/hel2.bmp");
             oimage=SDL_DisplayFormat(temp);
             SDL_FreeSurface(temp);
             r=3;
     }
     else
     {
             temp=IMG_Load("images/hel3.bmp");
             oimage=SDL_DisplayFormat(temp);
             SDL_FreeSurface(temp);
             r=1;
     }
     //******** color keying  *************//
     SDL_SetColorKey( oimage, SDL_SRCCOLORKEY, SDL_MapRGB( oimage->format, 0, 0, 0 ) );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void apply_message( int x, int y, char nep[20], SDL_Surface* destination ,SDL_Color textColor1, TTF_Font *font1 )
{
    // Holds offsets
    SDL_Rect offset;
    SDL_Surface *message;
    message = TTF_RenderText_Blended( font1, nep, textColor1 );

    // Get offsets
    offset.x = x;
    offset.y = y;

    // Blit
    SDL_BlitSurface( message, NULL, destination, &offset );
    SDL_FreeSurface(message);
}

void cleanup()
{
    SDL_FreeSurface(oobs);
    SDL_FreeSurface(oimage);
    SDL_FreeSurface(optbg);
    SDL_FreeSurface(screen);
    TTF_CloseFont( font[0] );
    TTF_Quit();
    Mix_FreeMusic( music );             // free music
    Mix_CloseAudio();                   //Quit SDL_mixer
    SDL_Quit();                         //quit SDL
}

void level()
{
             Mix_PauseMusic();          // pausing playing music
             int quit1=0;               // for playing this functions loop
             Uint8 *keys;
             level_var++;               // for increasing the level
             //itoa(level_var,level_var_char,10);                     // converting the int level into char
             keys=SDL_GetKeyState(NULL);

             while(quit1==0)
             {

                apply_message(250,275,"LEVEL : ",screen,textcolor2,font[2]);
                apply_message(400,275,level_var_char,screen,textcolor2,font[2]);
                SDL_Flip(screen);

                while(SDL_PollEvent(&event))
                {
                     if (event.type==SDL_QUIT)                          // for close button on the display window
                     {
                               quit1=1;
                               quit=1;
                     }

                     switch(event.type)
                     {
                          case SDL_KEYDOWN:
                          switch(event.key.keysym.sym)
                          {
                                case '\r':        // for detecting alphabetical enter
                                quit1=1;
                          }
                     }

                     if(keys[SDLK_KP_ENTER])     //detecting numerical enter key to quit
                     {
                          quit1=1;
                     }

                }
             }

             if(sound_var==1)
             {
                  Mix_ResumeMusic();
             }

             rectangle();
             speedobs+=5;
             speed+=2;
}

void load_image()
{
    temp=IMG_Load("images/bg.bmp");                                //load background
    optbg = SDL_DisplayFormat(temp);                               //optimize background
    SDL_FreeSurface(temp);

    temp = IMG_Load("images/obs.bmp");                            // loading obstacle
    oobs = SDL_DisplayFormat(temp);                               // optimising obstacle
    SDL_SetColorKey( oobs, SDL_SRCCOLORKEY, SDL_MapRGB( oobs->format, 0, 0, 0  ) );
    SDL_FreeSurface(temp);

    temp = IMG_Load("images/score_bg.bmp");                       // loading obstacle
    oscore = SDL_DisplayFormat(temp);                             // optimising obstacle
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/suru.bmp");
    osuru=SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/sound_playing.bmp");
    sound_playing=SDL_DisplayFormat(temp);
    SDL_SetColorKey( sound_playing, SDL_SRCCOLORKEY, SDL_MapRGB( sound_playing->format, 255, 255, 255  ) );
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/sound_muted.bmp");
    sound_muted=SDL_DisplayFormat(temp);
    SDL_SetColorKey( sound_muted, SDL_SRCCOLORKEY, SDL_MapRGB( sound_muted->format, 255, 255, 255  ) );
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/bonus_dec.bmp");
    bonus_image[0]=SDL_DisplayFormat(temp);
    SDL_SetColorKey( bonus_image[0], SDL_SRCCOLORKEY, SDL_MapRGB( bonus_image[0]->format, 0, 0, 0  ) );
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/bonus_inc.bmp");
    bonus_image[1]=SDL_DisplayFormat(temp);
    SDL_SetColorKey( bonus_image[1], SDL_SRCCOLORKEY, SDL_MapRGB( bonus_image[1]->format, 0, 0, 0  ) );
    SDL_FreeSurface(temp);

    temp=IMG_Load("images/wall.bmp");
    wall=SDL_DisplayFormat(temp);
    SDL_SetColorKey( wall, SDL_SRCCOLORKEY, SDL_MapRGB( wall->format, 255, 255, 255 ) );
    SDL_FreeSurface(temp);
}

void rectangle()
{
    // for helicopter
    rect.x=50;
    rect.y=rand()%100+30;
    rect.w=99;
    rect.h=40;

    // for first obstacles
    rect1.x=600;
    rect1.y=rand()%160;
    rect1.w=30;
    rect1.h=50;

    // for second obstacle
    rect2.x=720;
    rect2.y=rand()%125+250;
    rect2.w=30;
    rect2.h=50;

    // for third obstacle
    rect3.x=550;
    rect3.y=rand()%85+465;
    rect3.w=30;
    rect3.h=50;

    // for holding wall
    rect4.x=0;
    rect4.y=0;
    rect3.w=655;
    rect3.h=30;

    // for bonus images
    rect_bonus[0].x=800;
    if( (rand()%10) %2 == 0)
       {
          rect_bonus[0].y=rand()%20+425;
       }
    else
          rect_bonus[0].y=rand()%20+210;

    rect_bonus[0].w=20;
    rect_bonus[0].h=20;

    rect_bonus[1].x=1500;
    if( (rand()%10) %2 == 0)
       {
           rect_bonus[1].y=rand()%30+425; ;
       }
    else
           rect_bonus[1].y=rand()%30+200;

    rect_bonus[1].w=20;
    rect_bonus[1].h=20;

}


void replay()
{
             int quit1=0;
             Mix_PauseMusic();
             srand(time(NULL));
             Uint8 *keys;
             SDL_Delay(1000);
             if(sound_var==1)
             {
                  Mix_ResumeMusic();
             }
             life_var--;
             rectangle();
}

int collision(SDL_Rect rect1,SDL_Rect rect2)
{
       if(!((rect1.x+rect1.w)<=rect2.x || rect1.y>=(rect2.y+rect2.h)|| rect1.x>=(rect2.x+rect2.w) || (rect1.y+rect1.h)<=rect2.y))
             return true;
       else
             return false;
}

void variable_init()
{
     quit=0;                                       // variable to terminate main loop
     r=1;                                          // variable to rotate helicopter
     x=0;                                          // for x coordinate of wall
     life_var=3;                                   // for life
     score_var=1;                                  // for saving the score
     speed = 5;                                    // variable for determining the speed of the helicopter
     speedobs = 4;                                 // variable for determining the speed of the obstacle
     level_var =1;                                 // for saving the level
     score_var_temp=0;                             // for storing the numbers of loop completed
     started_ticks=0;                              // for countng time elapsed tillnow
}

/////////         GAME END             //////////
