//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/editor.h"

#include <stdint.h>
#include <string>

#include "editable.h"
#include "sprite_manager.h"
#include "text.h"
#include "entity.h"
#include "game_manager.h"
#include "../deps/Lusvarghi_lib/Include/my_lib.h"

#include "scene.h"

#pragma warning( disable : 4996 )


Editor::Editor(){
  
  num_blueprint_ = 0;
  li_blueprint_ = NULL;
  
  num_editables_ = 0;
  li_editables_ = NULL;
  
  
  
  selected_ = NULL;
  
  text_draw_ = new Text();
  

  text_draw_->changeFont( "arial.ttf" );
  text_draw_->changeSize(20);
  text_draw_->set_color(0,0,0);
  
  edition_data_ = NULL;
  edition_obj_ = NULL;
  hitbox_text_ = {0,0,0,0};
  buffer_text_[0] = '\0';
  edition_index_ = 0;
 
  scroll_y_ = 30;
  cont_editable_ = 0;
  
  display_list_ = false;
  my_Scene_ = NULL;
}

Editor::Editor(const Editor& other){}

Editor::~Editor(){
  
  if(NULL != li_blueprint_){
    
    for(int i=0; i<num_blueprint_; i++ ){      
      li_blueprint_[i] = NULL;      
    }
    
    free(li_blueprint_);
    li_blueprint_ = NULL;
    
  }
  
  if(NULL != li_editables_){
    
    for(int i=0; i<num_editables_; i++ ){
      
      ((Entity*)(li_editables_[i]->object_))->releaceEditable();      
      li_editables_[i] = NULL;
    }
    
    free( li_editables_);
    li_editables_ = NULL;
    
  }
  
  
  selected_ = NULL;
  
  
  delete text_draw_;
}
  
void Editor::addBlueprint( BluePrint *new_blueprint ){
  
  if(NULL == new_blueprint){
    return;
  }
  
  for(int i=0;i<num_blueprint_;i++){
    
    if( li_blueprint_[i] == new_blueprint){
      return;
    }
    
  }
  
  num_blueprint_++;

  BluePrint **new_list_blueprint = (BluePrint**)calloc(num_blueprint_, sizeof(BluePrint*));
  

  if ( nullptr != new_list_blueprint){
    memcpy(new_list_blueprint, li_blueprint_, (num_blueprint_ - 1) * sizeof(BluePrint*));

    
    free(li_blueprint_);
    li_blueprint_ = new_list_blueprint;
  }

  li_blueprint_[num_blueprint_-1] = new_blueprint;
    
}
   
void Editor::addEditable( Editable *new_editable ){
  
  if(NULL == new_editable){
    return;
  }
  
  for(int i=0;i<num_editables_;i++){
    
    if(li_editables_[i] == new_editable){
      return;
    }
    
  }
  
  bool exists = false;
  for(int i=0;i<num_blueprint_ && !exists;i++){
    
    if( new_editable->my_blueprint_ == li_blueprint_[i] ){
      exists = true;
    }    
  }
  
  if(!exists){
    return;
  }
  
  num_editables_++;

  Editable** new_list_editables = (Editable**)calloc(num_editables_, sizeof(Editable*));
  

  if ( nullptr != new_list_editables ){
    memcpy(new_list_editables, li_editables_, (num_editables_ - 1) * sizeof(Editable*));

    
    free(li_editables_);
    li_editables_ = new_list_editables;
  }

  li_editables_[num_editables_-1] = new_editable;
  
}

void Editor::lessEditable( Editable *old_editable ){
  
  if(NULL == old_editable){
    return;
  }
  
  
  for(int i=0;i<num_editables_;i++){
    
    if( li_editables_[i] == old_editable ){
            
      Editable **new_list_editables = (Editable**)calloc((num_editables_ -1) , sizeof(Editable*) );
      

      int li = 0;
      for(int j=0;j<num_editables_;j++){
        
        if( j != i ){
          new_list_editables[li] = li_editables_[j];
          li++;
        }
        
      }
      
      free(li_editables_);
      
      li_editables_ = new_list_editables;
      num_editables_--;
           
      return;
    }  
    
  }
  
}
    
