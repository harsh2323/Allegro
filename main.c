#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

const float FPS = 60;
const float SCREEN_W = 800;
const float SCREEN_H = 600;
const float image_size = 32;
const float char_size = 4.0;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

int main(int argc, char **argv){
    ALLEGRO_BITMAP *image;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *start;

    int charMax = 5;
    ALLEGRO_BITMAP *character[charMax];
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    float image_x = 100;
    float image_y = 430;
    bool key[5] = { false, false, false, false, false };
    bool redraw = true;
    int maxFrame = 15;
    ALLEGRO_BITMAP *jump[maxFrame];
    int currFrame = 0;
    int frameCount = 0;
    int frameDelay = 10;
    bool doexit = false;
    bool gravity = true;
    bool jumping = false;
    bool charStop = false;
    const float gravityForce = 10.0;
    const float jumpSpeed = 10.0;

    al_init();
    if(!al_init()){
        fprintf(stderr, "Failed to initialize allegro");
        return -1;
    }

    if(!al_install_keyboard()){
        fprintf(stderr, "Failed to inititialize keyboard");
        return -1;
    }

    timer = al_create_timer( 1.0 / FPS );
    if(!timer){
        fprintf(stderr, "Failed to initialize timer");
        return -1;
    }

    al_init_image_addon();
    if(!al_init_image_addon()){
        fprintf(stderr, "Failed to image addon");
        return -1;
    }

    //LOAD JUMPING BITMAPS
    jump[0] = al_load_bitmap("jump/1.png");
    jump[1] = al_load_bitmap("jump/2.png");
    jump[2] = al_load_bitmap("jump/3.png");
    jump[3] = al_load_bitmap("jump/4.png");
    jump[4] = al_load_bitmap("jump/5.png");
    jump[5] = al_load_bitmap("jump/6.png");
    jump[6] = al_load_bitmap("jump/7.png");
    jump[7] = al_load_bitmap("jump/8.png");
    jump[8] = al_load_bitmap("jump/9.png");
    jump[9] = al_load_bitmap("jump/10.png");
    jump[10] = al_load_bitmap("jump/11.png");
    jump[11] = al_load_bitmap("jump/12.png");
    jump[12] = al_load_bitmap("jump/13.png");
    jump[13] = al_load_bitmap("jump/14.png");
    jump[14] = al_load_bitmap("jump/15.png");


    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display){
        fprintf(stderr, "failed to create display");
        return -1;
    }

    image = al_load_bitmap("newbg.jpeg");
    if(!image){
        fprintf(stderr, "Failed to load image");
        return -1;
    }

    character[0] = al_load_bitmap("idle/1.png");
    character[1] = al_load_bitmap("idle/2.png");
    character[2] = al_load_bitmap("idle/3.png");
    character[3] = al_load_bitmap("idle/4.png");
    character[4] = al_load_bitmap("idle/5.png");

    if(!character[charMax]){
        fprintf(stderr, "Failed to load character");
        return -1;
    }
    
    start = al_load_bitmap("idle/1.png");
    al_set_target_bitmap(start);
    al_set_target_bitmap(al_get_backbuffer(display));

    al_draw_bitmap(image, 0,0,0);
    al_draw_bitmap(start, 100, 430, 0);

    event_queue = al_create_event_queue();
    if(!event_queue){
        fprintf(stderr, "Failed to initialize event queue");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_flip_display();
   
    al_start_timer(timer);

    while(!doexit){
    
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);


        if(jumping == true && image_y != 430){
            do{
                image_y += 4.0 * jumpSpeed;
            }while(image_y != 430);
        }

        if(ev.type == ALLEGRO_EVENT_TIMER) {

            if(++frameCount >= frameDelay){
                if(++currFrame >= charMax){
                    currFrame = 0;
                }
                frameCount = 0;
                }

            if(key[KEY_SPACE] && gravity == true){
                jumping = true;

                image_y -= 4.0 * jumpSpeed;
                if(++frameCount >= frameDelay){
                    if(++currFrame >= maxFrame){
                        currFrame = 0;
                    }
                    frameCount = 0;
                }
                
            }

            if(key[KEY_LEFT] && image_x >= 4.0){
                image_x -= 4.0;
            }

            if(key[KEY_RIGHT] && image_x <= SCREEN_W - image_size - char_size ){
                image_x += 4.0;
            }

            redraw = true;
        }


        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = true;
                    break;
            }
        }

        else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    key[KEY_LEFT] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;

            }
        }

        if(redraw && al_is_event_queue_empty(event_queue)){
            redraw = false;
            al_draw_bitmap(image, 0, 0, 0);
            al_draw_bitmap(character[currFrame], 0, 0, 0);
            al_draw_bitmap(jump[currFrame], image_x, image_y, 0);
            al_flip_display();

        }

    }
    for(int i = 0; i < maxFrame; i++){
        al_destroy_bitmap(jump[i]);
    }
    for(int i = 0; i < charMax; i++){
        al_destroy_bitmap(character[i]);
    }
    al_destroy_bitmap(image);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);


    return 0;

}