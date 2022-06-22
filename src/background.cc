//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/background.h"

#include <string>

#include "blueprint.h"
#include "editable.h"

BluePrint* BackGround::s_my_blueprint_ = NULL;

BackGround::BackGround() : Lsprite(){
  
  name_ = NULL;
  sprite_ = NULL;
    
  reply_x_ = 1;
  reply_y_ = 1;
  
  speed_x_ = 0;
  speed_y_ = 0;
  
  scroll_x_ = false;
  scroll_y_ = false;

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_backGround;
}

BackGround::BackGround(const BackGround& other) : Lsprite( (Lsprite&)other ){
  
  if(sprite_ != NULL){
    set_position(0,0);
    
    rect_.w = SystemManager::Instance()->kWindowW / other.reply_x_;
    rect_.h = SystemManager::Instance()->kWindowH / other.reply_y_;   
        
  }
  
  reply_x_ = other.reply_x_;
  reply_y_ = other.reply_y_;
  
  scroll_x_ = other.scroll_x_;
  scroll_y_ = other.scroll_y_;
  
  speed_x_ = other.speed_x_;
  speed_y_ = other.speed_y_;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_backGround;
  
}

BackGround::BackGround(const char *name):Lsprite(name){
  
  if(sprite_ != NULL){
    set_position(0,0);
    rect_.w = SystemManager::Instance()->kWindowW;
    rect_.h = SystemManager::Instance()->kWindowH;   
  }
  
  reply_x_ = 1;
  reply_y_ = 1;
  
  speed_x_ = 0;
  speed_y_ = 0;
  
  scroll_x_ = false;
  scroll_y_ = false;

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_backGround;

}

BackGround::BackGround(const char *name, int reply_x, int reply_y):Lsprite(name){
  
  if(sprite_ != NULL){
    set_position(0,0);
    
    rect_.w = SystemManager::Instance()->kWindowW / reply_x;
    rect_.h = SystemManager::Instance()->kWindowH / reply_y;   
        
  }
  
  reply_x_ = reply_x;
  reply_y_ = reply_y;
  
  speed_x_ = 0;
  speed_y_ = 0;
  
  scroll_x_ = false;
  scroll_y_ = false;

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_backGround;

}

BackGround::BackGround(const char *name, int reply_x, int reply_y, bool scroll_x, bool scroll_y):Lsprite(name){
  
  if(sprite_ != NULL){
    set_position(0,0);
    
    rect_.w = SystemManager::Instance()->kWindowW / reply_x;
    rect_.h = SystemManager::Instance()->kWindowH / reply_y;   
        
  }
  
  reply_x_ = reply_x;
  reply_y_ = reply_y;
  
  scroll_x_ = scroll_x;
  scroll_y_ = scroll_y;
  
  speed_x_ = 0;
  speed_y_ = 0;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_backGround;
}
  
BackGround::~BackGround(){
  
  releaceEditable();

  my_editable_ = NULL;
}


void BackGround::set_reply(int reply_x, int reply_y){
  
  if(reply_x <= 0){reply_x = 1;}
  if(reply_y <= 0){reply_y = 1;}
  
  reply_x_ = reply_x;
  reply_y_ = reply_y;
  
  rect_.w = SystemManager::Instance()->kWindowW / reply_x_;
  rect_.h = SystemManager::Instance()->kWindowH / reply_y_;
  
}

void BackGround::set_scroll(bool scroll_x, bool scroll_y){
  
  scroll_x_ = scroll_x;
  scroll_y_ = scroll_y;
  
}

void BackGround::set_speed(int speed_x, int speed_y){
  
  speed_x_ = speed_x;
  speed_y_ = speed_y;
  
}
  
void BackGround::update(){
  
  if(scroll_x_){
    
    set_position( rect_.x + speed_x_, rect_.y);
    
    if( rect_.x > rect_.w ){
      rect_.x -= rect_.w;
    }
    
    if( rect_.x < -rect_.w ){
      rect_.x += rect_.w;
    }
    
  }
  
  if(scroll_y_){
    set_position( rect_.x, rect_.y + speed_y_);
    
    if( rect_.y > rect_.h ){
      rect_.y -= rect_.h;
    }
    
    if( rect_.y < -rect_.h ){
      rect_.y += rect_.h;
    }
    
    
  }
  
  
}

void BackGround::draw(){
  
  if(sprite_ == NULL){return;}
  
  for(int x=-1;x<reply_x_+1;x++){
    
    for(int y=-1;y<reply_y_+1;y++){
      
      SDL_Rect rect_destination = { rect_.x + (x * rect_.w) , rect_.y + (y * rect_.h), rect_.w, rect_.h};
            
      SDL_RenderCopy(SystemManager::Instance()->Main_render_, sprite_, NULL, &rect_destination);
      
    }   
    
  }
  
  
}


BluePrint* BackGround::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("BackGround");
  
  new_blueprint->set_father( Lsprite::my_blueprint() );
  
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Replicas");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "EjeX" );
  new_blueprint->addData(new_agroup, "EjeY" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("speed");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "EjeX" );
  new_blueprint->addData(new_agroup, "EjeY" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Scroll");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "EjeX" );
  new_blueprint->addData(new_agroup, "EjeY" );
  new_blueprint->addAgroup(new_agroup);
  
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 

Editable* BackGround::my_editable(){
  
  if( NULL != my_editable_ ){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(BackGround::my_blueprint());
  

  new_editable->set_father( Lsprite::my_editable() );
  
  new_editable->set_id(id());
  //
  new_editable->addData(&reply_x_);
  new_editable->addData(&reply_y_ );
  //
  new_editable->addData(&speed_x_);
  new_editable->addData(&speed_y_ );
  //
  new_editable->addData(&scroll_x_);
  new_editable->addData(&scroll_y_ );
  //
  new_editable->set_position(&rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
}

void BackGround::callBack(int num_edit){
   
  Lsprite::callBack(num_edit);  
  set_reply(reply_x_, reply_y_);
  
}

void* BackGround::cloneObject(){
  
  BackGround *new_obj = new BackGround(*this);
  
  return new_obj;
  
}

void BackGround::releaceEditable(){
  Lsprite::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void BackGround::releaceBluePrint(){
  Lsprite::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}