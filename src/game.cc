//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/game.h"

#include "sprite_manager.h"
#include "system_manager.h"
#include "database.h"

#include "scene.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "scene_login.h"
#include "scene_load.h"
#include "scene_register.h"
#include "scene_pause.h"
#include "scene_save.h"

Game::Game(){
  
  num_scenes_ = 0;
  list_scenes_ = NULL;
  current_scene_ = NULL;
  
}

Game::Game(const Game& other){}

Game::~Game(){}
  
int Game::init(){
  
  SystemManager::Instance()->set_windows_name("Progecto SDL");
    
  if(!SystemManager::Instance()->initSDL()){
    return -1;
  }
  Scene *new_scene = (Scene*) new Scene();  
  addScene( new_scene );  

  new_scene = (Scene*) new SceneMenu();  
  addScene(new_scene);

  new_scene = (Scene*) new SceneLogin();  
  addScene(new_scene);

  new_scene = (Scene*) new SceneLoad();  
  addScene(new_scene);

  new_scene = (Scene*) new SceneTest();  
  addScene(new_scene);

  new_scene = (Scene*) new SceneRegister();  
  addScene(new_scene);

  new_scene = (Scene*) new ScenePause();  
  addScene(new_scene);

  new_scene = (Scene*) new SceneSave();  
  addScene(new_scene);


  DataBase::Instance().getSprites();
  
  return 0;
}

void Game::loop(){
  
  bool gameover = false;  
  int change_scene = 0;
  SystemManager *ptr_system_manager = SystemManager::Instance();
   
  current_scene_ = list_scenes_[1];
  current_scene_->init();
  
  
  while( !gameover ){
    
    
    ptr_system_manager->last_time = SDL_GetTicks();
    ptr_system_manager->UpdateSistemInput();
    //-------------------------------------------------------
    SDL_SetRenderDrawColor(ptr_system_manager->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_system_manager->Main_render_);
    //---------------------------------------------------------    
    
    if( ptr_system_manager->last_input().keyboard_input == 27 && current_scene_ == list_scenes_[1] ){
      gameover = true;
    }
        
    current_scene_->input();
    change_scene = current_scene_->update();
    if( change_scene != 0){
      if(change_scene < 0){
        gameover = true;
      }else{
        current_scene_->exit();
        current_scene_ = list_scenes_[change_scene];
        current_scene_->init();
      }
    }
    current_scene_->draw();
    //------------------------------------------------------
    SDL_RenderPresent(ptr_system_manager->Main_render_);
    //-------------------------------------------------------
    ptr_system_manager->current_time = SDL_GetTicks();    
    ptr_system_manager->delta_time = (ptr_system_manager->current_time - ptr_system_manager->last_time);
    
    while( ptr_system_manager->delta_time < (unsigned)(1000 / ptr_system_manager->fps) ){
      
      ptr_system_manager->current_time = SDL_GetTicks();    
      ptr_system_manager->delta_time = (ptr_system_manager->current_time - ptr_system_manager->last_time);
          
    }
    //-----------------------------------------------------------------
    ptr_system_manager->clearInput();
  }
  
  for(int i=0;i<num_scenes_;i++){
    list_scenes_[i]->exit();
  }
  
}

void Game::exit(){
  
  for(int i=0;i<num_scenes_;i++){
    delete list_scenes_[i];
  }
  
  if(NULL != list_scenes_){
    
    free(list_scenes_);    
  }
  list_scenes_ = NULL;
  num_scenes_ = 0;
  
  DataBase::Instance().setSprites();
  
  SpriteManager &s_instance_ = SpriteManager::Instance();  
  delete &s_instance_;
  


  delete &(DataBase::Instance());
  
  SystemManager::Instance()->exitSDL();

  
  delete SystemManager::Instance();

}



void Game::addScene( Scene *new_scene ){
  
  if(NULL == new_scene){
    return;
  }
  
  for(int i=0;i<num_scenes_;i++){
    
    if(list_scenes_[i] == new_scene){
      return;
    }
    
  }
  
  num_scenes_++;
  
  Scene **new_list_scene = (Scene**)calloc( num_scenes_ , sizeof(Scene*) );
  

  if(NULL != new_list_scene ){
    
    if(num_scenes_ > 1){
      memcpy(new_list_scene, list_scenes_, (num_scenes_ - 1) * sizeof(Scene*) );
    }
    new_list_scene[num_scenes_ - 1] = new_scene;
    if(NULL != list_scenes_){
      
      free( list_scenes_ );
    }
    list_scenes_ = new_list_scene;
    
  } 
    
}




























