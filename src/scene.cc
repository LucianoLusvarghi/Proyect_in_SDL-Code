//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/scene.h"


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

Scene::Scene(){
  
  num_obj_ = 0;
  list_obj_ = NULL;
  editor_manager_ = new Editor();
  
  editor_manager_->my_Scene_ = this;
  editor_mode_ = false;
  
  exit_ = false;
  
  DataBase &ptr_database = DataBase::Instance();
  
  Entity *obj = new Entity();  
  ptr_database.addObj(obj);


  obj = new Rect();  
  ptr_database.addObj(obj);


  obj = new Lsprite();  
  ptr_database.addObj(obj);


  obj = new BackGround();  
  ptr_database.addObj(obj);


  obj = new Text();  
  ptr_database.addObj(obj);


  obj = new Collider();  
  ptr_database.addObj(obj);


  obj = new Enemy();  
  ptr_database.addObj(obj);
}

Scene::Scene(const Scene& other){}

Scene::~Scene(){
  
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

void Scene::init(){}

void Scene::input(){}

int Scene::update(){return 0;}

void Scene::draw(){}

void Scene::exit(){
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
  
void Scene::addObject(void *new_obj){
  
  if(NULL == new_obj){
    return;
  }
  
  

  for(int i=0;i<num_obj_;i++){
    
    if(list_obj_[i] == new_obj){
      
      ((Entity*)(new_obj))->releaceEditable();

      
      delete (Entity*)(new_obj);

      return;
    }
    
  }
  
  num_obj_++;
  
  void **new_list_obj = (void**)calloc( num_obj_ , sizeof(void*) );
  

  if(NULL != new_list_obj ){
    if(num_obj_ > 1){
      memcpy(new_list_obj, list_obj_, (num_obj_ - 1) * sizeof(void*) );
    }
    new_list_obj[num_obj_ - 1] = new_obj;
    if(NULL != list_obj_){
      
      free( list_obj_ );
    }
    list_obj_ = new_list_obj;
    
  } 
  
}

void* Scene::getLastObject() const{
  
  return list_obj_[num_obj_-1];  
  
}

void Scene::lessObject(void *oldObject){
  
  if(NULL == oldObject){
    return;
  }
  
  
  for(int i=0;i<num_obj_;i++){
    
    if( list_obj_[i] == oldObject ){
      
      void **new_list_obj = (void**)calloc((num_obj_ -1) , sizeof(Editable*) );
      

      int li = 0;      
      for(int j=0;j<num_obj_;j++){
        
        if( j != i){
          new_list_obj[li] = list_obj_[j];
          li++;
        }
      }
      num_obj_--;
      
      
      free(list_obj_);
      
      list_obj_ = new_list_obj;
      
      ((Entity*)oldObject)->releaceEditable();
      
      delete (Entity*)oldObject;      
      
      return;
    }  
    
  }
  
  
  
}
