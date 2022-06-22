//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#include "../include/inputField.h"

#include "blueprint.h"
#include "editable.h"
#include "text.h"

BluePrint* InputField::s_my_blueprint_ = NULL;

InputField::InputField() : Rect(){

  text_ = new Text("Enter Text");
  background_ = new Rect();
  background_->set_color(240, 240, 240);

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_InputField;

  input_ = (char*)calloc(21, sizeof(char));
  
  input_max_lenght_ = 20;
  is_selected_ = false;
  default_text_ = (char*)calloc(11, sizeof(char));
  
  memcpy(default_text_, "Enter Text", 11 * sizeof(char));
}

InputField::InputField(const char *name) : Rect(name){

  text_ = new Text("Enter Text");
  background_ = new Rect();
  background_->set_color(240, 240, 240);

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_InputField;

  input_ = (char*)calloc(21, sizeof(char));
  
  input_max_lenght_ = 20;
  is_selected_ = false;
  default_text_ = (char*)calloc(11, sizeof(char));
  
  memcpy(default_text_, "Enter Text", 11 * sizeof(char));
}

InputField::InputField(InputField &other) : Rect(other){

  text_ = new Text("Enter Text");
  background_ = new Rect();
  background_->set_color(240, 240, 240);

  my_editable_ = NULL;
  engine_tag_ = EngineObjectTag::kEngineObjectTag_InputField;
  input_ = (char*)calloc(21, sizeof(char));
  
  input_max_lenght_ = 20;
  is_selected_ = false;
  default_text_ = (char*)calloc(11, sizeof(char));
  
  memcpy(default_text_, "Enter Text", 11 * sizeof(char));
}

InputField::~InputField(){
  
  free(input_);
  free(default_text_);
    
  delete text_;  
  delete background_;
  
  releaceEditable();
}

bool InputField::isPressed(short mouse_x, short mouse_y){

  SDL_Point windows_point = { mouse_x, mouse_y };
  SDL_Rect rect_clip = rect_;
  SDL_Rect rect_result;

  bool to_return = false;

  if ( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) ){
    to_return = true;
    if ( strlen(input_) == 0 ){
      changeText(" ");
    }
  } else{
    if ( strlen(input_) == 0 ){
      changeText(default_text_);
    }
  }
  
  is_selected_ = to_return;
  return to_return;

}

void InputField::changeDefaultText(const char* newDefaultText){
  int lenght = strlen(newDefaultText);
  char *new_default_text = (char*)calloc(lenght+1, sizeof(char));
  
  if ( NULL != new_default_text ){
    
    free(default_text_);
    default_text_ = new_default_text;
    memcpy(default_text_, newDefaultText, lenght * sizeof(char));
  }
}

void InputField::draw(){

  Rect::draw();
  background_->draw();
  text_->draw();
  
  if ( is_selected_ ){
    unsigned int timer = SDL_GetTicks();
    if ( (SystemManager::Instance()->last_time & 1000) > 500 ){
      SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, 255);
      SDL_RenderDrawLine(SystemManager::Instance()->Main_render_, rect_.x + 10 + text_->rect_.w, rect_.y + 5, rect_.x + 10 + text_->rect_.w, rect_.y + 5 + text_->rect_.h);

    }

  }

}

void InputField::set_text(const char *font, const char *text, int size, const SDL_Color& color){

  text_->changeFont(font);
  text_->set_text(text);
  text_->changeSize(size);
  text_->set_color(color);

}

void InputField::changeText(const char *text){
  text_->set_text(text);
}

void InputField::changeFont(const char *font){
  text_->changeFont(font);
}
void InputField::changeColorText(unsigned char R, unsigned char G, unsigned char B, unsigned char A){
  text_->set_color(R, G, B, A);
}
void InputField::changeColorText(const SDL_Color& colorRGBA){
  text_->set_color(colorRGBA);
}

BluePrint* InputField::my_blueprint(){ return NULL; }

Editable* InputField::my_editable(){ return NULL; }

void InputField::callBack(int num_edit){}

void* InputField::cloneObject(){ return NULL; }

void InputField::set_position(const SDL_Point& new_position){
  Rect::set_position(new_position);

  SDL_Point internal = { new_position.x + 10, new_position.y + 10 };

  background_->set_position(internal);
  text_->set_position(internal);

}

void InputField::set_position(int x, int y){
  Rect::set_position(x, y);
  text_->set_position(x + 10, y + 10);
  background_->set_position(x + 10, y + 10);
}

void InputField::set_rotation(int new_rotation){}

void InputField::set_scale(const SDL_Point& new_scale){
  Rect::set_scale(new_scale);

  SDL_Point internal = { new_scale.x - 20, new_scale.y - 40 };
  text_->set_scale(internal);
  internal.y += 20;
  background_->set_scale(internal);
}
void InputField::set_scale(int w, int h){
  Rect::set_scale(w, h);
  text_->set_scale(w - 20, h - 40);
  background_->set_scale(w - 20, h - 20);
}

void InputField::input(sInput *input_event){

  if ( input_event->keyboard_input != 0 && is_selected_ && input_event->keyboard_input  != 27 && input_event->keyboard_input != 17 ){
    unsigned int lenght = strlen(input_);

    if( input_event->keyboard_input != 8 && input_event->keyboard_input != 127 ){

      if ( lenght + 1 > input_max_lenght_ ){        
          return;
      }

      if ( input_event->keyboard_input >= 32 && input_event->keyboard_input <= 126 ){

        input_[lenght] = input_event->keyboard_input;
        changeText(input_);
      }
    } else{
      if(lenght > 0 ){
        input_[lenght-1] = 0;      
      
        if ( lenght - 1 == 0 ){
          changeText(default_text_);
        } else{
          changeText(input_);
        }
      } else{
        changeText(" ");
      }
    }

  }

}

bool InputField::is_selected()const{return is_selected_;}

const char* InputField::input()const{return input_;}

void InputField::changeMaxInput(unsigned int newValue){

  char* newInput = (char*)calloc(newValue + 1, sizeof(char));
  
  if ( newInput != NULL ){

    if ( newValue > input_max_lenght_ ){
      memcpy(newInput, input_, input_max_lenght_ * sizeof(char));
    } else{
      memcpy(newInput, input_, newValue * sizeof(char));
    }    
    
    
    free(input_);
    input_ = newInput;
    input_max_lenght_ = newValue;
  }

}

void InputField::releaceEditable(){
  Rect::releaceEditable();
  if ( my_editable_ != NULL ){
    delete my_editable_;
  }
  my_editable_ = NULL;
}

void InputField::releaceBluePrint(){
  Rect::releaceBluePrint();
  if ( s_my_blueprint_ != NULL ){
    delete s_my_blueprint_;
  }
  s_my_blueprint_ = NULL;
}