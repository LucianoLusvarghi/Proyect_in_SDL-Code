// Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene_register.h"

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

SceneRegister::SceneRegister(){

  change_scene_ = 0; 
  exit_ = false;
}

SceneRegister::SceneRegister(const SceneRegister& other){}

SceneRegister::~SceneRegister(){

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

void SceneRegister::init(){

  if ( NULL == editor_manager_ ){
    editor_manager_ = NULL;
    editor_mode_ = false;
  } else{
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }


  //----------------------------------  
  //Text "User: "
  addObject(new Rect());
  ((Entity*)getLastObject())->set_position(100, 120);
  ((Entity*)getLastObject())->set_scale(500, 115);
  ((Entity*)getLastObject())->set_color(100, 100, 100);

  addObject(new Text("arial.ttf", 90, "UserName: "));
  ((Text*)getLastObject())->set_position(110, 140);
  ((Text*)getLastObject())->set_color(0, 0, 0, 255);
  //----------------------------------
  //Text "Password: "
  addObject(new Rect());
  ((Entity*)getLastObject())->set_position(100, 320);
  ((Entity*)getLastObject())->set_scale(500, 115);
  ((Entity*)getLastObject())->set_color(100, 100, 100);

  addObject(new Text("arial.ttf", 90, "Password: "));
  ((Text*)getLastObject())->set_position(110, 340);
  ((Text*)getLastObject())->set_color(0, 0, 0, 255);
  //----------------------------------
  //Button "Login"
  addObject(new Button("Register"));
  ((Entity*)getLastObject())->set_position(100, 520);
  ((Entity*)getLastObject())->set_scale(500, 115);
  ((Entity*)getLastObject())->set_color(100, 100, 100);

  SDL_Color color = { 0, 0, 0, 255 };
  ((Button*)getLastObject())->set_text("arial.ttf", "Register", 90, color);

  //----------------------------------     
  //Input "User"
  addObject(new InputField("User"));
  ((InputField*)getLastObject())->set_position(700, 120);
  ((InputField*)getLastObject())->set_scale(650, 115);
  ((InputField*)getLastObject())->set_color(100, 100, 100);

  ((InputField*)getLastObject())->set_text("arial.ttf", "Enter Text", 90, color);
  ((InputField*)getLastObject())->changeMaxInput(10);
  //---------------------------------- 
  //Input "Pasword"
  addObject(new InputField("Password"));
  ((InputField*)getLastObject())->set_position(700, 320);
  ((InputField*)getLastObject())->set_scale(650, 115);
  ((InputField*)getLastObject())->set_color(100, 100, 100);

  ((InputField*)getLastObject())->set_text("arial.ttf", "Enter Text", 90, color);
  ((InputField*)getLastObject())->changeMaxInput(10);
  //----------------------------------  
  addObject(new Text("arial.ttf", 30, " ", color));
  ((Text*)getLastObject())->set_position(700, 520);
  ((Text*)getLastObject())->set_scale(400, 115);
  ((Text*)getLastObject())->set_color(0, 0, 0);  
  //----------------------------------


  exit_ = false;
  change_scene_ = 0;

}

void SceneRegister::input(){

  static Uint32 s_delay_time = 0;

  SystemManager *ptr_system_manager = SystemManager::Instance();
  sInput input_event = ptr_system_manager->last_input();



  if ( 0 == editor_mode_ ){

    for ( int i = 0; i < num_obj_; i++ ){

      ((Entity*)(list_obj_[i]))->input(&input_event);

    }

  }

  

    for ( int i = 0; i < num_obj_; i++ ){

      //Select Input Field 
      if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_InputField ){
        if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
          ((InputField*)(list_obj_[i]))->isPressed(input_event.mouse_x, input_event.mouse_y);
        }
      }

      //Button Interact
      if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_button ){

        if ( ((Button*)(list_obj_[i]))->isPressed(input_event.mouse_x, input_event.mouse_y) ){
          if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
            change_scene_ = 4;
          }
          ((Button*)(list_obj_[i]))->set_color(200, 100, 100);
        } else{
          change_scene_ = 0;
          ((Button*)(list_obj_[i]))->set_color(100, 100, 100);
        }

      }
    }

  

  if ( ptr_system_manager->last_input().keyboard_input == 27 ){
    exit_ = true;
  }

  s_delay_time += ptr_system_manager->delta_time;

  if ( s_delay_time > 1000 ){
    s_delay_time = 0;
  }

}





int SceneRegister::update(){

  if ( exit_ )return 1;

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->update();

  }

  if ( 0 != change_scene_ ){

    if ( 4 == change_scene_ ){

      const char* user;
      const char* password;

      for ( int i = 0; i < num_obj_; i++ ){
        if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_InputField ){

          if ( std::string(((Entity*)(list_obj_[i]))->my_name_).compare("User") == 0 ){
            user = ((InputField*)(list_obj_[i]))->input();
          } else if ( std::string(((Entity*)(list_obj_[i]))->my_name_).compare("Password") == 0 ){
            password = ((InputField*)(list_obj_[i]))->input();
          }

        }
      }

      if ( strlen(user) < 3 ){
        ((Text*)(list_obj_[7]))->set_text("UserName too short, minimum 3 characters");
                 
        change_scene_ = 0;
      } else if ( strlen(password) < 5 ){       
        ((Text*)(list_obj_[7]))->set_text("Password too short, minimum 5 characters");
         
        change_scene_ = 0;
      } else{

        int checkIfExist = DataBase::Instance().setNewUser(user, password);
        if ( checkIfExist != -1 ){
          exit_ = true;
          return 3;
        } else{
          ((Text*)(list_obj_[7]))->set_text("UserName already exists");
          
          change_scene_ = 0;
        }

      }
    }

  }

  return 0;

}

void SceneRegister::draw(){

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->draw();

  }

}

void SceneRegister::exit(){

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

