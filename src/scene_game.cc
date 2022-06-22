//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene_game.h"

#include <array>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "entity.h"
#include "rect.h"
#include "Lsprite.h"
#include "sprite_manager.h"
#include "text.h"
#include "background.h"
#include "button.h"
#include "inputfield.h"
#include "collider.h"
#include "bullet.h"
#include "enemy.h"

#include "editor.h"
#include "database.h"
#include "system_manager.h"
#include "game_manager.h"

SceneTest::SceneTest(){}

SceneTest::SceneTest(const SceneTest& other){}

SceneTest::~SceneTest(){
  
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

void SceneTest::init(){
  
  if(NULL == editor_manager_){
    editor_manager_ = new Editor();
    
    editor_manager_->my_Scene_ = this;
    editor_mode_ = false;    
  }
  
  
  SpriteManager::Instance().addSprite("floor.png");
  SpriteManager::Instance().addSprite("tanques_06V2.png");
  SpriteManager::Instance().addSprite("tanques_06.png");
  SpriteManager::Instance().addSprite("wall.png");

  GameManager *GM = GameManager::Instance();



  if ( GM->is_new_game() ){

    //Load spawn points
    Vec2 spawnPoints[16];
    std::fstream level_spawn("../data/maps/level_1/level_1._spawn.csv");
    std::array<char, 255> file_buffer;

    std::string row_buffer;
    std::string tile_buffer;
    int row = 0;
    int col = 0;
    int spawnpoint_index = 0;
    while ( level_spawn.getline(file_buffer.data(), 255) ){

      row_buffer.append(file_buffer.data());

      while ( row_buffer.length() > 0 ){

        tile_buffer += row_buffer.front();
        if ( row_buffer.front() == ',' ){
          tile_buffer.pop_back();
          int check_value = std::stoi(tile_buffer.c_str());

          if ( 0 == check_value ){
            spawnPoints[spawnpoint_index].x = 50.0f * col;
            spawnPoints[spawnpoint_index].y = 50.0f * row;
            spawnpoint_index++;
          }
          tile_buffer.clear();
          col++;
        }

        row_buffer.erase(0, 1);

      }
      col = 0;
      row++;
    }
    level_spawn.close();

    //------------------------------------------

    //BackGround
    addObject(new BackGround("floor"));

    ((BackGround*)getLastObject())->set_reply(32, 18);
    ((BackGround*)getLastObject())->set_scroll(true, true);
    ((BackGround*)getLastObject())->set_speed(0, -1);

    //----------------------------------  
    
    for ( int i = 0; i < 7; i++ ){
      addObject(new Enemy("tanques_06V2"));
      ((Entity*)getLastObject())->set_position(0, 0);
      ((Entity*)getLastObject())->set_rotation(0);      
      ((Entity*)getLastObject())->set_scale(30, 30);

      ((Enemy*)getLastObject())->set_speed(3);
      ((Enemy*)getLastObject())->bullet_->set_speed(4);
      ((Enemy*)getLastObject())->set_spawn_points(spawnPoints, 16);
      ((Enemy*)getLastObject())->alive_ = false;
    }
    
    //----------------------------------  
    
   
    //------------------------------------
    addObject(new Enemy("tanques_06"));
    ((Entity*)getLastObject())->set_position(0, 0);
    ((Entity*)getLastObject())->set_rotation(0);
    ((Entity*)getLastObject())->set_scale(30, 30);

    ((Enemy*)getLastObject())->set_speed(3);
    ((Enemy*)getLastObject())->bullet_->set_speed(4);
    ((Enemy*)getLastObject())->set_spawn_points(spawnPoints, 16);
    ((Enemy*)getLastObject())->alive_ = false;
    ((Enemy*)getLastObject())->is_player_ = true;

    
    //load walls position

    std::fstream level_wall("../data/maps/level_1/level_1._wall.csv");
        
    row_buffer.clear();
    tile_buffer.clear();
    row = 0;
    col = 0;
    while ( level_wall.getline(file_buffer.data(), 255) ){

      row_buffer.append(file_buffer.data());
      
      while ( row_buffer.length() > 0 ){

        tile_buffer += row_buffer.front();
        if ( row_buffer.front() == ',' || row_buffer.front() == '\n' ){
          tile_buffer.pop_back();
          int check_value = std::stoi(tile_buffer.c_str());

          if ( 0 == check_value ){

            addObject(new Collider("wall"));
            ((Entity*)getLastObject())->set_position(50 * col, 50 * row);
            ((Entity*)getLastObject())->set_rotation(0);
            ((Entity*)getLastObject())->set_scale(50, 50);

          }
          tile_buffer.clear();
          col++;
        }

        row_buffer.erase(0, 1);

      }
      
      addObject(new Collider("wall"));
      ((Entity*)getLastObject())->set_position(50 * col, 50 * row);
      ((Entity*)getLastObject())->set_rotation(0);
      ((Entity*)getLastObject())->set_scale(50, 50);

      col = 0;
      row++;
    }
    level_wall.close();

    
    //----------------------------------  
    editor_manager_->setListObj(list_obj_, num_obj_);
    //----------------------------------  
    
    GM->setGameState(num_obj_, list_obj_);
    GM->saveGame(GM->current_save_slot());

  } 
  else if(GM->is_load_game()) {

    GM->loadGame(GM->current_save_slot());

    num_obj_ = GM->getNumObjGameState();
    list_obj_ = GM->getListObjGameState();

    editor_manager_->setListObj(list_obj_, num_obj_);
  } 
  else{
    num_obj_ = GM->getNumObjGameState();
    list_obj_ = GM->getListObjGameState();

    editor_manager_->setListObj(list_obj_, num_obj_);
  }

  offset_x_ = 0;
  exit_ = false;

}

void SceneTest::input(){
  
  static Uint32 s_delay_time = 0;
  
  SystemManager *ptr_system_manager = SystemManager::Instance();   
  sInput input_event = ptr_system_manager->last_input();
  
  if( input_event.keyboard_input == 9 && s_delay_time > 100){
    
    editor_mode_ = !editor_mode_;
    s_delay_time = 0;

    if ( 0 == editor_mode_ ){
      for ( int i = 0; i < num_obj_; i++ ){
        ((Entity*)(list_obj_[i]))->set_position(((Rect*)(list_obj_[i]))->rect_.x - offset_x_, ((Rect*)(list_obj_[i]))->rect_.y);

        if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
          ((Enemy*)(list_obj_[i]))->bullet_->rect_.x -= offset_x_;
        }

      }
      offset_x_ = 0;
    }

  }
  
  if( 0 == editor_mode_){
      
    for(int i=0;i<num_obj_;i++){      
      ((Entity*)(list_obj_[i]))->input(&input_event);          
    }
    
    GameManager::Instance()->input(&input_event);

  }else{
        
    editor_manager_->editionUpdate(&input_event);      
    
    if ( (input_event.keyboard_input == 'A' || input_event.keyboard_input == 'a') && offset_x_ < kEditorWidth ){
      offset_x_+= 5;

      for ( int i = 0; i < num_obj_; i++ ){
        ((Entity*)(list_obj_[i]))->set_position(((Rect*)(list_obj_[i]))->rect_.x + 5, ((Rect*)(list_obj_[i]))->rect_.y);

        if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
          ((Enemy*)(list_obj_[i]))->bullet_->rect_.x += 5;
        }

      }

    } 

    if ( (input_event.keyboard_input == 'D' || input_event.keyboard_input == 'd') && offset_x_ > 0){
      offset_x_-= 5;

      for ( int i = 0; i < num_obj_; i++ ){
        ((Entity*)(list_obj_[i]))->set_position(((Rect*)(list_obj_[i]))->rect_.x - 5, ((Rect*)(list_obj_[i]))->rect_.y);

        if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
          ((Enemy*)(list_obj_[i]))->bullet_->rect_.x -= 5;
        }
      }

    }

  }
    
    
  if(input_event.mouse_key_input == -1 && s_delay_time > 100) {

    editor_manager_->searchInWindows(input_event.mouse_x, input_event.mouse_y);

	}
    
  if(input_event.mouse_wheel_input != 0 && s_delay_time > 100){
      
    editor_manager_->scroll_y_ += input_event.mouse_wheel_input * 15;
      
  }
  
  if ( ptr_system_manager->last_input().keyboard_input == 27 ){
    exit_ = true;
  }

  s_delay_time += ptr_system_manager->delta_time;
  
  if(s_delay_time > 1000){
    s_delay_time = 0;
  }

}
    

  


int SceneTest::update(){
  
  if ( exit_ )return 6;

  if(!editor_mode_){
      
    for(int i=0;i<num_obj_;i++){    
      ((Entity*)(list_obj_[i]))->update();    
    }   
    
    GameManager::Instance()->update();

  }  
  
  return 0;
 
}

void SceneTest::draw(){
  
  for(int i=0;i<num_obj_;i++){
    
    ((Entity*)(list_obj_[i]))->draw();
    
  }

  if(editor_mode_){
    editor_manager_->draw();    
  }
  
}

void SceneTest::exit(){
  
  for ( int i = 0; i < num_obj_; i++ ){
    ((Entity*)(list_obj_[i]))->set_position(((Rect*)(list_obj_[i]))->rect_.x - offset_x_, ((Rect*)(list_obj_[i]))->rect_.y);

    if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
      ((Enemy*)(list_obj_[i]))->bullet_->rect_.x -= offset_x_;
    }

  }
  offset_x_ = 0;


  if ( NULL != editor_manager_ ){
    
    delete editor_manager_;
    editor_manager_ = NULL;
  }
  

  list_obj_ = NULL;
  num_obj_ = 0;
}