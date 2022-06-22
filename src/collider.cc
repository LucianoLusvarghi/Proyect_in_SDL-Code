//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/collider.h"

#include <string>

#include "blueprint.h"
#include "editable.h"

#include "sprite_manager.h"
#include "System_manager.h"

#include "../deps/Lusvarghi_lib/Include/my_lib.h"

BluePrint* Collider::s_my_blueprint_ = NULL;

Collider::Collider() : Lsprite(){

  name_ = NULL;
  sprite_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_colider;
 
  collision_ = false;

  my_editable_ = NULL;

}


bool Collider::checkCollision(const Collider& other){

  Mat3 transform;
  Mat3 translation;
  Mat3 rotation;
  Mat3 scale;

  //points that make a square
  Vec2 pointsInit[4];
  pointsInit[0].x = -1.0f;
  pointsInit[0].y = -1.0f;

  pointsInit[1].x = 1.0f;
  pointsInit[1].y = -1.0f;

  pointsInit[2].x = 1.0f;
  pointsInit[2].y = 1.0f;

  pointsInit[3].x = -1.0f;
  pointsInit[3].y = 1.0f;

  //current square
  Vec2 pointsEnd[4];

  //points to check collision
  Vec2 puntosControl[8];

  
  translation = MatOfTranslate((float)(rect_.x + (rect_.w >> 1)), (float)(rect_.y + (rect_.h >> 1)));
  scale = MatOfEscalate((float)(rect_.w >> 1), (float)(rect_.h >> 1));
  rotation = Mat3Idt();

  transform = MatOfTransform(translation, rotation, scale);

  for ( int j = 0; j < 4; j++ ){

    pointsEnd[j] = MulOfMatWVec(transform, pointsInit[j]);

  }

  //-----------------------------------
  
  translation = MatOfTranslate((float)(other.rect_.x + (other.rect_.w >> 1)), (float)(other.rect_.y + (other.rect_.h >> 1)));
  scale = MatOfEscalate((float)(other.rect_.w >> 1), (float)(other.rect_.h >> 1));
  rotation = Mat3Idt();

  transform = MatOfTransform(translation, rotation, scale);

  for ( int j = 0; j < 4; j++ ){

    puntosControl[j] = MulOfMatWVec(transform, pointsInit[j]);

  }

  for ( int j = 4; j < 7; j++ ){
    puntosControl[j].x = puntosControl[j - 4].x + ((puntosControl[j + 1 - 4].x - puntosControl[j - 4].x) * 0.5f);
    puntosControl[j].y = puntosControl[j - 4].y + ((puntosControl[j + 1 - 4].y - puntosControl[j - 4].y) * 0.5f);
  }
  puntosControl[7].x = puntosControl[3].x + ((puntosControl[0].x - puntosControl[3].x) * 0.5f);
  puntosControl[7].y = puntosControl[3].y + ((puntosControl[0].y - puntosControl[3].y) * 0.5f);

  //-------------------------------------------
  
  bool inside = false;
  
  for ( int i = 0; i < 8 && !inside; i++ ){

    if ( puntosControl[i].x > pointsEnd[0].x && puntosControl[i].x < pointsEnd[1].x && puntosControl[i].y > pointsEnd[0].y && puntosControl[i].y < pointsEnd[2].y ){
      inside = true;
    }

    if ( i < 4 ){

      if ( pointsEnd[i].x > puntosControl[0].x  && pointsEnd[i].x < puntosControl[1].x  && pointsEnd[i].y > puntosControl[0].y && pointsEnd[i].y < puntosControl[2].y  ){
        inside = true;
      }

    }

  }
  
  collision_ = inside;
  return inside;
}



Collider::Collider(const char *name) : Lsprite(name){

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_colider;

}

Collider::Collider(const Collider& other) : Lsprite((Lsprite&)other){
    
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_colider;

}

Collider::~Collider(){

  if ( NULL != name_ ){
    free(name_);
    name_ = NULL;
  }

  
  releaceEditable();
  my_editable_ = NULL;

}


BluePrint* Collider::my_blueprint(){

  if ( NULL != s_my_blueprint_ ){
    return s_my_blueprint_;
  }

  BluePrint *new_blueprint = new BluePrint("Collider");
  
  new_blueprint->set_father(Lsprite::my_blueprint());

  AGroup* new_agroup = new_blueprint->createAgroup("Colission");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Is Colission");
  new_blueprint->addAgroup(new_agroup);

  if ( NULL == s_my_blueprint_ ){
    s_my_blueprint_ = new_blueprint;
  }

  return s_my_blueprint_;

}

Editable* Collider::my_editable(){

  if ( NULL != my_editable_ ){
    return my_editable_;
  }

  Editable *new_editable = new Editable(Collider::my_blueprint());
  
  new_editable->set_father(Lsprite::my_editable());

  new_editable->set_id(id());
  // 
  new_editable->addData(&collision_);
  //----------------
  new_editable->set_position(&rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);

  new_editable->set_object(this);

  my_editable_ = new_editable;

  return new_editable;

}

void Collider::releaceEditable(){
  Lsprite::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Collider::releaceBluePrint(){
  Lsprite::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}

void* Collider::cloneObject(){
  Collider *new_obj = new Collider(*this);
  
  return new_obj;
}
