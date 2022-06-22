//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/sprite_manager.h"

#include "System_manager.h"
#include "blueprint.h"
#include "editable.h"


#pragma warning( disable : 4996 )

static SpriteManager *s_instance_ = NULL;

SpriteManager& SpriteManager::Instance(){
    
  if ( NULL == s_instance_ ){ s_instance_ = new SpriteManager();  }
  
  return *s_instance_;
}

SpriteManager::SpriteManager(){
  
  list_sprites_ = NULL;
  sprites_name_ = NULL;
  sprites_dir_ = NULL;
  num_sprites_ = 0; 

  s_my_blueprint_ = NULL;
  my_editable_ = NULL;
  
  new_sprite_ = NULL;
  
  set_my_name("SpriteManager");
}

SpriteManager::~SpriteManager(){
  
  for(int i=0;i<num_sprites_;i++){   
    
    free(sprites_name_[i]);

    
    free(sprites_dir_[i]);

   
    SDL_DestroyTexture(list_sprites_[i]);
  }
  
 
  free(sprites_name_);

  
  free(sprites_dir_);

  
  free(list_sprites_);
  
  s_instance_ = NULL;
  
}

SpriteManager::SpriteManager(const SpriteManager& other){}

void SpriteManager::addSprite(const char *file_dir){
  
  std::string sprite_dir;
  
  char buffer[20];
  sprintf( buffer , "%.*s", strlen(file_dir) - 4, file_dir );
  
  if( NULL != getSprite(buffer) ){
    return;
  }
  
 
  
  sprite_dir = "../data/sprites/" + sprite_dir;
  sprite_dir.append(file_dir);

  SDL_Surface * image = IMG_Load(sprite_dir.c_str());
  
  if(image == NULL){
    printf("Error al cargar imagen %s \n SDL-image error %s\n", file_dir, IMG_GetError());
    return;
  }
  
  num_sprites_++;

  SDL_Texture **newSprites = (SDL_Texture**)calloc(num_sprites_, sizeof(SDL_Texture*));
  

  if ( newSprites != NULL ){
    memcpy(newSprites, list_sprites_, (num_sprites_ - 1) * sizeof(SDL_Texture*));

    
    free(list_sprites_);
    list_sprites_ = newSprites;
  }

  char **newSpritesName = (char**)calloc(num_sprites_, sizeof(char*));
  

  if ( newSpritesName != NULL ){
    memcpy(newSpritesName, sprites_name_, (num_sprites_ - 1) * sizeof(char*));

    
    free(sprites_name_);
    sprites_name_ = newSpritesName;
  }
  
  char **newSpriteDir = (char**)calloc(num_sprites_, sizeof(char*));
  

  if ( newSpriteDir != NULL ){
    memcpy(newSpriteDir, sprites_dir_, (num_sprites_ - 1) * sizeof(char*));

    
    free(sprites_dir_);
    sprites_dir_ = newSpriteDir;
  }



  sprites_dir_[num_sprites_-1] = (char*)calloc( strlen(file_dir)+1, sizeof(char) );
  

  strcpy( sprites_dir_[num_sprites_-1], file_dir);
  
  int dirLen = strlen(file_dir);
  
  std::string sprite_name = "";
  
  bool po = false;
  bool end = false;
  for(int i= dirLen-1; i >=0 && !end; i--){
    
    if(file_dir[i] == '/' || file_dir[i] == 92){
      po = false;
      end = true;
    }
    
    if(po){
      
      sprite_name += file_dir[i];
      
    }
    
    if( file_dir[i] == '.'){
      po = true;
    }
    
    
    
  }
  dirLen = sprite_name.length();
  
  sprites_name_[num_sprites_ - 1] = (char*) calloc( dirLen + 1 , sizeof(char) );
  

  for(int i=0;i<dirLen;i++){
    
    (sprites_name_[num_sprites_ - 1])[i] = sprite_name[dirLen-1-i];
    
  }
    
  list_sprites_[num_sprites_ - 1] = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, image);
  

  if( NULL != s_my_blueprint_ ){
    
    AGroup* new_agroup = s_my_blueprint_->createAgroup("Nombre");
    s_my_blueprint_->set_my_data_type(new_agroup, kDataType_String );
    s_my_blueprint_->set_editable(new_agroup, false );
    s_my_blueprint_->addData(new_agroup, "Nombre" );
    
    s_my_blueprint_->addAgroup(new_agroup);
    
    //-------------------------------------
    new_agroup = s_my_blueprint_->createAgroup("IMG");
    s_my_blueprint_->set_my_data_type(new_agroup, kDataType_IMG );
    s_my_blueprint_->set_editable(new_agroup, false );
    s_my_blueprint_->addData(new_agroup, "IMG" );
    
    s_my_blueprint_->addAgroup(new_agroup);
    
  }
  
  if( NULL != my_editable_ ){
    
    for ( int i = 0; i < num_sprites_ - 1; i++ ){

      my_editable_->list_data_[(i*2) + 2] = &sprites_name_[i];
      my_editable_->list_data_[(i*2) + 3] = &list_sprites_[i];
    }

    my_editable_->addData( &sprites_name_[num_sprites_ - 1] ); 
    my_editable_->addData( &list_sprites_[num_sprites_ - 1] ); 
    
  }
  
  
}