void Editor::searchInWindows( int x, int y){
  
  if(x < kEditorWidth){
    
    searchInEditor(x, y);
    return;
  }
  
  edition_data_ = NULL;
  edition_obj_ = NULL;
  
  Mat3 transform;
  Mat3 translation;
  Mat3 rotation;
  Mat3 scale;
  Vec2 pointsInit[4];
  Vec2 pointsEnd[4];
   
  bool exists = false;
  
  
  for(int i=0; i< num_editables_; i++){
    
    if( NULL != li_editables_[i]->pos_x_ && NULL != li_editables_[i]->pos_y_ && NULL != li_editables_[i]->width_ && NULL != li_editables_[i]->height_){
      
      pointsInit[0].x = -1.0f;
      pointsInit[0].y = -1.0f;
      
      pointsInit[1].x = 1.0f;
      pointsInit[1].y =-1.0f;
      
      pointsInit[2].x = 1.0f;
      pointsInit[2].y = 1.0f;
      
      pointsInit[3].x =-1.0f;
      pointsInit[3].y = 1.0f;
      
      translation = MatOfTranslate( (float)(*li_editables_[i]->pos_x_) + (float)(*li_editables_[i]->width_)/2, (float)(*li_editables_[i]->pos_y_) + (float)(*li_editables_[i]->height_)/2 );
      scale = MatOfEscalate( (float)(*li_editables_[i]->width_)/2, (float)(*li_editables_[i]->height_)/2);
      
      exists = true;
      
    }
    
    if(NULL != li_editables_[i]->rotation && exists){
      
      rotation = MatOfRotation( ((float)(*li_editables_[i]->rotation) * 0.0174532f) );
      
    }else{
      rotation = Mat3Idt();
    }

    if( exists ){
    
      transform = MatOfTransform( translation, rotation, scale);
    
      for(int j=0;j<4;j++){
        
        pointsEnd[j] = MulOfMatWVec( transform, pointsInit[j] );
        
      }
      
      bool negative = false;
      
      for(int j=0;j<3 && !negative ;j++){
        
        float liX = pointsEnd[j+1].x - pointsEnd[j].x;
        float liY = pointsEnd[j].y - pointsEnd[j+1].y;
        float liT = ( pointsEnd[j].x * pointsEnd[j+1].y ) - ( pointsEnd[j+1].x * pointsEnd[j].y );
        
        float comp = (( liY * x ) + (liX * y) + liT) / sqrt( (liX * liX)+(liY * liY) );
        
        if( comp < 0){ negative = true; }
        
      }
      
      float liX = pointsEnd[0].x - pointsEnd[3].x;
      float liY = pointsEnd[3].y - pointsEnd[0].y;
      float liT = ( pointsEnd[3].x * pointsEnd[0].y ) - ( pointsEnd[0].x * pointsEnd[3].y );
        
      float comp = (( liY * x ) + (liX * y) + liT) / sqrt( (liX * liX)+(liY * liY) );
        
      if( comp < 0){ negative = true; }
      
      if(!negative){
                
        if( li_editables_[i] != selected_ ){
          
          selected_ = li_editables_[i];
          return;
        }
          
          
      }
      
    }

    exists = false;
    
  }
  
  
  selected_ = NULL;
  
  
}

