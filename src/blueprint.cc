//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com
#include "../include/blueprint.h"


BluePrint::BluePrint(){
  
  name_ = NULL;  
  vec_agroup_ = new std::vector<AGroup*>();  
  father_ = NULL;
  
}

BluePrint::BluePrint(char *name){
  
  if(NULL == name){
    
    name_ = (char*)calloc(strlen("Default")+1, sizeof(char) );
    
    memcpy(name_, "Default", strlen("Default") );
    
  }else{  
    name_ = (char*)calloc(strlen(name)+1, sizeof(char) );
    
    memcpy(name_, name, strlen(name) );
  }
  
  vec_agroup_ = new std::vector<AGroup*>();
  
  father_ = NULL;
  
}

BluePrint::BluePrint(const BluePrint& other){}

BluePrint::~BluePrint(){
  
  if(NULL != name_){
    
    free(name_);
    name_ = NULL;
  }
  
  if(0 != vec_agroup_->size()){
    
    for(unsigned int i=0; i< vec_agroup_->size();i++){
      
     if(NULL != vec_agroup_->at(i)->name ){
       
       free( vec_agroup_->at(i)->name );
       vec_agroup_->at(i)->name = NULL;
     } 
     
     if(NULL != vec_agroup_->at(i)->data){
       
       for(int j=0;j<vec_agroup_->at(i)->num_data;j++){
         
         vec_agroup_->at(i)->data[j].name->clear();
 
       }
       
       
       free(vec_agroup_->at(i)->data);
       vec_agroup_->at(i)->data = NULL;
       
     }
     
     vec_agroup_->at(i)->my_data_type = kDataType_Char;
     vec_agroup_->at(i)->editable = false;
      
     
     free(vec_agroup_->at(i));

    }
    
    vec_agroup_->clear();
    delete vec_agroup_;
    vec_agroup_ = NULL;
    
  }  
  father_ = NULL;
}

void BluePrint::set_father(BluePrint *father){  
  father_ = father;    
}
BluePrint* BluePrint::father()const{
  return father_;
}


AGroup* BluePrint::createAgroup(const char *name){
  
  AGroup *new_agroup = (AGroup*)calloc(1, sizeof(AGroup) );
  

  if(NULL == name){
    
    new_agroup->name = (char*)calloc(strlen("Default")+1, sizeof(char) );
    
    memcpy(new_agroup->name, "Default", strlen("Default") );
    
  }else{  
    new_agroup->name = (char*)calloc(strlen(name)+1, sizeof(char) );
    
    memcpy(new_agroup->name, name, strlen(name) );
  }
    
  new_agroup->my_data_type = kDataType_Char;
  new_agroup->num_data = 0;
  new_agroup->data = NULL;
  new_agroup->editable = false;
  
  return new_agroup;
  
}

void BluePrint::set_my_data_type(AGroup* dest, DataType data_tipe){  
  dest->my_data_type = data_tipe;  
}

void BluePrint::set_editable(AGroup* dest, bool editable){  
  dest->editable = editable;  
}

void BluePrint::addData( AGroup *dest, char *name){
  
  if(NULL == dest){
    return;
  }
  
  dest->num_data++;
  
  DataName *new_data = (DataName*)calloc(dest->num_data, sizeof(DataName) );
  new_data[dest->num_data - 1].name = new std::string();

  if ( new_data != NULL ){
    memcpy(new_data, dest->data, (dest->num_data - 1) * sizeof(DataName));
    
    free(dest->data);
    dest->data = new_data;
  }

  if(NULL == name){
    
    new_data[dest->num_data - 1].name->append("Default");

  }else{  

    new_data[dest->num_data - 1].name->append(name);

  }
  
  
}

void BluePrint::addAgroup(AGroup *agrup ){
  
  if ( NULL == agrup )return;

  vec_agroup_->insert(vec_agroup_->begin() + vec_agroup_->size(), agrup);

}

std::vector<AGroup*>* BluePrint::vec_agroup(){  
  return vec_agroup_;    
}

void BluePrint::set_name(char *name){
  
  if(NULL == name){
    
    if ( NULL != name_ ){
      
      free(name_);
      name_ = NULL;
    }
    name_ = (char*)calloc(strlen("Default")+1, sizeof(char) );
    
    memcpy(name_, "Default", strlen("Default") );
    
  }else{  
    if ( NULL != name_ ){
      
      free(name_);
      name_ = NULL;
    }
    name_ = (char*)calloc(strlen(name)+1, sizeof(char) );
    
    memcpy(name_, name, strlen(name) );
  }
  
  
}







