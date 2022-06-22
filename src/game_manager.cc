//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/game_manager.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#include <SDL.h>
#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_timer.h>



#include "entity.h"
#include "rect.h"
#include "lsprite.h"
#include "sprite_manager.h"
#include "text.h"
#include "background.h"
#include "enemy.h"
#include "bullet.h"

#include "database.h"
#include "editor.h"

GameManager* GameManager::Instance(){
  static GameManager* s_instance_ = nullptr;
  
  if ( nullptr == s_instance_ ){ s_instance_ = new GameManager(); }
  
  return s_instance_;
  
}

GameManager::GameManager(){
  num_obj_ = 0;
  list_obj_ = NULL;
  current_save_slot_ = -1;
  is_new_game_ = true;
  save_name_ = NULL;
  is_load_game_ = false;
}

GameManager::GameManager(const GameManager& other){}

GameManager::~GameManager(){
  destroyState();  
}

void GameManager::setGameState(unsigned int num_obj, void **list_obj){
  num_obj_ = num_obj;
  list_obj_ = list_obj;
}

unsigned int GameManager::getNumObjGameState()const{return num_obj_;}
void** GameManager::getListObjGameState()const{return list_obj_;}

void GameManager::saveGame(unsigned int slot){
  DataBase &database = DataBase::Instance();

  unsigned int TotalSaved = database.getNumSaveGame(database.current_user_id_);

  //Get the IDs in the database of the Saved games
  int *ListSavesIds = database.getListSaveGame(database.current_user_id_);

  if ( slot <= TotalSaved ){

    char* nameSave = database.getSaveGameName(ListSavesIds[slot-1]);
    database.updateSaveGame(nameSave, num_obj_, list_obj_);
    
    free(nameSave);

  } else{
    database.setNewSaveGame(save_name_, num_obj_, list_obj_);
  }

  
  free(ListSavesIds);

  current_save_slot_ = slot;
}

void GameManager::loadGame(unsigned int slot){
  DataBase &database = DataBase::Instance();

  unsigned int TotalSaved = database.getNumSaveGame(database.current_user_id_);
  if(slot > TotalSaved )return;
  //Get the IDs in the database of the Saved games
  int *ListSavesIds = database.getListSaveGame(database.current_user_id_);
  
  char* backupName = (char*)calloc(strlen(save_name_) + 1, sizeof(char));
  
  memcpy(backupName, save_name_, strlen(save_name_));
  destroyState();
  if ( NULL != save_name_ ){
    
    free(save_name_);
  }
  save_name_ = backupName;

  num_obj_ = database.getNumObjSaveGame(ListSavesIds[slot-1]);
  list_obj_ = database.getListObjSaveGame(ListSavesIds[slot - 1]);

  
  free(ListSavesIds);

  current_save_slot_ = slot;
}

void GameManager::set_is_new_game(bool newState){is_new_game_ = newState;}
bool GameManager::is_new_game()const{return is_new_game_;}

void GameManager::destroyState(){

  for (unsigned int i = 0; i < num_obj_; i++ ){
    ((Entity*)(list_obj_[i]))->releaceEditable();    
    delete (Entity*)list_obj_[i];
  }

  
  free(list_obj_);

  list_obj_ = NULL;
  num_obj_ = 0;
  
  
  free(save_name_);
  save_name_ = NULL;
}

void GameManager::set_save_name(const char* name){
  int lenght = strlen(name);
  char *new_text = (char*)calloc(lenght + 1, sizeof(char));
  
  if ( NULL != new_text ){
    if(NULL != save_name_ ){
      
      free(save_name_);
    }
    save_name_ = new_text;
    memcpy(save_name_, name, lenght * sizeof(char));
  }

}
const char* GameManager::save_name()const{return save_name_;}

void GameManager::set_current_save_slot(unsigned int slot){ current_save_slot_ = slot; }
unsigned int GameManager::current_save_slot()const{ return current_save_slot_; }

void GameManager::set_is_load_game(bool newState){ is_load_game_ = newState; }
bool GameManager::is_load_game()const{ return is_load_game_; }


void GameManager::input(sInput *input_event){

  for ( unsigned int i = 0; i < num_obj_; i++ ){
  
    if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
      if ( ((Enemy*)(list_obj_[i]))->is_player_ ){
        ((Enemy*)(list_obj_[i]))->input(input_event);
      }
    }
  
  }

}