void Editor::searchInEditor( int x, int y, int level){
  
  static int s_level_y = 30;
  char buffer[50] = {'\0'};  
  SDL_Point windows_point = {x, y};
  int level_x = 5;
  
  static int s_offset_cont_edit = 0;
  
  if(x > kEditorWidth){
    return;
  }
  
  if(0 == level){
    s_level_y = scroll_y_;
    hitbox_text_ = {0,0,0,0};
    edition_type_ = kDataType_Char;
    edition_data_ = NULL;
    sprintf( buffer_text_, "\0");
    edition_index_ = 0;
    
    text_draw_->set_text( "Lista de Elementos en Scena" );
    text_draw_->set_position( 5, s_level_y);
    
    SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
    SDL_Rect rect_result;
    
    if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result)){
        
      display_list_ = !display_list_;      
        
    }
    s_level_y += 30;
    level_x += 5;
    
    if( display_list_ ){
      
      for(int i=0;i<num_editables_;i++){
        
        sprintf(buffer, "%s" , ((Entity*)(((Editable*)(li_editables_[i]))->object_))->my_name_ );
        text_draw_->set_text( buffer );         
         
        rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
        
            
        if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result)){
          
          selected_ = li_editables_[i];
                    
        }
        s_level_y += 30;
        
      }    
    }
    s_level_y += 3;
    s_offset_cont_edit = 0;
  }
   
  if(NULL == selected_){
    return;
  }  
  
  if(NULL != selected_->father_){
    Editable *backup_selected = selected_;
    selected_ = selected_->father_;
    searchInEditor(x, y, 1);
    if( edition_data_ == NULL ){
      s_offset_cont_edit += selected_->num_data_;
    }
    if(selected_ == backup_selected->father_){
      selected_ = backup_selected;
    }else{
      return_selection_img_ = backup_selected;
      return;
    }
    
    return_selection_img_ = selected_;
    
    s_level_y += 3;
  }
  
  
  
  s_level_y += 30;
  
  int cont_data = 0;
  
  
  
  for(unsigned int i=0;i< selected_->my_blueprint_->vec_agroup()->size(); i++){
    
    text_draw_->set_text( selected_->my_blueprint_->vec_agroup()->at(i)->name);
    
    if( selected_->my_blueprint_->vec_agroup()->at(i)->num_data > 1 ){
      s_level_y += 30;
    }
    
    level_x = 5;
    
    for(int j=0;j< selected_->my_blueprint_->vec_agroup()->at(i)->num_data ;j++){
      
      if( selected_->my_blueprint_->vec_agroup()->at(i)->num_data > 1 ){
        text_draw_->set_text( selected_->my_blueprint_->vec_agroup()->at(i)->data[j].name->c_str());
                
        level_x += 20 + text_draw_->width();
      }else{
        
        level_x = 20 + text_draw_->width();
        
      }
      
      switch(selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type ){
        
        case kDataType_Char:{
          
          
          sprintf(buffer, "%X" , *(char*)selected_->list_data_[cont_data] );
          if( strlen(buffer) > 2 ){
            char tmp_Buffer[50] = { "/0" };
            memcpy(tmp_Buffer, buffer, 50);
            sprintf(buffer, "%.2s" , tmp_Buffer + strlen(buffer) - 2 );
          }else{
            char tmp_Buffer[50] = { "/0" };
            memcpy(tmp_Buffer, buffer, 50);
            sprintf(buffer, "%.2s" , tmp_Buffer);
          }
          text_draw_->set_text( buffer );
          
          SDL_Rect rect_clip = {level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) && selected_->my_blueprint_->vec_agroup()->at(i)->editable){
            
            hitbox_text_ = rect_clip;
            edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
            edition_data_ = selected_->list_data_[cont_data];
            sprintf( buffer_text_, "%s", buffer );
            edition_obj_ = selected_->object_;
            cont_editable_ = cont_data + s_offset_cont_edit;
            return;
          }
          
          level_x += 15 + text_draw_->width();
          
          
          break;
        }
        case kDataType_Int:{
          
          sprintf(buffer, "%d" , *(int*)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );
          
          SDL_Rect rect_clip = { level_x , s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) && selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
                        
            hitbox_text_ = rect_clip;
            edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
            edition_data_ = selected_->list_data_[cont_data];
            sprintf( buffer_text_, "%s", buffer );
            edition_obj_ = selected_->object_;
            cont_editable_ = cont_data + s_offset_cont_edit;
            return;
          }
          
          level_x += 15 + text_draw_->width();
          text_draw_->draw();  
                    
          break;
        }
        case kDataType_Float:{
                    
          sprintf(buffer, "%.4f" , *(float*)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );
          
          SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) && selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            
            hitbox_text_ = rect_clip;
            edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
            edition_data_ = selected_->list_data_[cont_data];
            sprintf( buffer_text_, "%s", buffer );
            edition_obj_ = selected_->object_;
            cont_editable_ = cont_data + s_offset_cont_edit;
            return;
          }
          
          level_x += 15 + text_draw_->width();
          break;
        }
        case kDataType_Bool:{
          
          
          SDL_Rect rect_clip = { level_x, s_level_y, 20, 20 };
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) && selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            
            *((bool*)selected_->list_data_[cont_data]) = !*((bool*)selected_->list_data_[cont_data]);
            
            hitbox_text_ = rect_clip;
            edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
            edition_data_ = NULL;
            edition_obj_ = NULL;            
            return;
          }
          
          level_x += 25;          
          break;
        }
        
        case kDataType_IMG:{
          
          SDL_Rect rect_clip = { level_x, s_level_y, 140, 140 };          
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) ){
            
            if( selected_ == SpriteManager::Instance().my_editable() && selection_img_ != NULL){
              
              free(*((char**)selection_img_->list_data_[0]));
              int len = strlen(*((char**)selected_->list_data_[cont_data - 1]));
              *((char**)selection_img_->list_data_[0]) = (char*)calloc(len + 1, sizeof(char) );
              
              memcpy(*((char**)selection_img_->list_data_[0]), (*((char**)selected_->list_data_[cont_data - 1])), len);
              
              *((SDL_Texture**)selection_img_->list_data_[1]) = ( *((SDL_Texture**)selected_->list_data_[cont_data]) );
              edition_data_ = selection_img_->list_data_[cont_data];
              edition_obj_ = selection_img_->object_;
              
              
              selected_ = return_selection_img_;
              
              selection_img_ = NULL;
            }
            
            return;
          }
          
          s_level_y += 190;          
          break;
        }
        case kDataType_String:{
          
          sprintf(buffer, "%s" , *(char**)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );         
          
          
          SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
          SDL_Rect rect_result;
          
          if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) && selected_->my_blueprint_->vec_agroup()->at(i)->editable){
            hitbox_text_ = rect_clip;
            edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
            edition_data_ = selected_->list_data_[cont_data];
            sprintf( buffer_text_, "%s", buffer );
            edition_obj_ = selected_->object_;
            cont_editable_ = cont_data + s_offset_cont_edit;
            return;
          }
          
         
          level_x += 115;
          break;
        }
        case kDataType_Color:{
          
          char rgba[4];
          
          level_x = 5;
          s_level_y += 30;
          
          rgba[0] = ((SDL_Color*)selected_->list_data_[cont_data])->r; 
          rgba[1] = ((SDL_Color*)selected_->list_data_[cont_data])->g;
          rgba[2] = ((SDL_Color*)selected_->list_data_[cont_data])->b;
          rgba[3] = ((SDL_Color*)selected_->list_data_[cont_data])->a;
          
          
          
          for(int k=0;k<4;k++){
              
            switch(k){
              
              case 0:{
                text_draw_->set_text( "R" );         
                
                break;
              }
              case 1:{
                text_draw_->set_text( "G" );         
                
                break;
              }
              case 2:{
                text_draw_->set_text( "B" );         
                  
                break;
              }
              case 3:{
                text_draw_->set_text( "A" );    
                
                break;
              }
              
            }
            level_x += 15 + text_draw_->width();
            sprintf(buffer, "%X" , rgba[k] );
            if( strlen(buffer) > 2 ){
              char tmp_Buffer[50] = { "/0" };
              memcpy(tmp_Buffer, buffer, 50);
              sprintf(buffer, "%.2s" , tmp_Buffer + strlen(buffer) - 2 );
            }else{
              char tmp_Buffer[50] = { "/0" };
              memcpy(tmp_Buffer, buffer, 50);
              sprintf(buffer, "%.2s" , tmp_Buffer);
            }
            text_draw_->set_text( buffer );
            
            
            SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_Rect rect_result;
            
            if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result) ){
              
              hitbox_text_ = rect_clip;
              edition_type_ = selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type;
              edition_data_ = selected_->list_data_[cont_data];
              sprintf( buffer_text_, "%s", buffer );
              edition_obj_ = selected_->object_;
              edition_index_ = k;
              cont_editable_ = cont_data + s_offset_cont_edit;
              return;
            }
                       
            level_x += 15 + text_draw_->width();
            
          }
        
          s_level_y += 30;
          level_x = 5;
          
          break;
        }
        case kDataType_SpriteManager:{
          
          sprintf(buffer, "%s" , *(char**)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );         
          
          Vec2 pointsEnd[3];
          
          pointsEnd[0] = {(float)level_x + text_draw_->width() + 15, (float)s_level_y };
          pointsEnd[1] = {(float)level_x + text_draw_->width() + 15 + text_draw_->height(), (float)s_level_y};
          pointsEnd[2] = {(float)level_x + text_draw_->width() + 15 + text_draw_->height(), (float)s_level_y+text_draw_->height()};
          
          bool negative = false;
      
          for(int j=0;j<2 && !negative ;j++){
            
            float liX = pointsEnd[j+1].x - pointsEnd[j].x;
            float liY = pointsEnd[j].y - pointsEnd[j+1].y;
            float liT = ( pointsEnd[j].x * pointsEnd[j+1].y ) - ( pointsEnd[j+1].x * pointsEnd[j].y );
            
            float comp = (( liY * x ) + (liX * y) + liT) / sqrt( (liX * liX)+(liY * liY) );
            
            if( comp < 0){ negative = true; }
            
          }
          
          float liX = pointsEnd[0].x - pointsEnd[2].x;
          float liY = pointsEnd[2].y - pointsEnd[0].y;
          float liT = ( pointsEnd[2].x * pointsEnd[0].y ) - ( pointsEnd[0].x * pointsEnd[2].y );
            
          float comp = (( liY * x ) + (liX * y) + liT) / sqrt( (liX * liX)+(liY * liY) );
            
          if( comp < 0){ negative = true; }
          
          if(!negative){
            
            addBlueprint( SpriteManager::Instance().my_blueprint());
            addEditable( SpriteManager::Instance().my_editable() );
            
            selection_img_ = selected_;            
            selected_ = SpriteManager::Instance().my_editable();
            return;
          }
          
          
          level_x += 115;
          break;
        }
        
        
      }
      cont_data++;
            
    }
    s_level_y += 30;
    
  }
  
  if( 0 == level ){
    
    SDL_Rect rect_clip = { 0, SystemManager::Instance()->kWindowH - 35, 200, 35 };
    SDL_Rect rect_result;
          
    if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result)){
      
      my_Scene_->addObject( ((Entity*)selected_->object_)->cloneObject());
      addEditable( ((Entity*)my_Scene_->getLastObject())->my_editable() );
      selected_ = ((Entity*)my_Scene_->getLastObject())->my_editable();
      
      GameManager::Instance()->setGameState(my_Scene_->num_obj_, my_Scene_->list_obj_);

      return;
    }
    
    rect_clip = { 200, SystemManager::Instance()->kWindowH - 35, 200, 35 };
    
    if( SDL_EnclosePoints(&windows_point, 1, &rect_clip, &rect_result)){
      
      my_Scene_->lessObject( selected_->object_ );
      lessEditable(selected_);
      GameManager::Instance()->setGameState(my_Scene_->num_obj_, my_Scene_->list_obj_);

      selected_ = NULL;
      return;
    }
    
  }
  
  
}

