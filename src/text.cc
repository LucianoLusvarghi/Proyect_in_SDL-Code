//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/text.h"

#include "System_manager.h"
#include "blueprint.h"
#include "editable.h"

#include <SDL_ttf.h>

BluePrint* Text::s_my_blueprint_ = NULL;

Text::Text() : Rect(){
  
  font_name_ = NULL;
  font_ = NULL;
  text_ = NULL;
  texture_text_ = NULL;
  size_ = 20;
  color_ = {255,255,255};
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::Text(const Text& other) : Rect( (Rect&)other ) {
  
  if( other.font_name() != NULL ){
    font_name_ = (char*)calloc( strlen(other.font_name())+1, sizeof(char) );  
    
    memcpy(font_name_, other.font_name(), strlen(other.font_name()));
  }else{
    font_name_ = NULL;
  }
  size_ = other.size();
  
  if(font_name_ != NULL){
    std::string ttf_dir = "../data/fonts/";
    ttf_dir += font_name_;
    font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
    if(NULL == font_){      
      printf("NO se pudo cargar el archvo ttf - %s\n", TTF_GetError());      
    }

  }else{
    font_ = NULL;
  }
  
  if( other.text() != NULL ){
  
    text_ = (char*)calloc( strlen(other.text())+1, sizeof(char) );  
    
    memcpy(font_name_, other.text(), strlen(other.text()));
  
  }else{
    text_ = NULL;
  }
  color_ = other.color_;
  
  if( font_ != NULL && text_ != NULL ){  
    SDL_Surface * Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
  }else{
    texture_text_ = NULL;    
  }
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::Text(const char *name):Rect(name){
  
  font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
  
  memcpy(font_name_, name, strlen(name));
  
  size_ = 20;

  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
  
 
  
  color_ = {255,255,255};
  text_ = NULL;
  texture_text_ = NULL;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::Text(const char *name, int size):Rect(name){
  
  font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) ); 
  
  memcpy(font_name_, name, strlen(name));
  
  size_ = size;
  
  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
  
  if(NULL == font_){    
    printf("NO se pudo cargar el archvo ttf - %s\n", TTF_GetError());
  }
  
  color_ = {255,255,255};
  text_ = NULL;
  texture_text_ = NULL;
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::Text(const char *name, int size, const char *text):Rect(name){
  
  font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
  
  memcpy(font_name_, name, strlen(name));
  
  size_ = size;
  
  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
  if(NULL == font_){    
    printf("NO se pudo cargar el archvo ttf - %s\n", TTF_GetError());
  }
  text_ = (char*)calloc( strlen(text)+1, sizeof(char) ); 
  
  memcpy(text_ , text, strlen(text));
  
  color_ = {255,255,255};
  
  SDL_Surface * Dw_text = TTF_RenderText_Solid(font_, text_, color_);
  texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
  SDL_FreeSurface(Dw_text);
  SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
  
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::Text(const char *name, int size, const char *text, const SDL_Color& color):Rect(name){
  
  font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
 
  memcpy(font_name_, name, strlen(name));
  
  size_ = size;
  
  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
  if(NULL == font_){    
    printf("NO se pudo cargar el archvo ttf - %s\n", TTF_GetError());
  }
  text_ = (char*)calloc( strlen(text)+1, sizeof(char) );  
  
  memcpy(text_, text, strlen(text));
  
  color_ = color;
  
  SDL_Surface * Dw_text = TTF_RenderText_Solid(font_, text_, color_);
  texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
  SDL_FreeSurface(Dw_text);
  SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
 
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_text;

}

Text::~Text(){
  
  if ( NULL != texture_text_ ){
    SDL_DestroyTexture(texture_text_);
    texture_text_ = NULL;
  }

  if ( NULL != font_ ){
    TTF_CloseFont(font_);
  }

  if ( NULL != font_name_ ){
    free(font_name_);
  }
  
  if ( NULL != text_ ){
    free(text_);
  }
  
  releaceEditable();
  my_editable_ = NULL;
  
}

void Text::changeFont(const char *name){
  
  if(NULL != font_name_){
    
    char* new_font_name = font_name_;
    
    font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) ); 
    
    if( NULL != font_name_ ){
      memcpy(font_name_, name, strlen(name));
      
      free(new_font_name);
    }else{
      font_name_ = new_font_name;
    }
    
    
  }else{
    
    font_name_ = (char*)calloc( strlen(name)+1, sizeof(char) );  
    
    memcpy(font_name_, name, strlen(name));
    
  }
  
  
  if(NULL != font_){
    TTF_CloseFont(font_);
  }
  
  
  
  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
  if(NULL == font_){    
    printf("NO se pudo cargar el archvo ttf - %s\n", TTF_GetError());
  }

  if( NULL != text_ && NULL != font_){
    
    if(NULL != texture_text_){
      SDL_DestroyTexture(texture_text_);
      texture_text_ = NULL;
    }
    
    SDL_Surface *Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);    
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
        
  }  
  
}

const char* Text::font_name()const{return font_name_;}
  
SDL_Color Text::GetColor()const{return color_;}

void Text::set_text(const char *text){
  
  if(NULL != text_){
    
    char *new_text = text_;
    
    text_ = (char*)calloc( strlen(text)+1, sizeof(char) );
    
    if(NULL != text_){    
      memcpy(text_, text, strlen(text));
      
      free(new_text);
    }else{
      
      text_ = new_text;
      
    }    
    
  }else{
    
    text_ = (char*)calloc( strlen(text)+1, sizeof(char) );
    
    if(NULL != text_){    
      memcpy(text_, text, strlen(text));     
    }
    
  }
  
  
  
  if(NULL != font_){
    
    if(NULL != texture_text_){
      SDL_DestroyTexture(texture_text_);
      texture_text_ = NULL;
    }
    
    SDL_Surface *Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
  }
  
  
}

