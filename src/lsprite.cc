//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/Lsprite.h"


#include "sprite_manager.h"
#include "System_manager.h"
#include "blueprint.h"
#include "editable.h"


BluePrint* Lsprite::s_my_blueprint_ = NULL;

Lsprite::Lsprite() : Rect(){
  
  name_ = NULL;
  sprite_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_sprite;

  set_position(0,0);
  set_rotation(0);
  set_scale(1,1);
  
  
  my_editable_ = NULL;
  
}


Lsprite::Lsprite(const char *name) : Rect(name){
  
  
  name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
  
  memcpy(name_, name, strlen(name));
    
  sprite_ = SpriteManager::Instance().getSprite(name_);
  
  if(sprite_ != NULL){
    set_position(0,0);
    SDL_QueryTexture(sprite_, NULL, NULL, &rect_.w, &rect_.h);
    
  }
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_sprite;

}

Lsprite::Lsprite(const Lsprite& other) : Rect( (Rect&)other ){
  
  if (other.name() != NULL) {
    name_ = (char*)calloc(strlen(other.name()) + 1, sizeof(char));
    
    memcpy(name_, other.name(), strlen(other.name()));
  }
  else{
    name_ = NULL;
  }
  sprite_ = other.sprite_;
  rect_ = other.rect_;
  rotation_ = other.rotation_;
    
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_sprite;

}

Lsprite::~Lsprite(){
  

  if(NULL != name_){
    
    free(name_);
    name_ = NULL;
  }
  
  
  releaceEditable();

  my_editable_ = NULL;
  
}

void Lsprite::set_name(const char *name){
  
  if(name_ != NULL){
    
    char* new_name = name_;
    
    name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
    

    if( NULL == name_ ){
      name_ = new_name;
    }else{
      memcpy(name_, name, strlen(name));
      
      free(new_name);
    }
    
     
  }else{
    
    name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
    
    memcpy(name_, name, strlen(name));
    
  }
    
}

bool Lsprite::callSpriteManager(){
  
  sprite_ = SpriteManager::Instance().getSprite(name_);
  
  if(sprite_ == NULL){
    return false;
  }
    
  SDL_QueryTexture(sprite_, NULL, NULL, &rect_.w, &rect_.h);
  
  return true;
  
}

const char* Lsprite::name()const{return name_;}

int Lsprite::height(){return rect_.h;}

int Lsprite::width(){return rect_.w;}

void Lsprite::draw(){
  
  if(sprite_ == NULL){return;}
  SDL_Point center = { (rect_.w >> 1), (rect_.h >> 1) };
  SDL_RenderCopyEx(SystemManager::Instance()->Main_render_, sprite_, NULL, &rect_, rotation_, &center, SDL_FLIP_NONE  );
  
}


BluePrint* Lsprite::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("Sprite");
  

  new_blueprint->set_father( Rect::my_blueprint() );
  
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Name");
  new_blueprint->set_my_data_type(new_agroup, kDataType_SpriteManager );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, " " );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Textura");
  new_blueprint->set_my_data_type(new_agroup, kDataType_IMG );
  new_blueprint->set_editable(new_agroup, false );
  new_blueprint->addData(new_agroup, " " );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Rotacion");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Ang");
  new_blueprint->addAgroup(new_agroup);

  
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 

Editable* Lsprite::my_editable(){
  
  if( NULL != my_editable_ ){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(Lsprite::my_blueprint());
 

  new_editable->set_father( Rect::my_editable() );
  
  new_editable->set_id( id() );
  //
  new_editable->addData( &name_ );
  //
  new_editable->addData( &sprite_ );
  //
  new_editable->addData(&rotation_);

  //----------------
  new_editable->set_position( &rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
}

void Lsprite::callBack(int num_edit){
  
  SDL_Rect check_change_in_rect;  
  SDL_QueryTexture(sprite_, NULL, NULL, &check_change_in_rect.w, &check_change_in_rect.h);
    
  if( rect_.w != check_change_in_rect.w || rect_.h != check_change_in_rect.h){
    check_change_in_rect.w = rect_.w;
    check_change_in_rect.h = rect_.h;
  }  
  set_name(name_);
  callSpriteManager();
  
  rect_.w = check_change_in_rect.w;
  rect_.h = check_change_in_rect.h;
  
}

void* Lsprite::cloneObject(){
  
  Lsprite *new_obj = new Lsprite(*this);
  

  return new_obj;
  
  
}

void Lsprite::releaceEditable(){
  Rect::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Lsprite::releaceBluePrint(){
  Rect::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}