void Editor::draw(int level){
  
  static int s_level_y = 30;
  int level_x = 5;
  char buffer[50] = {'\0'};
  
  if(0 == level){    
    
    SDL_Rect global = {0,0, kEditorWidth, SystemManager::Instance()->kWindowH};  
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 123, 123, 123, SDL_ALPHA_OPAQUE);    
    SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &global);    
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &global);
    
    if (scroll_y_ > 30){
    
      scroll_y_ -= 30 ;
      
    }
    s_level_y = scroll_y_;
    
    text_draw_->set_text( "Lista de Elementos en Scena" );
    text_draw_->set_position( 5, s_level_y);
    text_draw_->draw();
    s_level_y += 30;
    if( display_list_ ){
    
      for(int i=0;i<num_editables_;i++){
        
        sprintf(buffer, "%s" , ((Entity*)(((Editable*)(li_editables_[i]))->object_))->my_name_ );
        text_draw_->set_text( buffer );         
        text_draw_->set_position( level_x + 5, s_level_y);
              
        text_draw_->draw(); 
        s_level_y += 30;
      }
      
    }
    
    //-----------------------------
    for(int i=0;i<3;i++){
      SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, level_x, s_level_y +i, level_x + (kEditorWidth - (level_x*2)), s_level_y + i );
    }
    //----------------------------
    s_level_y += 3;
        
  }
  
  if(NULL == selected_){
    return;
  }
    
  if(NULL != selected_->father_){
    Editable *bakcup_selected = selected_;
    selected_ = selected_->father_;
    draw(1);
    selected_ = bakcup_selected;
    //-----------------------------
    for(int i=0;i<3;i++){
      SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, level_x, s_level_y +i, level_x + (kEditorWidth - (level_x*2)), s_level_y + i );
    }
    //----------------------------
    s_level_y += 3;
  }
    
  text_draw_->set_text( selected_->my_blueprint_->name_ );
  text_draw_->set_position( 5, s_level_y);
  text_draw_->draw();
  
  s_level_y += 30;
  
  int cont_data = 0;
  
  
  
  for(unsigned int i=0;i< selected_->my_blueprint_->vec_agroup()->size(); i++){
    
    text_draw_->set_text( selected_->my_blueprint_->vec_agroup()->at(i)->name);
    text_draw_->set_position( 15, s_level_y);
    text_draw_->draw();
    if( selected_->my_blueprint_->vec_agroup()->at(i)->num_data > 1 ){
      s_level_y += 30;
    }
    
    level_x = 5;
    
    for(int j=0;j< selected_->my_blueprint_->vec_agroup()->at(i)->num_data ;j++){
      
      if( selected_->my_blueprint_->vec_agroup()->at(i)->num_data > 1 ){
        text_draw_->set_text( selected_->my_blueprint_->vec_agroup()->at(i)->data[j].name->c_str());
        text_draw_->set_position( 10 + level_x, s_level_y);
        text_draw_->draw();  
        
        level_x += 20 + text_draw_->width();
      }else{
        
        level_x = 20 + text_draw_->width();
        
      }
      
      switch(selected_->my_blueprint_->vec_agroup()->at(i)->my_data_type ){
        
        case kDataType_Char:{
          
          text_draw_->set_position( 5 + level_x, s_level_y);
          sprintf(buffer, "%X" , *(char*)selected_->list_data_[cont_data] );
          if( strlen(buffer) > 2 ){
              sprintf(buffer, "%.2s" , buffer + strlen(buffer) - 2 );
          }else{
            char tmp_Buffer[50] = { "/0" };
            memcpy(tmp_Buffer, buffer, 50);
            sprintf(buffer, "%.2s" , tmp_Buffer);
          }
          text_draw_->set_text( buffer );
          
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }
          text_draw_->draw();
          
          //s_level_y += 30;
          level_x += 15 + text_draw_->width();
          
          
          break;
        }
        case kDataType_Int:{
          text_draw_->set_position( 5 + level_x, s_level_y);
          sprintf(buffer, "%d" , *(int*)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );
          
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_Rect rect_clip = { level_x , s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }
          level_x += 15 + text_draw_->width();
          text_draw_->draw();  
                  
          //s_level_y += 30;
          //level_x += 115 ;
          
          break;
        }
        case kDataType_Float:{
          
          text_draw_->set_position( 5 + level_x, s_level_y);
          sprintf(buffer, "%.4f" , *(float*)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );
          
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }
          text_draw_->draw();  
                 
          //s_level_y += 30;
          level_x += 15 + text_draw_->width();
          break;
        }
        case kDataType_Bool:{
          
          
          SDL_Rect rect_clip = { level_x, s_level_y, 20, 20 };
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }
          SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
          SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          
          
          SDL_Rect interior = { level_x + 5, s_level_y + 5, 10, 10 };
          SDL_RenderDrawRect( SystemManager::Instance()->Main_render_, &interior);
          
          if( *((bool*)selected_->list_data_[cont_data])){
            
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &interior);
            
          }
          //s_level_y += 30;
          level_x += 25;          
          break;
        }
        
        case kDataType_IMG:{
          
          SDL_Rect rect_clip = { 15, s_level_y + 25, 140, 140 };
          
          SDL_RenderCopy(SystemManager::Instance()->Main_render_, *(SDL_Texture**)selected_->list_data_[cont_data], NULL, &rect_clip);
          
          s_level_y += 190;          
          break;
        }
        case kDataType_String:{
          
          sprintf(buffer, "%s" , *(char**)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );         
          text_draw_->set_position( level_x + 5, s_level_y);
          
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }
          
          text_draw_->draw(); 
                 
          level_x += 115;
          break;
        }
        case kDataType_Color:{
          
          char rgba[4];
          
          level_x = 5;
          s_level_y += 30;
          
          rgba[0] = (char)((SDL_Color*)selected_->list_data_[cont_data])->r; 
          rgba[1] = (char)((SDL_Color*)selected_->list_data_[cont_data])->g;
          rgba[2] = (char)((SDL_Color*)selected_->list_data_[cont_data])->b;
          rgba[3] = (char)((SDL_Color*)selected_->list_data_[cont_data])->a;
          
          for(int k=0;k<4;k++){
              
            switch(k){
              
              case 0:{
                text_draw_->set_text( "R" );         
                text_draw_->set_position( level_x + 5 , s_level_y);
                text_draw_->draw();
                break;
              }
              case 1:{
                text_draw_->set_text( "G" );         
                text_draw_->set_position( level_x + 5, s_level_y);
                text_draw_->draw();   
                break;
              }
              case 2:{
                text_draw_->set_text( "B" );         
                text_draw_->set_position( level_x + 5, s_level_y);
                text_draw_->draw();     
                break;
              }
              case 3:{
                text_draw_->set_text( "A" );         
                text_draw_->set_position( level_x + 5, s_level_y);
                text_draw_->draw();
                break;
              }
              
            }
            level_x += 15 + text_draw_->width();
            sprintf(buffer, "%X" , rgba[k] );
            if( strlen(buffer) > 2 ){
              sprintf(buffer, "%.2s" , buffer + strlen(buffer) - 2 );
            }else{
              char tmp_Buffer[50] = { "/0" };
              memcpy(tmp_Buffer, buffer, 50);
              sprintf(buffer, "%.2s" , tmp_Buffer);
            }
            text_draw_->set_text( buffer );
            text_draw_->set_position( 5 + level_x, s_level_y);
            if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
              SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
              SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
              SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
              SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
            }
            
            text_draw_->draw();
            
            level_x += 15 + text_draw_->width();
            
          }
        
          s_level_y += 30;
          level_x = 5;
          
          SDL_Rect rect_clip = { level_x, s_level_y, 140, 20 };
          SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
          SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
          SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);          
          SDL_RenderDrawRect( SystemManager::Instance()->Main_render_, &rect_clip);
                    
          SDL_Rect interior = { 5 + level_x, s_level_y + 5, 130, 10 };
          SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, rgba[0], rgba[1], rgba[2], rgba[3]);          
          SDL_RenderFillRect( SystemManager::Instance()->Main_render_, &interior);
          SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);          
          
          //s_level_y += 30;
          //level_x += 60;
          break;
        }
        case kDataType_SpriteManager:{
          
          sprintf(buffer, "%s" , *(char**)selected_->list_data_[cont_data] );
          text_draw_->set_text( buffer );         
          text_draw_->set_position( level_x + 5, s_level_y);
          
          if ( selected_->my_blueprint_->vec_agroup()->at(i)->editable ){
            SDL_Rect rect_clip = { level_x, s_level_y, 10 + text_draw_->width(), 5 + text_draw_->height() };
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_clip);
            SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &rect_clip);
          }

          SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, level_x + text_draw_->width() + 15, s_level_y, level_x + text_draw_->width() + 15 + text_draw_->height(), s_level_y );
          SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, level_x + text_draw_->width() + 15, s_level_y, level_x + text_draw_->width() + 15 + text_draw_->height(), s_level_y+text_draw_->height() );
          SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, level_x + text_draw_->width() + 15 + text_draw_->height(), s_level_y, level_x + text_draw_->width() + 15 + text_draw_->height(), s_level_y+text_draw_->height() );
          text_draw_->draw(); 
                 
          level_x += 115;
          break;
        }
        
      }
      cont_data++;
            
    }
    s_level_y += 30;
    
  }
  
  if (s_level_y < 50 && 0 == level){
    
    scroll_y_ += ( 50 );
      
  }
  
  if( 0 == level ){
    
    SDL_Rect global = {0,SystemManager::Instance()->kWindowH - 35, kEditorWidth, 35};  
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 200, 200, 200, SDL_ALPHA_OPAQUE);    
    SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &global);    
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(SystemManager::Instance()->Main_render_, &global); 

    
    text_draw_->set_text("CLONAR");
    text_draw_->set_position( 10, SystemManager::Instance()->kWindowH - 30);
    text_draw_->draw();
    
    SDL_RenderDrawLine( SystemManager::Instance()->Main_render_, 200, SystemManager::Instance()->kWindowH - 35, 200, SystemManager::Instance()->kWindowH );
    
    text_draw_->set_position( kEditorWidth - text_draw_->width() - 20, SystemManager::Instance()->kWindowH - 30);
    text_draw_->set_text("ELIMINAR");
    text_draw_->draw();
    
    editionDraw();
  }
  
  
}

