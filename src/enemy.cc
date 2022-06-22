//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/enemy.h"

#include "bullet.h"
#include "sprite_manager.h"

#include "blueprint.h"
#include "editable.h"

BluePrint* Enemy::s_my_blueprint_ = NULL;

Enemy::Enemy() : Collider(){

  engine_tag_ = EngineObjectTag::kEngineObjectTag_enemy;
  set_position(0, 0);
  set_rotation(0);
  set_scale(1, 1);
 
  my_editable_ = NULL;

  time_to_respawn_ = 1000;
  speed_ = 2;
  shoot_ = false;
  alive_ = true;
  direction_ = { 1.0f, 0.0f };

  list_spawn_points_ = (Vec2*)calloc(15, sizeof(Vec2) );
  
  num_spawn_points_ = 15;
  last_num_spawn_points_ = 15;

  SpriteManager::Instance().addSprite("disparo_03.png");
  bullet_ = new Bullet("disparo_03");
  
  bullet_->set_scale(20, 20);

  delay_time_ = 0;
  is_player_ = false;
}

Enemy::Enemy(const char *name) : Collider(name){

  my_editable_ = NULL;

  time_to_respawn_ = 1000;
  speed_ = 2;
  shoot_ = false;
  alive_ = true;
  direction_ = { 1.0f, 0.0f };

  list_spawn_points_ = (Vec2*)calloc(15, sizeof(Vec2));
  
  num_spawn_points_ = 15;
  last_num_spawn_points_ = 15;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_enemy;

  SpriteManager::Instance().addSprite("disparo_03.png");
  bullet_ = new Bullet("disparo_03");
  
  bullet_->set_scale(20, 20);
  delay_time_ = 0;
  is_player_ = false;
}

Enemy::Enemy(const Enemy& other) : Collider((Collider&)other){

  my_editable_ = NULL;

  time_to_respawn_ = 1000;
  speed_ = 2;
  shoot_ = false;
  alive_ = true;
  direction_ = { 1.0f, 0.0f };

  list_spawn_points_ = (Vec2*)calloc(15, sizeof(Vec2));
  
  for ( int i = 0; i < 15; i++ ){
    list_spawn_points_[i] = other.list_spawn_points_[i];
  }
  num_spawn_points_ = 15;
  last_num_spawn_points_ = 15;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_enemy;

  SpriteManager::Instance().addSprite("disparo_03.png");
  bullet_ = new Bullet("disparo_03");
  
  bullet_->set_scale(20, 20);
  delay_time_ = 0;
  is_player_ = other.is_player_;
}

Enemy::~Enemy(){

  if ( NULL != name_ ){
    
    free(name_);
    name_ = NULL;
  }

  if ( NULL != list_spawn_points_ ){
    
    free(list_spawn_points_);
    list_spawn_points_ = NULL;
  }

  
  releaceEditable();

  
  delete bullet_;

}


void Enemy::update(){

  bullet_->update();

  if ( !alive_ ){
    delay_time_ += SystemManager::Instance()->delta_time;    
  } else{
    
    rect_.x += (int)(direction_.x * speed_);
    rect_.y += (int)(direction_.y * speed_);

    if ( rect_.x < 0 || rect_.x + rect_.w > SystemManager::Instance()->kWindowW ){
      direction_.x *= -1;
    }

    if ( rect_.y < 0 || rect_.y + rect_.h > SystemManager::Instance()->kWindowH ){
      direction_.y *= -1;
    }

    
    if ( !bullet_->alive_ && !bullet_->explosion_ && !is_player_){
      Vec2 spawn_position = { (float)rect_.x, (float)rect_.y };
      bullet_->spawn(spawn_position, direction_);
    }

  }

}

void Enemy::draw(){

  if ( direction_.x < 0.0f ){
    rotation_ = 90;
  } else if(direction_.x > 0.0f){
    rotation_ = 270;
  } else if ( direction_.y > 0.0f ){
    rotation_ = 0;
  } else{
    rotation_ = 180;
  }

  if ( alive_ ){
    Collider::draw();
  }
  bullet_->draw();

}


void Enemy::spawn(){

  int random = rand() % (last_num_spawn_points_ - 1);

  rect_.x = (int)list_spawn_points_[random].x;
  rect_.y = (int)list_spawn_points_[random].y;

  random = rand() & 3;
  switch ( random ){
  case 0: direction_ = { 1,0 }; break;
  case 1: direction_ = { -1,0 }; break;
  case 2: direction_ = { 0,1 }; break;
  case 3: direction_ = { 0,-1 }; break;
  default:
    break;
  }

  alive_ = true; 
}

void Enemy::set_spawn_points(const Vec2 *points, int num){

  if ( num > 15 )num = 15;
  if ( num < 0 )return;
  
  for ( int i = 0; i < 15; i++ ){
    list_spawn_points_[i] = points[i];
  }

  
  num_spawn_points_ = num;
  last_num_spawn_points_ = num;

}


