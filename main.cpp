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
bool display_hud=true;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int step;

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

//Area clicked
bool location_clicked(int min_x,int max_x,int min_y,int max_y){
    if(mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && mouse_b & 1 || mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && joy[0].button[1].b)
        return true;
    else return false;
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
void fill_screen(){
  pixels.clear();
  for( int i = 0; i <SCREEN_W; i++){
    for( int j = 0; j <SCREEN_H; j++){
    pixel newPixel;
    newPixel.x = i;
    newPixel.y = j;
    newPixel.b = new_b;
    newPixel.g = new_g;
    newPixel.r = new_r;

    pixels.push_back(newPixel);
    }
  }



}
void update(){

  if(key[KEY_F])fill_screen();
  if(key[KEY_F11]){
    if(display_hud && step>9){
      display_hud=false;
      step=0;
    }
    if(!display_hud && step>9){
      display_hud=true;
      step=0;
    }
  }

  if(mouse_b & 1 && (!location_clicked(0,300,0,70) || !display_hud)){
   for( int i = 0; i <pixels.size(); i++){
      if(pixels[i].x == mouse_x && pixels[i].y == mouse_y)pixels.erase(pixels.begin()+i);
    }
    pixel newPixel;
    newPixel.x = mouse_x;
    newPixel.y = mouse_y;
    newPixel.b = new_b;
    newPixel.g = new_g;
    newPixel.r = new_r;

    pixels.push_back(newPixel);
  }
  if(location_clicked(0,280,0,25)){
    new_r=mouse_x-10;
    if(new_r>255)new_r=255;
    if(new_r<0)new_r=0;
  }
  if(location_clicked(0,280,26,45)){
    new_g=mouse_x-10;
    if(new_g>255)new_g=255;
    if(new_g<0)new_g=0;
  }
  if(location_clicked(0,280,46,70)){
    new_b=mouse_x-10;
    if(new_b>255)new_b=255;
    if(new_b<0)new_b=0;
  }
  step++;
}

void draw(){


    rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(255,255,255));
    for( int i = 0; i <pixels.size(); i++){
       putpixel(buffer,pixels[i].x,pixels[i].y,makecol(pixels[i].r,pixels[i].g,pixels[i].b));
    }

    if(display_hud){
      textprintf_ex(buffer,font,280,12,makecol(0,0,0),-1,"%i",new_r);
      textprintf_ex(buffer,font,280,32,makecol(0,0,0),-1,"%i",new_g);
      textprintf_ex(buffer,font,280,52,makecol(0,0,0),-1,"%i",new_b);

      textprintf_ex(buffer,font,350,52,makecol(0,0,0),-1,"%i",pixels.size());

      rectfill(buffer,10,70,275,80,makecol(new_r,new_g,new_b));
      rect(buffer,10,70,275,80,makecol(255-new_b,255-new_r,255-new_g));

      draw_sprite(buffer,slider_red,10,10);
      draw_sprite(buffer,slider_green,10,30);
      draw_sprite(buffer,slider_blue,10,50);

      draw_sprite(buffer,knob,10+new_r,10);
      draw_sprite(buffer,knob,10+new_g,30);
      draw_sprite(buffer,knob,10+new_b,50);
    }

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
