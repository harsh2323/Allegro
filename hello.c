#include <stdio.h>
#include <allegro5/allegro.h>
#include<allegro5/allegro_image.h>  

const float FPS = 60;
const float SCREEN_H = 640;
const float SCREEN_W = 480;
const float bouncer_size = 32;

int main(int argc, char **argv){

   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *bouncer = NULL;
   //ALLEGRO_BITMAP *image = NULL;

   float bouncer_x = SCREEN_W / 2.0 - bouncer_size / 2.0;
   float bouncer_y = SCREEN_H / 2.0 - bouncer_size / 2.0;
   float bouncer_dx = 4.0;
   float bouncer_dy = 4.0;
   bool redraw = true;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   timer = al_create_timer( 1.0 / FPS );
   if(!timer){
      fprintf(stderr, "failed to create a timer\n" );
      return -1;
   }

   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }

   bouncer = al_create_bitmap(bouncer_size, bouncer_size);
   if(!bouncer){
      fprintf(stderr, "Failed to create bitmap");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

   al_set_target_bitmap(bouncer);
   al_clear_to_color(al_map_rgb(255, 0, 255));
   al_set_target_bitmap(al_get_backbuffer(display));


   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      return -1;
   }
 
   al_register_event_source(event_queue, al_get_display_event_source(display));

   al_register_event_source(event_queue, al_get_timer_event_source(timer));

   al_clear_to_color(al_map_rgb(0,0,0));
   
   al_start_timer(timer);

   al_flip_display();
 
   while(true)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      ALLEGRO_TIMEOUT timeout;
      al_init_timeout(&timeout, 0.06);
      
      bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

      if(ev.type == ALLEGRO_EVENT_TIMER){

         if(bouncer_x < 0 || bouncer_x > SCREEN_W - bouncer_size){
            bouncer_dx = -bouncer_dx;
         }

         if(bouncer_y < 0 || bouncer_y > SCREEN_H - bouncer_size){
            bouncer_dy = -bouncer_dy;
         }

         bouncer_x += bouncer_dx;
         bouncer_y += bouncer_dy;

         redraw = true;
      }

      if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

      if(redraw && al_is_event_queue_empty(event_queue)){
         redraw = false;
         al_clear_to_color(al_map_rgb(0, 0, 0));
         al_draw_bitmap(bouncer, bouncer_x, bouncer_y ,0);
         al_flip_display();
      }

      al_clear_to_color(al_map_rgb(0,0,0));
      al_flip_display();
   }
   al_destroy_bitmap(bouncer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_timer(timer);


   return 0;
}