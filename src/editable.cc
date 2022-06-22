//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/editable.h"
#include "../include/entity.h"

Editable::Editable(){
  
  my_blueprint_ = NULL;
  father_ = NULL;
  
  num_data_ = 0;
  list_data_ = NULL;
  
  id_ = 0;
  pos_x_ = NULL;
  pos_y_ = NULL;
  width_ = NULL;
  height_ = NULL; 
  rotation = NULL;
}

Editable::Editable(const Editable& other){
  
  my_blueprint_ = other.my_blueprint_;
  father_ = NULL;
  
  num_data_ = 0;
  list_data_ = NULL;
  
  id_ = 0;
  pos_x_ = NULL;
  pos_y_ = NULL;
  width_ = NULL;
  height_ = NULL; 
  rotation = NULL;
}

Editable::Editable(BluePrint *my_plantilla){
  
  my_blueprint_ = my_plantilla;
  father_ = NULL;
  
  num_data_ = 0;
  list_data_ = NULL;
  
  id_ = 0;
  pos_x_ = NULL;
  pos_y_ = NULL;
  width_ = NULL;
  height_ = NULL; 
  rotation = NULL;
}
  
void Editable::set_father(Editable *father){  
  father_ = father;    
}

void Editable::set_my_blueprint(BluePrint *my_plantilla){  
  my_blueprint_ = my_plantilla;  
}
  
void Editable::set_id(int id){ id_ = id;}

int Editable::id(){ return id_; }
  
void Editable::addData(void *data){
  
  if(NULL == data){
    return;
  }
    
  num_data_++;
  
  if(NULL == list_data_){
    
    list_data_ = (void**)calloc( 1, sizeof(void*) );
    

  }else{
    
    void **new_li_data = (void**)calloc(num_data_, sizeof(void*));
    

    if ( new_li_data != nullptr ){

      memcpy(new_li_data, list_data_, (num_data_ - 1) * sizeof(void*));
      
      free(list_data_);
      list_data_ = new_li_data;
    }

  }
  
  list_data_[num_data_-1] = data;
  
}
  
Editable::~Editable(){
  
  my_blueprint_ = NULL;  
 
  if(NULL != list_data_){
    
    free( list_data_);
    num_data_ = 0;
    list_data_ = NULL;
  }
  
  id_ = 0;
  
}

void Editable::set_position(int *x, int *y){
  
  pos_x_ = x;
  pos_y_ = y;
  
}

void Editable::set_dimension(int *w, int *h){  
  width_ = w;
  height_ = h;  
}

void Editable::set_rotation(int *r){  
  rotation = r;  
}

void Editable::set_object(void *obj){  
  object_ = obj;  
}

