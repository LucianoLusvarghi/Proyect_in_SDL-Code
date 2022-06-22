//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/rect.h"


#include "blueprint.h"
#include "editable.h"

BluePrint* Rect::s_my_blueprint_ = NULL;

//contructores

Rect::Rect() : Entity(){
  
  rect_.x = 0;
  rect_.y = 0;
  
  rect_.w = 1;
  rect_.h = 1;
  
  rotation_ = 0;  
  
  color_.r = 255;
  color_.g = 255;
  color_.b = 255;
  color_.a = 255;
  
  solid_ = true;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_rect;

}

Rect::Rect(const char *name):Entity(name){
  
  rect_.x = 0;
  rect_.y = 0;
  
  rect_.w = 1;
  rect_.h = 1;
  
  rotation_ = 0;  
  
  color_.r = 255;
  color_.g = 255;
  color_.b = 255;
  color_.a = 255;
  
  solid_ = true;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_rect;

  
}

Rect::Rect(const Rect& other) : Entity( (Entity&)other) {
      
  rect_ = other.rect_;
  
  rotation_ = other.rotation_;  
  
  color_ = other.color_;
  
  solid_ = other.solid_;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_rect;

}

Rect::~Rect(){
  
  releaceEditable();
}

void Rect::set_position(const SDL_Point& new_position){
  
  rect_.x = new_position.x;
  rect_.y = new_position.y;
  
}

void Rect::set_position(int x, int y){
  
  rect_.x = x;
  rect_.y = y;
 
}
  
void Rect::set_rotation(int new_rotation){
  
  rotation_ = new_rotation;
  
}
  
void Rect::set_scale(const SDL_Point& new_scale){
  
  rect_.w = new_scale.x;
  rect_.h = new_scale.y;
  
}

void Rect::set_scale(int w, int h){
  
  rect_.w = w;
  rect_.h = h;
  
}
  
void Rect::set_color(unsigned char R,unsigned char G,unsigned char B,unsigned char A){
  
  color_.r = R;
  color_.g = G;
  color_.b = B;
  color_.a = A;
  
}

void Rect::set_color(const SDL_Color& colorRGBA){
  
  color_ = colorRGBA;
  
}
  
void Rect::set_solid(uint8_t newState){
  
  solid_ = newState;
  
}

void Rect::draw(){
  
  SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, color_.r, color_.g, color_.b, color_.a);
  
  if(solid_){
    SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_);
  }else{
    SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_);
  }
  
}
 
BluePrint* Rect::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("Rect");
  

  new_blueprint->set_father( Entity::my_blueprint() );
  
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Rect");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "X" );
  new_blueprint->addData(new_agroup, "Y" );
  new_blueprint->addData(new_agroup, "W" );
  new_blueprint->addData(new_agroup, "H" );

  new_blueprint->addAgroup(new_agroup);
  //
 
  new_agroup = new_blueprint->createAgroup("Color");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Color );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "COLOR" );
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Solid");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "solid" );
  new_blueprint->addAgroup(new_agroup);
  //
  
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 

Editable* Rect::my_editable(){
  
  if(NULL != my_editable_){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(Rect::my_blueprint());
  

  new_editable->set_father( Entity::my_editable() );
  
  new_editable->set_id( id() );
  //
  new_editable->addData( &rect_.x );
  new_editable->addData( &rect_.y );
  new_editable->addData( &rect_.w );
  new_editable->addData( &rect_.h );
  // 
  new_editable->addData( &color_ );
  //
  new_editable->addData( &solid_ );
  
  new_editable->set_position( &rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
}

void Rect::callBack(int num_edit){}

void* Rect::cloneObject(){  
  Rect *new_obj = new Rect(*this);
  return new_obj;
}

void Rect::releaceEditable(){
  Entity::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Rect::releaceBluePrint(){
  Entity::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}











