#include<allegro.h>
#include<alpng.h>
#include<time.h>
#include<vector>

using namespace std;

BITMAP* buffer;
BITMAP* cursor;
BITMAP* slider_red;
BITMAP* slider_green;
BITMAP* slider_blue;
BITMAP* knob;

bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

int new_r;
int new_g;
int new_b;


struct pixel{
  int x;
  int y;
  int r;
  int g;
  int b;
};

vector<pixel> pixels;

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}


//A function to streamline error reporting in file loading
void abort_on_error(const char *message){
	 set_window_title("Error!");
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

void update(){
  if(mouse_b & 1){
    pixel newPixel;
    newPixel.x = mouse_x;
    newPixel.y = mouse_y;
    newPixel.b = new_b;
    newPixel.g = new_g;
    newPixel.r = new_r;

    pixels.push_back(newPixel);
  }
}

void draw(){


    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    for( int i = 0; i <pixels.size(); i++){
        putpixel(buffer,pixels[i].x,pixels[i].y,makecol(pixels[i].r,pixels[i].g,pixels[i].b));
    }


    textprintf_ex(buffer,font,270,12,makecol(0,0,0),-1,"%i",new_r);
    textprintf_ex(buffer,font,270,24,makecol(0,0,0),-1,"%i",new_g);
    textprintf_ex(buffer,font,270,36,makecol(0,0,0),-1,"%i",new_b);

    rectfill(buffer,10,50,265,60,makecol(new_r,new_g,new_b));

    draw_sprite(buffer,slider_red,10,10);
    draw_sprite(buffer,slider_blue,10,22);
    draw_sprite(buffer,slider_green,10,34);

    draw_sprite(buffer,knob,10,10);
    draw_sprite(buffer,knob,10,22);
    draw_sprite(buffer,knob,10,34);

    draw_sprite(buffer,cursor,mouse_x,mouse_y);

    draw_sprite(screen,buffer,0,0);


}







void setup(){
    buffer=create_bitmap(1024,768);


    srand(time(NULL));

     // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

  if (!(cursor = load_bitmap("cursor.png", NULL)))
     abort_on_error("Cannot find image cursor.png\nPlease check your files and try again");
  if (!(slider_red = load_bitmap("slider_red.png", NULL)))
     abort_on_error("Cannot find image slider_red.png\nPlease check your files and try again");
  if (!(slider_blue = load_bitmap("slider_blue.png", NULL)))
     abort_on_error("Cannot find image slider_blue.png\nPlease check your files and try again");
  if (!(slider_green = load_bitmap("slider_green.png", NULL)))
     abort_on_error("Cannot find image slider_green.png\nPlease check your files and try again");
  if (!(knob = load_bitmap("knob.png", NULL)))
     abort_on_error("Cannot find image knob.png\nPlease check your files and try again");
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);


  set_gfx_mode(GFX_AUTODETECT_WINDOWED,1024,768, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Photostore");
  setup();


      while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }


	return 0;
}
END_OF_MAIN()
