//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/system_manager.h"


static SystemManager* s_insts = nullptr;

SystemManager* SystemManager::Instance(){
    
  if ( nullptr == s_insts ){ s_insts = new SystemManager();  }
  
  return s_insts;
  
}

SystemManager::SystemManager(){
  
  current_time = 0;
  last_time = 0;
  delta_time = 0;
  
  windows_name_ = NULL;
  last_input_ = {0,0,0,0,0};
  is_majuscule_ = false;
}

SystemManager::~SystemManager(){}

SystemManager::SystemManager(const SystemManager& other){}

void SystemManager::set_windows_name(const char *name){
      
  windows_name_ = (char*)calloc( strlen(name)+1, sizeof(char) );
  memcpy(windows_name_, name, strlen(name));
  
}

const char* SystemManager::windows_name(){  
  return windows_name_;
}

bool SystemManager::initSDL(){
  
  if (0 != SDL_Init(SDL_INIT_VIDEO) ){
    printf("ERROR AL INICIAR SDL\nSDL ERROR: %s\n", SDL_GetError());
       
    return false;
  }
  
  if(NULL == windows_name_ ){
    
    printf("No has indicado nombre de ventana - ERROR\n");
    return false;
    
  }
  
  if(NULL == (Main_window_ = SDL_CreateWindow(windows_name_, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                              kWindowW, kWindowH, 0) )){
    printf("ERROR AL CREAR WINDOW\nSDL ERROR: %s\n", SDL_GetError());    
    return false;
  }
  
  if(NULL == ( Main_render_ = SDL_CreateRenderer(Main_window_, -1, SDL_RENDERER_ACCELERATED ) )){
    printf("ERROR AL CREAR RENDER\nSDL ERROR: %s\n", SDL_GetError());    
    return false;    
  }
  
  if( !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){    
    printf("Error al iniciar imagen \n Error: %s\n", IMG_GetError());
    return false;
  }
  
  if (0 > TTF_Init()){    
    printf("ERROR AL INICIAR TEXT\nSDL-TTF ERROR: %s\n", SDL_GetError());    
    return false;      
  }
  
  return true;  
}

void SystemManager::exitSDL(){
  
  TTF_Quit();
  IMG_Quit();
  SDL_Quit(); 
  
  SDL_DestroyRenderer( Main_render_ );
  
  SDL_DestroyWindow( Main_window_ );  
  
  
  free(windows_name_);
  


}