SDL_Texture* SpriteManager::getSprite(const char *sprite_name){
  
  for(int i=0;i<num_sprites_;i++){
    
    if( strcmp(sprite_name, sprites_name_[i]) == 0){
      
      return list_sprites_[i];
      
    }    
    
  }
  
  return NULL;
  
}

SDL_Texture* SpriteManager::getSprite(int indice){
  
  return list_sprites_[indice]; 
  
}

char* SpriteManager::getSpriteName(int indice){
  
  return sprites_name_[indice];
  
}


int SpriteManager::getSpriteIndex(const char *sprite_name){
  
  for(int i=0;i<num_sprites_;i++){
    
    if( strcmp(sprite_name, sprites_name_[i]) == 0){
      
      return i;
      
    }    
    
  }  
  
  return -1;
  
}


BluePrint* SpriteManager::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("SpriteManager");
  

  new_blueprint->set_father( NULL );
  
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Total Sprites");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, false );
  new_blueprint->addData(new_agroup, "Total Sprites" );
  
  new_blueprint->addAgroup(new_agroup);
  
  //
  new_agroup = new_blueprint->createAgroup("Agregar: ");
  new_blueprint->set_my_data_type(new_agroup, kDataType_String );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Agregar: " );
  
  new_blueprint->addAgroup(new_agroup);
  
  //
  for(int i=0;i<num_sprites_;i++){
    
    //
    new_agroup = new_blueprint->createAgroup("Nombre");
    new_blueprint->set_my_data_type(new_agroup, kDataType_String );
    new_blueprint->set_editable(new_agroup, false );
    new_blueprint->addData(new_agroup, "Nombre" );
    
    new_blueprint->addAgroup(new_agroup);
    //
    new_agroup = new_blueprint->createAgroup("IMG");
    new_blueprint->set_my_data_type(new_agroup, kDataType_IMG );
    new_blueprint->set_editable(new_agroup, false );
    new_blueprint->addData(new_agroup, "IMG" );

    new_blueprint->addAgroup(new_agroup);
        
  } 
    
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 

Editable* SpriteManager::my_editable(){
  
  if( NULL != my_editable_ ){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(SpriteManager::my_blueprint());
  

  new_editable->set_father( NULL );
    
  //
  new_editable->addData( &num_sprites_ );
  //
  new_editable->addData( &new_sprite_ );
  //
  for( int i=0;i<num_sprites_;i++ ){
    
    new_editable->addData( &sprites_name_[i] );
    new_editable->addData( &list_sprites_[i] );
    
  }
  
  //
  new_editable->set_position( NULL , NULL);
  new_editable->set_dimension(NULL, NULL);
  new_editable->set_rotation(NULL);
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
  
  
}

void SpriteManager::callBack(int num_edit){
  
  if(NULL != new_sprite_){
    addSprite(new_sprite_);
    
    free(new_sprite_);
    new_sprite_ = NULL;
  }
  
}

void SpriteManager::releaceEditable(){
  Entity::releaceEditable();

  delete my_editable_;
  my_editable_ = NULL;
}

void SpriteManager::releaceBluePrint(){
  Entity::releaceBluePrint();

  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}