const char* Text::text()const{return text_;}

void Text::draw(){
  
  if(NULL != texture_text_){    
    SDL_Point center = { (rect_.w >> 1), (rect_.h >> 1) };
    SDL_RenderCopyEx(SystemManager::Instance()->Main_render_, texture_text_, NULL, &rect_, rotation_, &center, SDL_FLIP_NONE);
  }
}
  
void Text::changeSize(int newSize){
    
  TTF_CloseFont(font_);  
  size_ = newSize;  
  std::string ttf_dir = "../data/fonts/";
  ttf_dir += font_name_;
  font_ = TTF_OpenFont(ttf_dir.c_str(), size_);
      
  if(NULL != text_ && NULL != font_){
    
    if(NULL != texture_text_){
      SDL_DestroyTexture(texture_text_);
      texture_text_ = NULL;
    }
    
    SDL_Surface *Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);
  }
  
}

int Text::size()const{return size_;}

void Text::set_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A){
  
  Rect::set_color(R,G,B,A);
  
  if(NULL != text_ && NULL != font_){
    
    if(NULL != texture_text_){
      SDL_DestroyTexture(texture_text_);
      texture_text_ = NULL;
    }
    
    SDL_Surface *Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);   
    
  }
  
  
}

void Text::set_color(const SDL_Color& colorRGBA){
  
  Rect::set_color(colorRGBA);
  
  if(NULL != text_ && NULL != font_){
    
    if(NULL != texture_text_){
      SDL_DestroyTexture(texture_text_);
      texture_text_ = NULL;
    }
    
    SDL_Surface *Dw_text = TTF_RenderText_Solid(font_, text_, color_);
    texture_text_ = SDL_CreateTextureFromSurface(SystemManager::Instance()->Main_render_, Dw_text);
    SDL_FreeSurface(Dw_text);
    SDL_QueryTexture(texture_text_, NULL, NULL, &rect_.w, &rect_.h);   
    
  }  
  
}

int Text::height(){return rect_.h;}

int Text::width(){return rect_.w;}

BluePrint* Text::my_blueprint(){
  
  if(NULL != s_my_blueprint_){
    return s_my_blueprint_;
  }
  
  BluePrint *new_blueprint = new BluePrint("Text");
  

  new_blueprint->set_father( Rect::my_blueprint() );
  
  //
  AGroup* new_agroup = new_blueprint->createAgroup("Texto");
  new_blueprint->set_my_data_type(new_agroup, kDataType_String );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Texto" );
  
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Rotacion");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int);
  new_blueprint->set_editable(new_agroup, true);
  new_blueprint->addData(new_agroup, "Ang");
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Fuente");
  new_blueprint->set_my_data_type(new_agroup, kDataType_String );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Font" );
  
  new_blueprint->addAgroup(new_agroup);
  //
  new_agroup = new_blueprint->createAgroup("Size");
  new_blueprint->set_my_data_type(new_agroup, kDataType_Int );
  new_blueprint->set_editable(new_agroup, true );
  new_blueprint->addData(new_agroup, "Size" );

  new_blueprint->addAgroup(new_agroup);
  
  if(NULL == s_my_blueprint_){
    s_my_blueprint_ = new_blueprint;
  }
  
  return s_my_blueprint_;
  
} 
 
Editable* Text::my_editable(){
  
  if( NULL != my_editable_ ){
    return my_editable_;
  }
  
  Editable *new_editable = new Editable(Text::my_blueprint());
  

  new_editable->set_father( Rect::my_editable() );
  
  new_editable->set_id( id() );
  //
  new_editable->addData( &text_ );
  //
  new_editable->addData(&rotation_);
  //
  new_editable->addData( &font_name_ );
  //
  new_editable->addData( &size_ );
  
  //
  new_editable->set_position( &rect_.x, &rect_.y);
  new_editable->set_dimension(&rect_.w, &rect_.h);
  new_editable->set_rotation(&rotation_);
  
  new_editable->set_object(this);
  
  my_editable_ = new_editable;
  
  return new_editable;
  
}

void Text::callBack(int num_edit){
  
  
  num_edit -= 13;
  
  SDL_Rect backup_rect;
  bool diferente = false;
  SDL_QueryTexture(texture_text_, NULL, NULL, &backup_rect.w, &backup_rect.h);
    
  if( rect_.w != backup_rect.w || rect_.h != backup_rect.h){
    backup_rect.w = rect_.w;
    backup_rect.h = rect_.h;
    diferente = true;
  }
  
  set_color(color_);
  
  switch(num_edit){
    
    case 0:{
      set_text(text_);
      break;
    }
    case 1:{
      changeFont( font_name_ );
      break;
    }
    case 2:{
      changeSize(size_);
      diferente = false;
      break;
    }
    default:{
      
      break;
    }
    
    
  }
  
  if(diferente){
    rect_.w = backup_rect.w;
    rect_.h = backup_rect.h;
  }
    
  
}

void* Text::cloneObject(){
  
  Text *new_obj = new Text(*this);
  
  return new_obj;
   
}

void Text::releaceEditable(){
  Rect::releaceEditable();
  
  delete my_editable_;
  my_editable_ = NULL;
}

void Text::releaceBluePrint(){
  Rect::releaceBluePrint();
  
  delete s_my_blueprint_;
  s_my_blueprint_ = NULL;
}