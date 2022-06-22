//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene_pause.h"

#include "entity.h"
#include "rect.h"
#include "Lsprite.h"
#include "sprite_manager.h"
#include "text.h"
#include "background.h"
#include "button.h"
#include "inputfield.h"
#include "collider.h"
#include "enemy.h"

#include "editor.h"
#include "database.h"
#include "system_manager.h"
#include "game_manager.h"

ScenePause::ScenePause(){

  change_scene_ = 0;

}

ScenePause::ScenePause(const ScenePause& other){}

ScenePause::~ScenePause(){

  if ( list_obj_ != NULL ){
    for ( int i = 0; i < num_obj_; i++ ){

      
      ((Entity*)(list_obj_[i]))->releaceEditable();

      
      delete (Entity*)(list_obj_[i]);

    }
    
    free(list_obj_);
    list_obj_ = NULL;
    num_obj_ = 0;
  }
  if ( NULL != editor_manager_ ){
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }
}

void ScenePause::init(){

  if ( NULL == editor_manager_ ){
    editor_manager_ = NULL;
    editor_mode_ = false;
  } else{
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }

  SDL_Color color_text_button = { 0, 0, 0, 255 };
  //----------------------------------
  //Button Load
  addObject(new Button());
  ((Button*)getLastObject())->set_position(340, 120);
  ((Button*)getLastObject())->set_scale(780, 115);
  ((Button*)getLastObject())->set_color(100, 100, 100);
  
  ((Button*)getLastObject())->set_text("arial.ttf", "Load", 90, color_text_button);
  //----------------------------------  
   //Button Save
  addObject(new Button());
  ((Button*)getLastObject())->set_position(340, 320);
  ((Button*)getLastObject())->set_scale(780, 115);
  ((Button*)getLastObject())->set_color(100, 100, 100);

  ((Button*)getLastObject())->set_text("arial.ttf", "Save", 90, color_text_button);
  //----------------------------------  
   //Button Quit
  addObject(new Button());
  ((Button*)getLastObject())->set_position(340, 520);
  ((Button*)getLastObject())->set_scale(780, 115);
  ((Button*)getLastObject())->set_color(100, 100, 100);

  ((Button*)getLastObject())->set_text("arial.ttf", "Return Main Menu", 90, color_text_button);
  //----------------------------------  
   //Button Return Game
  addObject(new Button());
  ((Button*)getLastObject())->set_position(340, 720);
  ((Button*)getLastObject())->set_scale(780, 115);
  ((Button*)getLastObject())->set_color(100, 100, 100);

  ((Button*)getLastObject())->set_text("arial.ttf", "Return Game", 90, color_text_button);
  //----------------------------------  


  exit_ = false;
  change_scene_ = 0;
  //----------------------------------  

}

void ScenePause::input(){

  static Uint32 s_delay_time = 0;

  SystemManager *ptr_system_manager = SystemManager::Instance();
  sInput input_event = ptr_system_manager->last_input();



  if ( 0 == editor_mode_ ){

    for ( int i = 0; i < num_obj_; i++ ){

      ((Entity*)(list_obj_[i]))->input(&input_event);

    }

  }

  

    for ( int i = 0; i < num_obj_; i++ ){

      if ( ((Button*)(list_obj_[i]))->isPressed(input_event.mouse_x, input_event.mouse_y) ){

        if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
          if ( 0 == i ){ change_scene_ = 3; }
          if ( 1 == i ){ change_scene_ = 7; }
          if ( 2 == i ){ change_scene_ = 1; GameManager::Instance()->destroyState(); }
          if ( 3 == i ){
            change_scene_ = 4;
            GameManager::Instance()->set_is_new_game(false);
            GameManager::Instance()->set_is_load_game(false);
          }
          s_delay_time = 0;
        }
        ((Button*)(list_obj_[i]))->set_color(200, 100, 100);

      } else{
        ((Button*)(list_obj_[i]))->set_color(100, 100, 100);
      }

    }
    
  

  if ( ptr_system_manager->last_input().keyboard_input == 27 ){
    GameManager::Instance()->set_is_new_game(false);
    GameManager::Instance()->set_is_load_game(false);
    exit_ = true;
  }

  s_delay_time += ptr_system_manager->delta_time;

  if ( s_delay_time > 1000 ){
    s_delay_time = 100;
  }

}





int ScenePause::update(){

  if ( exit_ )return 4;

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->update();

  }

  if ( 0 != change_scene_ ){

    if ( -1 == change_scene_ ){
      exit_ = true;
      return -1;
    }
    return change_scene_;


  }

  return 0;

}

void ScenePause::draw(){

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->draw();

  }

}

void ScenePause::exit(){

  if ( NULL != editor_manager_ ){
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }

  for ( int i = 0; i < num_obj_; i++ ){

    
    ((Entity*)(list_obj_[i]))->releaceEditable();

    
    delete (Entity*)list_obj_[i];

  }

  
  free(list_obj_);

  list_obj_ = NULL;
  num_obj_ = 0;

}