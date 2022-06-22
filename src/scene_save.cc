//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene_save.h"

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

SceneSave::SceneSave(){

  change_scene_ = 0;
  total_num_savegame_ = 0;
  list_savegame_ = NULL;
}

SceneSave::SceneSave(const SceneSave& other){}

SceneSave::~SceneSave(){

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

void SceneSave::init(){

  if ( NULL == editor_manager_ ){
    editor_manager_ = NULL;
    editor_mode_ = false;
  } else{
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }

  //Load sprite in spriteManager

  SpriteManager::Instance().addSprite("cruz.png");


  //----------------------------------  

  DataBase &ptr_database = DataBase::Instance();

  //Get the num of Saved games
  total_num_savegame_ = ptr_database.getNumSaveGame(ptr_database.current_user_id_);

  //Get the IDs in the database of the Saved games
  list_savegame_ = ptr_database.getListSaveGame(ptr_database.current_user_id_);

  SDL_Color color_text_button = { 0, 0, 0, 255 };

  for ( int i = 0; i < 3; i++ ){

    //Create a button to load a game, delete a game or create a game
    if ( i < total_num_savegame_ ){

      //Button load
      addObject(new Button(std::to_string(i + 1).c_str()));
      ((Entity*)getLastObject())->set_position(300, 120 + (i * 130));
      ((Entity*)getLastObject())->set_scale(1000, 115);
      ((Entity*)getLastObject())->set_color(100, 100, 100);

      ((Button*)getLastObject())->set_text("arial.ttf", ptr_database.getSaveGameName(list_savegame_[i]), 90, color_text_button);
      //----------------------------------------------------
      //Button delete
      addObject(new Button(std::to_string(i + 1).c_str()));
      ((Entity*)getLastObject())->set_position(1350, 120 + (i * 130));
      ((Entity*)getLastObject())->set_scale(115, 115);
      ((Entity*)getLastObject())->set_color(100, 100, 100);

      ((Button*)getLastObject())->loadImg("cruz");
    } else{

      addObject(new InputField(("New" + std::to_string(i + 1)).c_str()));
      ((Entity*)getLastObject())->set_position(300, 120 + (i * 130));
      ((Entity*)getLastObject())->set_scale(1000, 115);
      ((Entity*)getLastObject())->set_color(100, 100, 100);

      ((InputField*)getLastObject())->set_text("arial.ttf", "New Save", 90, color_text_button);
      ((InputField*)getLastObject())->changeDefaultText("New Save");
      ((InputField*)getLastObject())->changeMaxInput(15);
    }
  }

  addObject(new Button("Save"));
  ((Entity*)getLastObject())->set_position(300, 610);
  ((Entity*)getLastObject())->set_scale(1000, 115);
  ((Entity*)getLastObject())->set_color(100, 100, 100);

  ((Button*)getLastObject())->set_text("arial.ttf", "Save New", 90, color_text_button);
  //-------------------------


  //---

  exit_ = false;
  change_scene_ = 0;

}

void SceneSave::input(){

  static Uint32 s_delay_time = 0;

  SystemManager *ptr_system_manager = SystemManager::Instance();
  sInput input_event = ptr_system_manager->last_input();



  if ( 0 == editor_mode_ ){

    for ( int i = 0; i < num_obj_; i++ ){

      ((Entity*)(list_obj_[i]))->input(&input_event);

    }

  }

  if ( input_event.mouse_wheel_input != 0 ){

    if ( ((Rect*)(list_obj_[0]))->rect_.y + input_event.mouse_wheel_input > 0 && ((Rect*)(list_obj_[0]))->rect_.y + input_event.mouse_wheel_input < SystemManager::Instance()->kWindowH
      && ((Rect*)(list_obj_[num_obj_ - 1]))->rect_.y + input_event.mouse_wheel_input > 0 && ((Rect*)(list_obj_[num_obj_ - 1]))->rect_.y + input_event.mouse_wheel_input < SystemManager::Instance()->kWindowH ){

      for ( int i = 0; i < num_obj_; i++ ){

        ((Rect*)(list_obj_[i]))->rect_.y += input_event.mouse_wheel_input;

      }

    }
  }

  

    for ( int i = 0; i < num_obj_; i++ ){

      if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_InputField ){
        if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
          ((InputField*)(list_obj_[i]))->isPressed(input_event.mouse_x, input_event.mouse_y);
        }
      }

      if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_button ){

        if ( ((Button*)(list_obj_[i]))->isPressed(input_event.mouse_x, input_event.mouse_y) ){
          if ( input_event.mouse_key_input == -1 && s_delay_time > 100 ){
            if ( std::string(((Button*)(list_obj_[i]))->my_name_).length() > 1 ){

              const char *field = NULL;
              int slot = 0;
              for ( int j = 0; j < num_obj_ && field == NULL; j++ ){
                if ( ((Entity*)(list_obj_[j]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_InputField ){

                  field = ((InputField*)(list_obj_[j]))->input();
                  if ( strlen(field) == 0 ){ field = NULL; }
                  slot = std::stoi(&((Entity*)(list_obj_[j]))->my_name_[3]);


                }
              }
              if ( slot != 0 && field != NULL ){
                
                GameManager::Instance()->set_current_save_slot((unsigned)slot);
                GameManager::Instance()->set_save_name(field);
                GameManager::Instance()->set_is_new_game(false);
                GameManager::Instance()->set_is_load_game(false);
                GameManager::Instance()->saveGame((unsigned)slot);
                change_scene_ = 6;
              }

            } else{

              if ( ((Button*)(list_obj_[i]))->rect_.x > 800 ){
                char* nameSave = DataBase::Instance().getSaveGameName(list_savegame_[std::stoi(((Button*)(list_obj_[i]))->my_name_) - 1]);
                DataBase::Instance().deleteSaveGame(nameSave);


                free(nameSave);
                change_scene_ = 7;

              } else{

                int slot = std::stoi(((Button*)(list_obj_[i]))->my_name_);

                DataBase::Instance().current_savegame_id_ = list_savegame_[slot - 1];
                char* field = DataBase::Instance().getSaveGameName(list_savegame_[slot - 1]);

                
                GameManager::Instance()->set_current_save_slot((unsigned)slot);
                GameManager::Instance()->set_save_name(field);
                GameManager::Instance()->set_is_new_game(false);
                GameManager::Instance()->set_is_load_game(false);
                GameManager::Instance()->saveGame((unsigned)slot);


                free(field);

                change_scene_ = 6;
              }



            }
          }
        
          ((Button*)(list_obj_[i]))->set_color(200, 100, 100);
        } else{
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





int SceneSave::update(){

  if ( exit_ )return 6;

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->update();

  }

  if ( 0 != change_scene_ ){

    return change_scene_;

  }

  return 0;

}

void SceneSave::draw(){

  for ( int i = 0; i < num_obj_; i++ ){

    ((Entity*)(list_obj_[i]))->draw();

  }

}

void SceneSave::exit(){

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

  if ( list_savegame_ != NULL ){
   
    free(list_savegame_);
    list_savegame_ = NULL;
  }
}