void Editor::editionUpdate(sInput *input_event){
   
  if( NULL == edition_data_ ){
    return;
  }
  
  if( (input_event->keyboard_input >= 32 && input_event->keyboard_input <= 122)){
    char tmp_Buffer[20] = { "/0" };
    memcpy(tmp_Buffer, buffer_text_, 20);
    sprintf(buffer_text_, "%s%c", tmp_Buffer, input_event->keyboard_input);
      
  } 
  
  if( input_event->keyboard_input == 8 ){
    
    buffer_text_[strlen(buffer_text_)-1] = '\0';
    
  }
  
  if( input_event->keyboard_input == 10 ){
    
    editionSet();
    buffer_text_[0] = '\0';
    edition_data_ = NULL;
    edition_obj_ = NULL;
    
  }
    
}


void Editor::editionSet(){
  
  if(NULL == edition_data_){
    return;
  }
  
  switch(edition_type_){
    
    case kDataType_Char:{
      
      *((char*)edition_data_) = buffer_text_[0];
      ((Entity*)(edition_obj_))->callBack(cont_editable_);
      
      break;
    }
    case kDataType_Int:{
      
      
      
      *((int*)edition_data_) = std::stoi(buffer_text_);
      ((Entity*)(edition_obj_))->callBack(cont_editable_);
      
      break;
    }
    case kDataType_Float:{
      
      *((float*)edition_data_) = std::stof(buffer_text_);
      ((Entity*)(edition_obj_))->callBack(cont_editable_);
      break;
    }    
    case kDataType_IMG:{
      
      break;
    }
    case kDataType_String:{
      
      char *string_copy = (char*)calloc( strlen(buffer_text_)+1, sizeof(char) );
      

      if(NULL != string_copy ){

        if ( NULL != *((char**)edition_data_) ){
          free(*((char**)edition_data_));
        }
        memcpy(string_copy, buffer_text_, strlen(buffer_text_) );
        
        *((char**)edition_data_) = string_copy;
        ((Entity*)(edition_obj_))->callBack(cont_editable_);
        
      }    
      
      break;
    }
    case kDataType_Color:{
      int hexa = 0;
      
      int len = strlen( buffer_text_ );
              
      for(int i=0;i<len;i++){
        
        if(i == 0){
          
          if( buffer_text_[len-1-i] >= 'A' && buffer_text_[len-1-i] <= 'Z'){
        
            hexa += (buffer_text_[len-1-i] - 'A' + 10);
          
          }else if( buffer_text_[len-1-i] >= '0' && buffer_text_[len-1-i] <= '9'){
            
            hexa += (buffer_text_[len-1-i] - '0') ;
            
          }else if( buffer_text_[len-1-i] >= 'a' && buffer_text_[len-1-i] <= 'z'){
            
             hexa += (buffer_text_[len-1-i] - 'a' + 10);
          }
          
        }else{
          
          if( buffer_text_[len-1-i] >= 'A' && buffer_text_[len-1-i] <= 'Z'){
        
            hexa += (buffer_text_[len-1-i] - 'A' + 10) * ((16 * (i)));
          
          }else if( buffer_text_[len-1-i] >= '0' && buffer_text_[len-1-i] <= '9'){
            
            hexa += ((buffer_text_[len-1-i] - '0') * (16 * (i)));
            
          }else if( buffer_text_[len-1-i] >= 'a' && buffer_text_[len-1-i] <= 'z'){
            
             hexa += ((buffer_text_[len-1-i] - 'a' + 10) * (16 * (i)));
          }
          
        }
                
      }
      
      switch(edition_index_){
        
        case 0:{
          (*(SDL_Color*)edition_data_).r = (char)hexa;
          break;
        }
        case 1:{
          (*(SDL_Color*)edition_data_).g = (char)hexa;
          break;
        }
        case 2:{
          (*(SDL_Color*)edition_data_).b = (char)hexa;
          break;
        }
        case 3:{
          (*(SDL_Color*)edition_data_).a = (char)hexa;
          break;
        }
        
      }
            
      ((Entity*)(edition_obj_))->callBack(cont_editable_);
      edition_data_ = NULL;
      edition_obj_ = NULL;
      
      break;
    }
    default:{
      printf("ERROR");
      break;
    }
    
  }
  
  
  
}



