//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#include "../include/entity.h"

#include "blueprint.h"
#include "editable.h"

int Entity::s_last_id = 0;

BluePrint* Entity::s_my_blueprint_ = NULL;

Entity::Entity(){
  
  tag_ = 0;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_noTag;
  enable_ = true;
  id_ = s_last_id;
  s_last_id++;
  
  my_editable_ = NULL;
  
  my_name_ = (char*)calloc( strlen("DefaultName")+1, sizeof(char) );  
  
  memcpy(my_name_, "DefaultName", strlen("DefaultName"));
  
}

Entity::Entity(int tag){
  
  tag_ = tag;
  enable_ = true;
  id_ = s_last_id;
  s_last_id++;
  
  my_editable_ = NULL;
  
  my_name_ = (char*)calloc( strlen("DefaultName")+1, sizeof(char) ); 
  
  memcpy(my_name_, "DefaultName", strlen("DefaultName"));
  
}

Entity::Entity(const char *name){
  
  tag_ = 0;
  enable_ = true;
  id_ = s_last_id;
  s_last_id++;
  
  if(NULL != name){
    
    my_name_ = (char*)calloc( strlen(name)+1, sizeof(char) ); 
    
    memcpy(my_name_, name, strlen(name));
    
  }else{
    
    my_name_ = (char*)calloc( strlen("DefaultName")+1, sizeof(char) ); 
    
    memcpy(my_name_, "DefaultName", strlen("DefaultName"));
    
  }
  
  
  
  my_editable_ = NULL;
}

Entity::Entity(const char *name, int tag){
  
  tag_ = tag;
  enable_ = true;
  id_ = s_last_id;
  s_last_id++;
  
  if(NULL != name){
    
    my_name_ = (char*)calloc( strlen(name)+1, sizeof(char) ); 
    
    memcpy(my_name_, name, strlen(name));
    
  }else{
    
    my_name_ = (char*)calloc( strlen("DefaultName")+1, sizeof(char) ); 
    
    memcpy(my_name_, "DefaultName", strlen("DefaultName"));
    
  }
  
  my_editable_ = NULL;
}

Entity::Entity(const Entity& other) {
  
  tag_ = other.tag_;
  enable_ = other.enable_;
  id_ = s_last_id;
  s_last_id++;
  
  my_name_ = (char*)calloc( strlen(other.my_name_)+1, sizeof(char) );  
  
  memcpy(my_name_, other.my_name_, strlen(other.my_name_));
  
  my_editable_ = NULL;
}

Entity::~Entity(){
  
 
  free(my_name_);
  
 
  releaceEditable();

}

int Entity::id(){
  return id_;
}


void Entity::toggleEnable(){
  
  enable_ = !enable_;
  
}

BluePrint* Entity::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("Entity");
 
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Enable");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Enable" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Name");
  new_blueprint->set_my_data_type(new_agroup, kDataType_String );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Name" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Tag");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Tag" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("ID");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, false );
  new_blueprint->addData(new_agroup, "ID" );
  new_blueprint->addAgroup(new_agroup);
  //
  
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 

Editable* Entity::my_editable(){
  
  if( NULL != my_editable_ ){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(Entity::my_blueprint());
  

  new_editable->set_id( id_ );
  
  new_editable->addData( &enable_ );
  new_editable->addData( &my_name_ );
  new_editable->addData( &tag_ );
  new_editable->addData( &id_ );
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
}

void Entity::set_position(int x, int y){}
void Entity::set_position(const SDL_Point &new_position){}
void Entity::set_rotation(int ang){}
void Entity::set_scale(int w, int h){}
void Entity::set_scale(const SDL_Point &new_scale){}

void Entity::set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a){}

void Entity::callBack(int num_edit){}

void Entity::input(sInput *input_event){}
void Entity::update(){}
void Entity::draw(){}

void Entity::set_my_name(const char *name){
  
  char *new_name = (char*)calloc( strlen(name)+1 , sizeof(char) );
  
  memcpy(new_name, name, strlen(name)+1 );
  
  if(NULL != my_name_){
    
    free(my_name_);
  }
  
  my_name_ = new_name;
  
}

void* Entity::cloneObject(){
  
  Entity *new_obj = new Entity(*this);
  

  return new_obj;
}

void Entity::releaceEditable(){
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Entity::releaceBluePrint(){
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}

