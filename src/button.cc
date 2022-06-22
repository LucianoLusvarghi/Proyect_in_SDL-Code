//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#include "../include/button.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "text.h"
#include "lsprite.h"

#include "blueprint.h"
#include "editable.h"


BluePrint* Button::s_my_blueprint_ = NULL;

Button::Button() : Rect(){
  
  text_ = new Text();
 
  sprite_ = NULL;
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_button;
}

Button::Button(const char *name) : Rect(name){

  text_ = new Text();
  
  sprite_ = NULL;
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_button;
}

Button::Button(Button &other) : Rect(other){
  
  text_ = new Text();
  
  sprite_ = NULL;
  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_button;
}

Button::~Button(){
  
  delete text_;
  if(NULL != sprite_ ){
    
    delete sprite_;
  }

  
  releaceEditable();

}

bool Button::isPressed(short mouse_x, short mouse_y){

  SDL_Point windows_point = { mouse_x, mouse_y };
  SDL_Rect clip_rect = rect_;
  SDL_Rect result_rect;

  bool to_return = false;

  if ( SDL_EnclosePoints(&windows_point, 1, &clip_rect, &result_rect) ){
    to_return = true;
  }

  return to_return;  

}


void Button::loadImg(const char *image_name){

  sprite_ = new Lsprite();
 
  sprite_->set_name(image_name);
  sprite_->callSpriteManager();  
  
  sprite_->set_position(rect_.x + 10, rect_.y + 20);
  
  sprite_->set_scale(rect_.w - 20, rect_.h - 40);

}

void Button::draw(){

  Rect::draw();
  text_->draw();

  if ( NULL != sprite_ ){
    sprite_->draw();
  }
  
}

void Button::set_text(const char *font, const char *text, int size, const SDL_Color& color){

  text_->changeFont(font);
  text_->set_text(text);
  text_->changeSize(size);
  text_->set_color(color);

}

void Button::changeText(const char *text){
  text_->set_text(text);
}

void Button::changeFont(const char *font){
  text_->changeFont(font);
}
void Button::changeColorText(unsigned char R, unsigned char G, unsigned char B, unsigned char A){
  text_->set_color(R,G,B,A);
}
void Button::changeColorText(const SDL_Color& colorRGBA){
  text_->set_color(colorRGBA);
}

BluePrint* Button::my_blueprint(){return NULL;}

Editable* Button::my_editable(){return NULL;}

void Button::callBack(int num_edit){}

void* Button::cloneObject(){return NULL;}

void Button::set_position(const SDL_Point& new_position){
  Rect::set_position(new_position);

  text_->set_position(new_position.x + 10, new_position.y + 10);  
  if ( NULL != sprite_ ){
    sprite_->set_position(new_position.x + 10, new_position.y + 10);
  }
}

void Button::set_position(int x, int y) {
  Rect::set_position(x,y);
  text_->set_position(x + 10, y + 10);
  if ( NULL != sprite_ ){
    sprite_->set_position(x + 10, y +10);
  }
}

void Button::set_rotation(int new_rotation){
  Rect::set_rotation(new_rotation);
  text_->set_rotation(new_rotation);
  if ( NULL != sprite_ ){
    sprite_->set_rotation(new_rotation);
  }
}

void Button::set_scale(const SDL_Point& new_scale){
  Rect::set_scale(new_scale);
  
  
  text_->set_scale(new_scale.x - 20, new_scale.y - 40);

  if ( NULL != sprite_ ){
    sprite_->set_scale(new_scale.x - 20, new_scale.y - 40);
  }

}

void Button::set_scale(int w, int h){
  Rect::set_scale(w,h);
  
  text_->set_scale(w - 20, h - 40);

  if ( NULL != sprite_ ){
    sprite_->set_scale(w - 20, h - 40);
  }
}

void Button::releaceEditable(){
  Rect::releaceEditable();
  if ( my_editable_ != NULL ){
    delete my_editable_;
  }
  my_editable_ = NULL;
}

void Button::releaceBluePrint(){
  Rect::releaceBluePrint();
  if ( s_my_blueprint_ != NULL ){
    delete s_my_blueprint_;
  }
  s_my_blueprint_ = NULL;
}