void Editor::editionDraw(){
  
  if(NULL == edition_data_ || NULL == edition_obj_){
    return;
  }
  static int s_old_scroll_y = scroll_y_;
  
  if( scroll_y_ != s_old_scroll_y ){
    
    hitbox_text_.y += scroll_y_ - s_old_scroll_y;
    s_old_scroll_y = scroll_y_;
    
  }
  
  
  char buffer[50] = {'\0'};
  sprintf(buffer, "%s " , buffer_text_ );
  text_draw_->set_text( buffer );         
  text_draw_->set_position( hitbox_text_.x + 5, hitbox_text_.y);
  
  if( text_draw_->width() > hitbox_text_.w ){
    
    SDL_Rect rect_to_draw = {hitbox_text_.x, hitbox_text_.y, text_draw_->width(), text_draw_->height() + 5 };
    
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &rect_to_draw);
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);          
    SDL_RenderDrawRect( SystemManager::Instance()->Main_render_, &rect_to_draw);
    text_draw_->draw();    
    SDL_RenderDrawLine(SystemManager::Instance()->Main_render_, rect_to_draw.x + text_draw_->width() + 1, rect_to_draw.y, rect_to_draw.x + text_draw_->width() + 1, rect_to_draw.y + rect_to_draw.h);
    SDL_RenderDrawLine(SystemManager::Instance()->Main_render_, rect_to_draw.x + text_draw_->width() + 2, rect_to_draw.y, rect_to_draw.x + text_draw_->width() + 2, rect_to_draw.y + rect_to_draw.h);
    
    
  }else{
    
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(SystemManager::Instance()->Main_render_, &hitbox_text_);
    SDL_SetRenderDrawColor(SystemManager::Instance()->Main_render_, 0, 0, 0, SDL_ALPHA_OPAQUE);          
    SDL_RenderDrawRect( SystemManager::Instance()->Main_render_, &hitbox_text_);
    text_draw_->draw();    
    SDL_RenderDrawLine(SystemManager::Instance()->Main_render_, hitbox_text_.x + text_draw_->width() + 1, hitbox_text_.y, hitbox_text_.x + text_draw_->width() + 1, hitbox_text_.y + hitbox_text_.h);
    SDL_RenderDrawLine(SystemManager::Instance()->Main_render_, hitbox_text_.x + text_draw_->width() + 2, hitbox_text_.y, hitbox_text_.x + text_draw_->width() + 2, hitbox_text_.y + hitbox_text_.h); 
    
  }
  
  
  
  
  
}


void Editor::setListObj( void **listObjt, int num){
  
  Editable **new_list_editables = (Editable**)calloc( num , sizeof(Editable*) );
  

  for(int i=0;i<num;i++){
      
    new_list_editables[i] = ((Entity*)listObjt[i])->my_editable();
     
    addBlueprint( ((Entity*)listObjt[i])->my_blueprint() );
      
  }    
  num_editables_ = num;
  
  if( NULL != li_editables_ ){  
    
    free(li_editables_);    
  }
  
  li_editables_ = new_list_editables;
  
}