void GameManager::update(){

 
  for (unsigned int i = 0; i < num_obj_; i++ ){

    //Check all iteraction with Enemy
    if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
      
      for ( unsigned int j = 0; j < num_obj_; j++ ){

        if ( j != i ){

          //Check collicion Enemy-wall and Enemy->Bullet - wall
          if ( ((Entity*)(list_obj_[j]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_colider ){

            //Check colicion Enemy -- wall
            if ( ((Enemy*)(list_obj_[i]))->checkCollision(*((Collider*)(list_obj_[j]))) ){

              Vec2 newDirection;
              Vec2 oldDirection = ((Enemy*)(list_obj_[i]))->direction();

              ((Enemy*)(list_obj_[i]))->set_direction((int)(oldDirection.x * -1), (int)(oldDirection.y * -1));
              ((Enemy*)(list_obj_[i]))->update();
              ((Enemy*)(list_obj_[i]))->set_direction((int)(oldDirection.x * -1), (int)(oldDirection.y * -1));
              do{
                int random = rand() & 3;
                switch ( random ){
                case 0: newDirection = { 1,0 }; break;
                case 1: newDirection = { -1,0 }; break;
                case 2: newDirection = { 0,1 }; break;
                case 3: newDirection = { 0,-1 }; break;
                default:
                  break;
                }
              } while ( oldDirection.x == newDirection.x && oldDirection.y == newDirection.y );

              ((Enemy*)(list_obj_[i]))->set_direction(newDirection);
              
            }

            //check Enemy->Bullet -- wall
            if ( ((Enemy*)(list_obj_[i]))->bullet_->alive_ ){
              if ( ((Enemy*)(list_obj_[i]))->bullet_->checkCollision(*((Collider*)(list_obj_[j]))) ){
                ((Enemy*)(list_obj_[i]))->bullet_->alive_ = false;
                ((Enemy*)(list_obj_[i]))->bullet_->explosion_ = true;
              }
            }

          }
          
          //Check coolicion Enemy - Enemy, Enemy->Bullet - Enemy, Enemy->Bullet - Enemy->Bullet
          if ( ((Entity*)(list_obj_[j]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){

            //Check other Enemy is alive
            if ( ((Enemy*)(list_obj_[j]))->alive_ ){

              //check Enemy -- Enemy
              if ( ((Enemy*)(list_obj_[i]))->checkCollision(*((Enemy*)(list_obj_[j]))) ){

                Vec2 newDirection;
                Vec2 oldDirection = ((Enemy*)(list_obj_[i]))->direction();

                ((Enemy*)(list_obj_[i]))->set_direction((int)(oldDirection.x * -1), (int)(oldDirection.y * -1));
                ((Enemy*)(list_obj_[i]))->update();
                ((Enemy*)(list_obj_[i]))->set_direction((int)(oldDirection.x * -1), (int)(oldDirection.y * -1));
                do{
                  int random = rand() & 3;
                  switch ( random ){
                  case 0: newDirection = { 1,0 }; break;
                  case 1: newDirection = { -1,0 }; break;
                  case 2: newDirection = { 0,1 }; break;
                  case 3: newDirection = { 0,-1 }; break;
                  default:
                    break;
                  }
                } while ( oldDirection.x == newDirection.x && oldDirection.y == newDirection.y );

                ((Enemy*)(list_obj_[i]))->set_direction(newDirection);


              }

              //Check Enemy->Bullet -- Enemy
              if ( ((Enemy*)(list_obj_[i]))->bullet_->alive_ ){
                if ( ((Enemy*)(list_obj_[i]))->bullet_->checkCollision(*((Enemy*)(list_obj_[j]))) ){
                  ((Enemy*)(list_obj_[i]))->bullet_->alive_ = false;
                  ((Enemy*)(list_obj_[i]))->bullet_->explosion_ = true;

                  ((Enemy*)(list_obj_[j]))->alive_ = false;

                }
              }
            }
            //Check Enemy->Bullet -- Enemy->Bullet
            if ( ((Enemy*)(list_obj_[i]))->bullet_->alive_ && ((Enemy*)(list_obj_[j]))->bullet_->alive_ ){
              if ( ((Enemy*)(list_obj_[i]))->bullet_->checkCollision(  *((Enemy*)(list_obj_[j]))->bullet_  ) ){
                ((Enemy*)(list_obj_[i]))->bullet_->alive_ = false;
                ((Enemy*)(list_obj_[i]))->bullet_->explosion_ = true;

                ((Enemy*)(list_obj_[j]))->bullet_->alive_ = false;
                ((Enemy*)(list_obj_[j]))->bullet_->explosion_ = true;
              }
            }

          }
        
        }
      }

    }


    //Check for spawn
    if ( ((Entity*)(list_obj_[i]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){
    
      if ( !((Enemy*)(list_obj_[i]))->alive_ ){

        if ( ((Enemy*)(list_obj_[i]))->delay_time_ > ((Enemy*)(list_obj_[i]))->time_to_respawn_ ){
          ((Enemy*)(list_obj_[i]))->delay_time_ = 0;

          ((Enemy*)(list_obj_[i]))->spawn();

          bool colicion = true;
          while ( colicion ){

            colicion = false;
            for ( unsigned int j = 0; j < num_obj_ && !colicion; j++ ){

              if ( j != i ){                
                //Check coolicion Enemy - Enemy, Enemy->Bullet - Enemy, Enemy->Bullet - Enemy->Bullet
                if ( ((Entity*)(list_obj_[j]))->engine_tag_ == EngineObjectTag::kEngineObjectTag_enemy ){

                  //Check other Enemy is alive
                  if ( ((Enemy*)(list_obj_[j]))->alive_ ){

                    //check Enemy -- Enemy
                    if ( ((Enemy*)(list_obj_[i]))->checkCollision(*((Enemy*)(list_obj_[j]))) ){
                      colicion = true;
                    }

                   
                  }
                  
                }

              }
            }

            if ( colicion ){
              ((Enemy*)(list_obj_[i]))->spawn();
            }


          }

        }

      }

    }


  }



}