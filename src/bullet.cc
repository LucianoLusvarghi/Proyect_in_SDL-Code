//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/bullet.h"

#include "sprite_manager.h"
#include "System_manager.h"

#include "blueprint.h"
#include "editable.h"

BluePrint* Bullet::s_my_blueprint_ = NULL;

Bullet::Bullet() : Collider(){

  engine_tag_ = EngineObjectTag::kEngineObjectTag_bullet;
  set_position(0, 0);
  set_rotation(0);
  set_scale(1, 1);

  my_editable_ = NULL;

 
  speed_ = 2; 
  alive_ = false;
  explosion_ = false;
  direction_ = { 1.0f, 0.0f };
 
  SpriteManager::Instance().addSprite("explosion.png");
  texture_explosion_ = SpriteManager::Instance().getSprite("explosion");
  sequence_ = 0;
  delay_time_ = 0;
}

Bullet::Bullet(const char *name) : Collider(name){

  my_editable_ = NULL;

  
  speed_ = 2;
  
  alive_ = false;
  explosion_ = false;
  direction_ = { 1.0f, 0.0f };
 
  engine_tag_ = EngineObjectTag::kEngineObjectTag_bullet;

  SpriteManager::Instance().addSprite("explosion.png");

  texture_explosion_ = SpriteManager::Instance().getSprite("explosion");
  sequence_ = 0;
  delay_time_ = 0;
}

Bullet::Bullet(const Bullet& other) : Collider((Collider&)other){

  my_editable_ = NULL;
   
  speed_ = other.speed_;  
  alive_ = false;
  explosion_ = false;
  direction_ = other.direction_;
 
  engine_tag_ = EngineObjectTag::kEngineObjectTag_bullet;
  SpriteManager::Instance().addSprite("explosion.png");
  texture_explosion_ = SpriteManager::Instance().getSprite("explosion");
  sequence_ = 0;
  delay_time_ = 0;
}

Bullet::~Bullet(){

  if ( NULL != name_ ){
    free(name_);
    name_ = NULL;
  }

  
  releaceEditable();

}


void Bullet::update(){

  if ( alive_){
    
    rect_.x += (int)(direction_.x * speed_);
    rect_.y += (int)(direction_.y * speed_);

    if ( rect_.x < 0 || rect_.x + rect_.w > SystemManager::Instance()->kWindowW ){
      alive_ = false;
      explosion_ = true;
    }

    if ( rect_.y < 0 || rect_.y + rect_.h > SystemManager::Instance()->kWindowH ){
      alive_ = false;
      explosion_ = true;
    }

  } else{

    if ( explosion_ ){
      delay_time_ += SystemManager::Instance()->delta_time;
      if ( delay_time_ > 30 ){
        delay_time_ = 0;
        sequence_++;
        if ( sequence_ > 24 ){
          explosion_ = false;
          sequence_ = 0;
        }
       
      }
    }
  }

}

void Bullet::draw(){

  if ( alive_){
    if ( direction_.x < 0.0f ){
      rotation_ = 180;
    } else if ( direction_.x > 0.0f ){
      rotation_ = 0;
    } else if ( direction_.y > 0.0f ){
      rotation_ = 90;
    } else{
      rotation_ = 270;
    }
    Collider::draw();
  }else if (explosion_ && NULL != texture_explosion_ ){

    SDL_Rect position;    
    position.x = rect_.x - 12;
    position.y = rect_.y - 24;
    position.w = 50;
    position.h = 50;
    SDL_Point center = { (position.w >> 1), (position.h >> 1) };
    
    SDL_Rect recuadro;
    int i, j;
    bool encontrado = false;
    for ( i = 0; i < 5 && !encontrado; ++i ){
      for ( j = 0; j < 5 && !encontrado; ++j ){
        if ( (i * 5) + j == sequence_ )encontrado = true;
      }
    }
    recuadro.x = j * 66;
    recuadro.y = i * 81;
    recuadro.w = 66;
    recuadro.h = 81;

    SDL_RenderCopyEx(SystemManager::Instance()->Main_render_, texture_explosion_, &recuadro, &position, 0.0, &center, SDL_FLIP_NONE);

  }


}


void Bullet::spawn(Vec2 position, Vec2 direction){

  if ( alive_ || explosion_ )return;

  rect_.x = (int)((position.x + 12) + (direction.x * 48));
  rect_.y = (int)((position.y + 12) + (direction.y * 48));

  direction_ = direction;

  alive_ = true;

}

void Bullet::set_speed(int newValue){ speed_ = newValue; }
int  Bullet::speed()const{ return speed_; }










BluePrint* Bullet::my_blueprint(){

  if ( NULL != s_my_blueprint_ ){
    return s_my_blueprint_;
  }

  BluePrint *new_blueprint = new BluePrint("Bullet");
  

  new_blueprint->set_father(Collider::my_blueprint());

  //
  AGroup* new_agroup = new_blueprint->createAgroup("direction");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Float);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "X");
  new_blueprint->addData(new_agroup, "Y");

  new_blueprint->addAgroup(new_agroup);
  //

  //
  new_agroup = new_blueprint->createAgroup("speed");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "speed");

  new_blueprint->addAgroup(new_agroup);
  //

  //
  new_agroup = new_blueprint->createAgroup("Alive");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Alive");

  new_blueprint->addAgroup(new_agroup);
  //

  //
  new_agroup = new_blueprint->createAgroup("Explocion");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Explocion");

  new_blueprint->addAgroup(new_agroup);
  //

 

  if ( NULL == s_my_blueprint_ ){
    s_my_blueprint_ = new_blueprint;
  }

  return s_my_blueprint_;

}

Editable* Bullet::my_editable(){

  if ( NULL != my_editable_ ){
    return my_editable_;
  }

  Editable *new_editable = new Editable(Bullet::my_blueprint());
 
  new_editable->set_father(Collider::my_editable());

  new_editable->set_id(id());
  // direction
  new_editable->addData(&direction_.x);
  new_editable->addData(&direction_.y);

  //speed
  new_editable->addData(&speed_);

  //alive
  new_editable->addData(&alive_);

  //alive
  new_editable->addData(&explosion_);

  //----------------
  new_editable->set_position(&rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);

  new_editable->set_object(this);

  my_editable_ = new_editable;

  return new_editable;

}

void Bullet::releaceEditable(){
  Collider::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Bullet::releaceBluePrint(){
  Collider::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}

void Bullet::callBack(int num_edit){



}

void* Bullet::cloneObject(){

  Bullet *newObj = new Bullet(*this);  

  return newObj;
}

void Bullet::set_direction(Vec2 newDirection){ direction_ = newDirection; }
void Bullet::set_direction(int new_direction_x, int new_direction_y){ direction_ = { (float)new_direction_x , (float)new_direction_y }; }
Vec2 Bullet::direction()const{ return direction_; }

