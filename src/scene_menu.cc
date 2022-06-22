//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene_menu.h"

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

SceneMenu::SceneMenu(){
  
  change_scene_ = 0;
  
}

SceneMenu::SceneMenu(const SceneMenu& other){}

SceneMenu::~SceneMenu(){
  
  if(list_obj_ != NULL){
    for(int i=0;i<num_obj_;i++){
      
      
      ((Entity*)(list_obj_[i]))->releaceEditable();

      
      delete (Entity*)(list_obj_[i]); 
      
    }
    
    free (list_obj_);
    list_obj_ = NULL;
    num_obj_ = 0;
  }
  if(NULL != editor_manager_){
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }
}

void SceneMenu::init(){
  
  if(NULL == editor_manager_){
    editor_manager_ = NULL;
    editor_mode_ = false;
  }else{
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }
    
  SDL_Color color_text_button = { 0, 0, 0, 255 };

  //
  addObject(new Button());
  static_cast<Button*>(getLastObject())->set_position(340, 120);
  static_cast<Button*>(getLastObject())->set_scale(680, 115);
  static_cast<Button*>(getLastObject())->set_color(100, 100, 100);
  static_cast<Button*>(getLastObject())->set_text("arial.ttf", "LOGIN", 90, color_text_button);
  //----------------------------------  
  addObject(new Button());
  static_cast<Button*>(getLastObject())->set_position(340, 320);
  static_cast<Button*>(getLastObject())->set_scale(680, 115);
  static_cast<Button*>(getLastObject())->set_color(100, 100, 100);
  static_cast<Button*>(getLastObject())->set_text("arial.ttf", "SIGN IN", 90, color_text_button);
  //----------------------------------  
  addObject(new Button());
  static_cast<Button*>(getLastObject())->set_position(340, 520);
  static_cast<Button*>(getLastObject())->set_scale(680, 115);
  static_cast<Button*>(getLastObject())->set_color(100, 100, 100);
  static_cast<Button*>(getLastObject())->set_text("arial.ttf", "EXIT", 90, color_text_button);
  //----------------------------------  

  //----------------------------------  
  change_scene_ = 0;
  //----------------------------------  
   
}

void SceneMenu::input(){
  
  static Uint32 s_delay_time = 0;
  
  SystemManager *ptr_system_manager = SystemManager::Instance();   
  sInput input_event = ptr_system_manager->last_input();
  
 
      
  if( 0 == editor_mode_){
      
    for(int i=0;i<num_obj_;i++){
      
      ((Entity*)(list_obj_[i]))->input(&input_event);
          
    }
    
  }
        
  for ( int i = 0; i < num_obj_; i++ ){
    
    if ( static_cast<Entity*>(list_obj_[i])->engine_tag_ == EngineObjectTag::kEngineObjectTag_button ){

      if ( static_cast<Button*>(list_obj_[i])->isPressed(input_event.mouse_x, input_event.mouse_y) ){

        if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
          s_delay_time = 0;
          
          if ( 0 == i ){ change_scene_ = 2; }
          if ( 1 == i ){ change_scene_ = 5; }
          if ( 2 == i ){ change_scene_ = -1; }
          
        }

        static_cast<Button*>(list_obj_[i])->set_color(200, 100, 100);


      } else{
        static_cast<Button*>(list_obj_[i])->set_color(100, 100, 100);
      }



    }

  }
    
  s_delay_time += ptr_system_manager->delta_time;
  
  if(s_delay_time > 1000){
    s_delay_time = 100;
  }
    
}
    

  


int SceneMenu::update(){
    
  for(int i=0;i<num_obj_;i++){
    
    ((Entity*)(list_obj_[i]))->update();
    
  }   
   
  if( 0 != change_scene_  ){
    
    if(-1 == change_scene_){
      exit_ = true;
      return -1;
    }
    return change_scene_;
    
    
  }
   
  return 0;
 
}

void SceneMenu::draw(){
  
  for(int i=0;i<num_obj_;i++){
    
    ((Entity*)(list_obj_[i]))->draw();
    
  }
  
}

void SceneMenu::exit(){
  
  if(NULL != editor_manager_){
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }
  
  for(int i=0; i<num_obj_;i++){
    ((Entity*)(list_obj_[i]))->releaceEditable();
    delete (Entity*)list_obj_[i];
  }
  
 
  free(list_obj_);
  
  list_obj_ = NULL;
  num_obj_ = 0;
  
}