void Enemy::set_speed(int newValue){ speed_ = newValue; }
int  Enemy::speed()const{ return speed_; }

void Enemy::set_time_to_respawn(int newValue){ time_to_respawn_ = newValue; }
int  Enemy::TimeToRevive()const{ return time_to_respawn_; }

void Enemy::input(sInput *input_event){

  if ( is_player_){

    if ( input_event->keyboard_input == 'w' || input_event->keyboard_input == 'W' ){
      direction_ = { 0.0f, -1.0f };
    }
    if ( input_event->keyboard_input == 's' || input_event->keyboard_input == 'S' ){
      direction_ = { 0.0f, 1.0f };
    }
    if ( input_event->keyboard_input == 'a' || input_event->keyboard_input == 'A' ){
      direction_ = { -1.0f, 0.0f };
    }
    if ( input_event->keyboard_input == 'd' || input_event->keyboard_input == 'D' ){
      direction_ = { 1.0f, 0.0f };
    }

    if ( input_event->keyboard_input == ' '){
      Vec2 position = { (float)rect_.x, (float)rect_.y };
      bullet_->spawn(position, direction_);
    }

  }


}








BluePrint* Enemy::my_blueprint(){

  if ( NULL != s_my_blueprint_ ){
    return s_my_blueprint_;
  }

  BluePrint *new_blueprint = new BluePrint("Enemy");
  

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
  new_agroup = new_blueprint->createAgroup("Shooting");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Shoot");

  new_blueprint->addAgroup(new_agroup);
  //

  //
  new_agroup = new_blueprint->createAgroup("Alive");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Alive");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("IsPlayer");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, false);
  new_blueprint->addData(new_agroup, "Is Player");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Bullet Alive");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Alive");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Bullet Position");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Float);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "X");
  new_blueprint->addData(new_agroup, "Y");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Bullet direction");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Float);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "X");
  new_blueprint->addData(new_agroup, "Y");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Bullet speed");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "speed");

  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Bullet Explocion");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Bool);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Explocion");

  new_blueprint->addAgroup(new_agroup);

  //SpawnPoints
  new_agroup = new_blueprint->createAgroup("Number SpawnPoints");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int);
  new_blueprint->set_editable(new_agroup, false);
  new_blueprint->addData(new_agroup, "Total");

  new_blueprint->addAgroup(new_agroup);
  // 
  //
  for ( int i = 0; i < 15; i++ ){

    new_agroup = new_blueprint->createAgroup(( "SpawnPoint" + std::to_string(i)).c_str());
    new_blueprint->set_my_data_type(new_agroup, kDataType_Float);
    new_blueprint->set_editable(new_agroup, true);
    new_blueprint->addData(new_agroup, "X");
    new_blueprint->addData(new_agroup, "Y");

    new_blueprint->addAgroup(new_agroup);

  }

  if ( NULL == s_my_blueprint_ ){
    s_my_blueprint_ = new_blueprint;
  }

  return s_my_blueprint_;

}

Editable* Enemy::my_editable(){

  if ( NULL != my_editable_ ){
    return my_editable_;
  }

  Editable *new_editable = new Editable(Enemy::my_blueprint());
  

  new_editable->set_father(Collider::my_editable());

  new_editable->set_id(id());
  // direction
  new_editable->addData(&direction_.x);
  new_editable->addData(&direction_.y);
  //speed
  new_editable->addData(&speed_);

  //Shoot
  new_editable->addData(&shoot_);

  //alive
  new_editable->addData(&alive_);

  //Is Player
  new_editable->addData(&is_player_);

  //Bullet alive
  new_editable->addData(&bullet_->alive_);
  //Bullet position
  new_editable->addData(&bullet_->rect_.x);
  new_editable->addData(&bullet_->rect_.y);
  //Bullet direction
  new_editable->addData(&bullet_->direction_.x);
  new_editable->addData(&bullet_->direction_.y);

  //Bullet speed
  new_editable->addData(&bullet_->speed_);

  //Bullet explocion
  new_editable->addData(&bullet_->explosion_);

  //numSpawns
  new_editable->addData(&num_spawn_points_);

  for ( int i = 0; i < 15; i++ ){

    new_editable->addData(&list_spawn_points_[i].x);
    new_editable->addData(&list_spawn_points_[i].y);
  }

  //----------------
  new_editable->set_position(&rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);

  new_editable->set_object(this);

  my_editable_ = new_editable;

  return new_editable;

}

void Enemy::releaceEditable(){
  Collider::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Enemy::releaceBluePrint(){
  Collider::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}

void Enemy::callBack(int num_edit){}

void* Enemy::cloneObject(){

  Enemy *newObj = new Enemy(*this);
  

  return newObj;
}

void Enemy::set_direction(Vec2 newDirection){  direction_ = newDirection;}
void Enemy::set_direction(int new_direction_x, int new_direction_y){ direction_ = { (float)new_direction_x , (float)new_direction_y };}
Vec2 Enemy::direction()const{ return direction_; }