void SystemManager::UpdateSistemInput(){

  SDL_Event input_event;

  sInput current_input = { 0,0,0,0,0 };

  int mouse_x;
  int mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  current_input.mouse_x = static_cast<short>(mouse_x);
  current_input.mouse_y = static_cast<short>(mouse_y);

  while ( SDL_PollEvent(&input_event) ){

    if ( input_event.type == SDL_QUIT ){

      current_input.keyboard_input = 27;
      current_input.mouse_key_input = 0;
      current_input.mouse_wheel_input = 0;
      return;
    }

    if ( input_event.type == SDL_KEYDOWN ){
      if ( SDLK_LSHIFT == input_event.key.keysym.sym || SDLK_RSHIFT == input_event.key.keysym.sym ){

        is_majuscule_ = true;
      }
    } else if ( input_event.type == SDL_KEYUP ){
      if ( SDLK_LSHIFT == input_event.key.keysym.sym || SDLK_RSHIFT == input_event.key.keysym.sym ){

        is_majuscule_ = false;
      }
    }

    if ( input_event.type == SDL_KEYDOWN ){

      switch ( input_event.key.keysym.sym ){

      case SDLK_0:{
        current_input.keyboard_input = '0';
        break;
      }
      case SDLK_1:{
        current_input.keyboard_input = '1';
        break;
      }
      case SDLK_2:{
        current_input.keyboard_input = '2';
        break;
      }
      case SDLK_3:{
        current_input.keyboard_input = '3';
        break;
      }
      case SDLK_4:{
        current_input.keyboard_input = '4';
        break;
      }
      case SDLK_5:{
        current_input.keyboard_input = '5';
        break;
      }
      case SDLK_6:{
        current_input.keyboard_input = '6';
        break;
      }
      case SDLK_7:{
        current_input.keyboard_input = '7';
        break;
      }
      case SDLK_8:{
        current_input.keyboard_input = '8';
        break;
      }
      case SDLK_9:{
        current_input.keyboard_input = '9';
        break;
      }

                  //---------------------------------------------------------------------------

      case SDLK_KP_0:{
        current_input.keyboard_input = '0';
        break;
      }
      case SDLK_KP_1:{
        current_input.keyboard_input = '1';
        break;
      }
      case SDLK_KP_2:{
        current_input.keyboard_input = '2';
        break;
      }
      case SDLK_KP_3:{
        current_input.keyboard_input = '3';
        break;
      }
      case SDLK_KP_4:{
        current_input.keyboard_input = '4';
        break;
      }
      case SDLK_KP_5:{
        current_input.keyboard_input = '5';
        break;
      }
      case SDLK_KP_6:{
        current_input.keyboard_input = '6';
        break;
      }
      case SDLK_KP_7:{
        current_input.keyboard_input = '7';
        break;
      }
      case SDLK_KP_8:{
        current_input.keyboard_input = '8';
        break;
      }
      case SDLK_KP_9:{
        current_input.keyboard_input = '9';
        break;
      }

                     //-------------------------------------------------------------------
      case SDLK_SPACE:{
        current_input.keyboard_input = ' ';
        break;
      }

      case SDLK_a:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'A';
        } else{
          current_input.keyboard_input = 'a';
        }
        break;
      }
      case SDLK_b:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'B';
        } else{
          current_input.keyboard_input = 'b';
        }
        break;
      }
      case SDLK_c:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'C';
        } else{
          current_input.keyboard_input = 'c';
        }
        break;
      }
      case SDLK_d:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'D';
        } else{
          current_input.keyboard_input = 'd';
        }
        break;
      }
      case SDLK_e:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'E';
        } else{
          current_input.keyboard_input = 'e';
        }
        break;
      }
      case SDLK_f:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'F';
        } else{
          current_input.keyboard_input = 'f';
        }
        break;
      }
      case SDLK_g:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'G';
        } else{
          current_input.keyboard_input = 'g';
        }
        break;
      }
      case SDLK_h:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'H';
        } else{
          current_input.keyboard_input = 'h';
        }
        break;
      }
      case SDLK_i:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'I';
        } else{
          current_input.keyboard_input = 'i';
        }
        break;
      }
      case SDLK_j:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'J';
        } else{
          current_input.keyboard_input = 'j';
        }
        break;
      }
      case SDLK_k:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'K';
        } else{
          current_input.keyboard_input = 'k';
        }
        break;
      }
      case SDLK_l:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'L';
        } else{
          current_input.keyboard_input = 'l';
        }
        break;
      }
      case SDLK_m:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'M';
        } else{
          current_input.keyboard_input = 'm';
        }
        break;
      }
      case SDLK_n:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'N';
        } else{
          current_input.keyboard_input = 'n';
        }
        break;
      }
      case SDLK_o:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'O';
        } else{
          current_input.keyboard_input = 'o';
        }
        break;
      }
      case SDLK_p:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'P';
        } else{
          current_input.keyboard_input = 'p';
        }
        break;
      }
      case SDLK_q:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'Q';
        } else{
          current_input.keyboard_input = 'q';
        }
        break;
      }
      case SDLK_r:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'R';
        } else{
          current_input.keyboard_input = 'r';
        }
        break;
      }
      case SDLK_s:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'S';
        } else{
          current_input.keyboard_input = 's';
        }
        break;
      }
      case SDLK_t:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'T';
        } else{
          current_input.keyboard_input = 't';
        }
        break;
      }
      case SDLK_u:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'U';
        } else{
          current_input.keyboard_input = 'u';
        }
        break;
      }
      case SDLK_v:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'V';
        } else{
          current_input.keyboard_input = 'v';
        }
        break;
      }
      case SDLK_w:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'W';
        } else{
          current_input.keyboard_input = 'w';
        }
        break;
      }
      case SDLK_x:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'X';
        } else{
          current_input.keyboard_input = 'x';
        }
        break;
      }
      case SDLK_y:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'Y';
        } else{
          current_input.keyboard_input = 'y';
        }
        break;
      }
      case SDLK_z:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = 'Z';
        } else{
          current_input.keyboard_input = 'z';
        }
        break;
      }

                  //---------------------------------------------------------------------------------

      case SDLK_MINUS:{

        if ( is_majuscule_ ){
          current_input.keyboard_input = '_';
        } else{
          current_input.keyboard_input = '-';
        }
        break;
      }
      case SDLK_PERIOD:{
        if ( is_majuscule_ ){
          current_input.keyboard_input = ':';
        } else{
          current_input.keyboard_input = '.';
        }

        break;
      }

                       //----------------------------------------------------------------------------------

      case SDLK_KP_MULTIPLY:{

        current_input.keyboard_input = '*';

        break;
      }

      case SDLK_ASTERISK:{

        current_input.keyboard_input = '*';

        break;
      }

      case SDLK_BACKSPACE:{

        current_input.keyboard_input = 8;

        break;
      }

      case SDLK_DELETE:{
        current_input.keyboard_input = 127;

        break;
      }

      case SDLK_RETURN:{
        current_input.keyboard_input = 10;

        break;
      }

      case SDLK_KP_ENTER:{
        current_input.keyboard_input = 10;

        break;
      }
      case SDLK_TAB:{

        current_input.keyboard_input = 9;

        break;
      }
      case SDLK_ESCAPE:{

        current_input.keyboard_input = 27;

        break;
      }

      }
      
    }

    
    


    if ( input_event.type == SDL_MOUSEBUTTONDOWN && input_event.button.button == SDL_BUTTON_LEFT ){

      current_input.mouse_key_input = -1;      

    }

    if ( input_event.type == SDL_MOUSEWHEEL ){

      
      current_input.mouse_wheel_input = input_event.wheel.y;

    }

    

  }
  
  last_input_ = current_input;

}

sInput SystemManager::last_input(){
  
  return last_input_;
  
}

void SystemManager::clearInput(){  
  last_input_ = {0,0,0,0,0};  
}

