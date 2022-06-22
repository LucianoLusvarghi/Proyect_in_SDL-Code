//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#include <assert.h>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../include/database.h"

#include "../include/entity.h"
#include "../include/editable.h"
#include "../include/blueprint.h"
#include "../include/sprite_manager.h"

#pragma warning(disable : 4996)

DataBase* DataBase::s_instance_ = NULL;

int CallBackSprites(void *data, int numColumns, char **ColumData, char **ColumnName);

int CallBackGetUserID(void *data, int numColumns, char **ColumData, char **ColumnName);
int CallBackGetNumSaved(void *data, int numColumns, char **ColumData, char **ColumnName);
int CallBackGetListSaved(void *data, int numColumns, char **ColumData, char **ColumnName);
int CallBackGetNameSaved(void *data, int numColumns, char **ColumData, char **ColumnName);


int CallBackGetListObjSave_num(void *data, int numColumns, char **ColumData, char **ColumnName);
int CallBackGetListObjSave_fill(void *data, int numColumns, char **ColumData, char **ColumnName);

unsigned char TransformHex(char first, char second){
  if ( first >= 97 )first -= 32;
  if ( second >= 97 )second -= 32;

  if ( first >= 65 ){first -= 55;
  }else{ first -= 48; }

  if ( second >= 65 ){second -= 55;
  }else{ second -= 48; }

  return (first * 16) + second;

}


DataBase& DataBase::Instance(){
      
  if ( NULL == s_instance_ ){ s_instance_ = new DataBase();  }
  
  return *s_instance_;
  
}

DataBase::DataBase(){

  vec_obj_ = new std::vector<Entity*>();
  vec_obj_name_ = new std::vector<char*>();

  db_ = NULL;

  current_user_id_ = -1;
  current_savegame_id_ = -1;

  if ( !sqlite3_open("../data/database/database.db", &db_) ){

    printf("Error in DataBase, Creating new");

    //-----------------------------------------------------------------
    std::string sql_query = 
      "CREATE TABLE Users("\
      "  ID        INTEGER PRIMARY KEY AUTOINCREMENT,"\
      "  User_Name STRING  UNIQUE"\
      "  NOT NULL,"\
      "  Password  STRING"\
      ");";
    int ErrorCode = 0;
    char* messaggeError;

    ErrorCode = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &messaggeError);

    if ( ErrorCode != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n num=%d", messaggeError, ErrorCode);
      sqlite3_free(messaggeError);
    } else{
      printf("Table Users created Successfully\n");
    }
    //--------------------------------------------------------------------
    sql_query.clear();
    sql_query =
      "CREATE TABLE saveGame("\
      "  ID      INTEGER PRIMARY KEY AUTOINCREMENT,"\
      "  Used_ID INTEGER REFERENCES Users(ID) ON DELETE CASCADE,"\
      "  SG_Name STRING  NOT NULL"\
      ");";

    ErrorCode = 0;
    messaggeError = NULL;

    ErrorCode = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &messaggeError);

    if ( ErrorCode != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n num=%d", messaggeError, ErrorCode);
      sqlite3_free(messaggeError);
    } else{
      printf("Table saveGame created Successfully\n");
    }
    //-----------------------------------------------------------------------
    sql_query.clear();
    sql_query =
      "CREATE TABLE Objects("\
      "  ID            INTEGER PRIMARY KEY AUTOINCREMENT,"\
      "  SG_ID         INTEGER REFERENCES saveGame(ID) ON DELETE CASCADE,"\
      "  Obj_ClassName STRING  NOT NULL,"\
      "  Obj_Data      STRING"\
      ");";

    ErrorCode = 0;
    messaggeError = NULL;

    ErrorCode = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &messaggeError);

    if ( ErrorCode != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n num=%d", messaggeError, ErrorCode);
      sqlite3_free(messaggeError);
    } else{
      printf("Table Objects created Successfully\n");
    }
    //------------------------------------------------------------------------
    sql_query.clear();
    sql_query = 
      "CREATE TABLE SpriteManager ("\
      "ID     INTEGER   PRIMARY KEY AUTOINCREMENT,"\
      "SM_dir CHAR (50) UNIQUE"\
      ");";

    ErrorCode = 0;
    messaggeError = NULL;

    ErrorCode = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &messaggeError);

    if ( ErrorCode != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n num=%d", messaggeError, ErrorCode);
      sqlite3_free(messaggeError);
    } else{
      printf("Table SpriteManager created Successfully\n");
    }
    //------------------------------------------------------------------------
    sql_query.clear();
  }


}

DataBase::DataBase(const DataBase &other){}



void DataBase::addObj(Entity *obj){
  
  if(NULL == obj){
    return;
  }
  
  const char* name = obj->my_blueprint()->name_;

  //check if the object is already in the vector
  for ( unsigned int i = 0; i < vec_obj_name_->size(); ++i ){
    if ( strcmp(name, vec_obj_name_->at(i)) == 0 ){      
      delete obj;
      return;
    }
  }

  
  char *copyName = (char*)calloc((strlen(name) + 1), sizeof(char));
  
  strcpy(copyName, name);

  vec_obj_name_->insert((vec_obj_name_->begin() + (vec_obj_name_->size())), copyName);
  vec_obj_->insert((vec_obj_->begin() + (vec_obj_->size())), obj);

  
}

void DataBase::setSprites(){
  
  SpriteManager &ptrSpriteManager = SpriteManager::Instance();
  
  for(int i=0;i<ptrSpriteManager.num_sprites_;i++){
    
    std::string sql_query = "Insert into SpriteManager (SM_dir) Values ('";
    sql_query.append( ptrSpriteManager.sprites_dir_[i]);
    sql_query += "');";
    
    sqlite3_exec( db_, sql_query.c_str(), NULL, 0, NULL );
    
  }
  
}
 
int CallBackSprites(void *data, int numColumns, char **ColumData, char **ColumnName){
    
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "SM_dir") == 0){  
    
      SpriteManager::Instance().addSprite(ColumData[i]);
    }
    
  }
  return 0;
}
 
void DataBase::getSprites(){
      
  std::string sql_query = "Select * FROM SpriteManager";
  
  sqlite3_exec( db_, sql_query.c_str(), CallBackSprites, 0, NULL );
    
}

DataBase::~DataBase(){
  
  while ( vec_obj_name_->size() != 0 ){
    free(vec_obj_name_->back());
    vec_obj_name_->pop_back();
  }

  while ( vec_obj_->size() != 0 ){
    (vec_obj_->back())->releaceBluePrint();
    delete vec_obj_->back();
    vec_obj_->pop_back();
  }

  vec_obj_->clear();
  vec_obj_name_->clear();

  delete vec_obj_;
  delete vec_obj_name_;

  sqlite3_close(db_);
  s_instance_ = NULL;
}


int DataBase::setNewUser(const char *user, const char *password){
  
  if(NULL == user || NULL == password ){
    current_user_id_ = -1;
    return -1;
  }
  
  std::string sql_query = "Insert into Users (User_Name, Password) Values ('";
  sql_query.append(user);
  sql_query += "', '";
  sql_query.append(password);
  sql_query += "');";
  
  char *zErrMsg = NULL;
  
  int errorCode = sqlite3_exec( db_, sql_query.c_str(), NULL, 0, &zErrMsg );
  
  if( errorCode != SQLITE_OK  ){
    
    printf("SQL errorCode: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    current_user_id_ = -1;
    return -1;
  }
  
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  
  current_user_id_ = getUserID(user, password);
  
  sql_query.clear();
  return current_user_id_;
  
}
  
int DataBase::setNewSaveGame(const char *name, int num_obj, void **list_obj){
  
  if(NULL == name || 0 == num_obj || NULL == list_obj || current_user_id_ < 0){
    
    return -1;
  }
  
  std::string sql_query = "Insert into saveGame (Used_ID, SG_Name ) Values (";
  sql_query.append( std::to_string(current_user_id_) );
  sql_query += ", '";
  sql_query.append(name);
  sql_query += "');";
  
  char *zErrMsg = NULL;
  
  int errorCode = sqlite3_exec( db_, sql_query.c_str(), NULL, 0, &zErrMsg );
  
  if( errorCode != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }
  
  current_savegame_id_ = getLastSaveGameId();
  sql_query.clear();

  setListObj( list_obj, num_obj );
  
  return 0;
  
  
}

int DataBase::updateSaveGame(const char *name, int num_obj, void **list_obj){
  
  if(NULL == name || 0 == num_obj || NULL == list_obj || current_savegame_id_ < 0){
    return -1;
  }
  
  std::string sql_query = "UPDATE saveGame set SG_Name = '";
  sql_query.append(name);
  sql_query += "' where ID = ";
  sql_query.append(std::to_string(current_savegame_id_));
  sql_query += "; ";
  
  char *zErrMsg = NULL;
  
  int error = sqlite3_exec( db_, sql_query.c_str(), NULL, 0, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }
  
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  
  updateListObj(list_obj, num_obj);
  
  return 0;
  
}

std::string GetObjDataAsString(Editable *obj){

  std::string dataAsString;
  if ( obj->father_ != nullptr ){
    dataAsString = GetObjDataAsString(obj->father_);
  }

  int auxIndex = 0;

  for (unsigned int j = 0; j < obj->my_blueprint_->vec_agroup()->size(); j++ ){

    for ( int k = 0; k < obj->my_blueprint_->vec_agroup()->at(j)->num_data; k++ ){
    
      switch ( obj->my_blueprint_->vec_agroup()->at(j)->my_data_type ){

        case kDataType_Char:{
          char datasize = 1;
          dataAsString += *(static_cast<char*>(obj->list_data_[auxIndex]));
          dataAsString += datasize;
          break;
        }
        case kDataType_Int:{
          std::string convert;
          convert.append( std::to_string(*static_cast<int*>(obj->list_data_[auxIndex])));
          
          char datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();
          break;
        }
        case kDataType_Float:{
          std::string convert;
          convert.append(std::to_string(*static_cast<float*>(obj->list_data_[auxIndex])));

          char datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();
          break;
        }
        case kDataType_Bool:{

          char datasize = 1;
          char objdata = 1;
          if ( *static_cast<bool*>(obj->list_data_[auxIndex]) != 0 )objdata = 2;
          dataAsString += objdata;
          dataAsString += datasize;
          break;
        }
        case kDataType_IMG:{
          char datasize = 3;
          dataAsString += "IMG";
          dataAsString += datasize;          
          break;
        }
        case kDataType_String:{
          std::string convert;
          convert.append(*(static_cast<char**>(obj->list_data_[auxIndex])));
          char datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();          
          break;
        }
        case kDataType_Color:{
          std::string convert;
          SDL_Color objData = *(static_cast<SDL_Color*>(obj->list_data_[auxIndex]));

          convert.append(std::to_string(objData.r));
          char datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();

          convert.append(std::to_string(objData.g));
          datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();

          convert.append(std::to_string(objData.b));
          datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();

          convert.append(std::to_string(objData.a));
          datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();
          
          break;
        }
        case kDataType_SpriteManager:{
          std::string convert;
          convert.append(*(static_cast<char**>(obj->list_data_[auxIndex])));
          char datasize = static_cast<char>(convert.length());
          dataAsString += convert;
          dataAsString += datasize;
          convert.clear();

          break;
        }
        
        default:{
          assert(false);
          break;
        }

      }

      auxIndex++;
    }
  }
  return dataAsString;
}


int DataBase::setObjInDatabase(Editable *obj){
  
  if(NULL == obj){
    return -1;
  }
    
  std::string sql_query = "Insert into Objects (SG_ID, Obj_ClassName, Obj_Data) Values (";
  sql_query.append(std::to_string(current_savegame_id_));
  sql_query += ", '";
  sql_query.append(obj->my_blueprint_->name_);
  sql_query += "', '";
  sql_query += GetObjDataAsString(obj);
  sql_query += "');";

  char *zErrMsg = NULL;

  int error = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }

  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();

  return 0;

}

int DataBase::setListObj(void **list_obj, int num_obj){
  
  if(NULL == list_obj || 0 == num_obj){
    return -1;
  }
  
  for(int i=0;i<num_obj;i++){
    
    if(setObjInDatabase( (static_cast<Entity*>(list_obj[i]))->my_editable() ) == -1){
      return -1;
    }
    
  }
    
  return 0;
  
}

int DataBase::updateListObj(void **list_obj, int num_obj){
  
  if(list_obj == NULL || num_obj == 0){
    return -1;
  }
  
  std::string sql_query = "Delete from Objects where Objects.SG_ID = ";
  sql_query.append(std::to_string(current_savegame_id_));
  sql_query += " ;";

  char *zErrMsg = NULL;

  int error = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }

  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();

  return setListObj(list_obj, num_obj);

}

int DataBase::deleteSaveGame(const char *name){
  
  if(NULL == name){
    return -1;
  }
  
  int IDSaveToDestroy = getSaveGameId(name);
  
  if ( 0 > IDSaveToDestroy )return -1;

  std::string sql_query = "Delete from Objects where Objects.SG_ID = ";
  sql_query.append(std::to_string(current_savegame_id_));
  sql_query += " ;";

  char *zErrMsg = NULL;

  int error = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }

  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();

  sql_query = "Delete from saveGame where saveGame.ID = ";
  sql_query.append(std::to_string(IDSaveToDestroy));
  sql_query += " ;";

  zErrMsg = NULL;

  error = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }

  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();

  //
  if( IDSaveToDestroy == current_savegame_id_ ){
    current_savegame_id_ = -1;
  }
  
  return 0;
    
}



int DataBase::deleteUser(const char *user, const char *password){
  
  if(NULL == user || NULL == password){
    return -1;
  }
    
  int IDUserToDestroy = getUserID(user, password);

  if ( 0 > IDUserToDestroy )return -1;

  std::string sql_query = "Delete from Users where Users.ID = ";
  sql_query.append(std::to_string(IDUserToDestroy));
  sql_query += " ;";

  char *zErrMsg = NULL;

  int error = sqlite3_exec(db_, sql_query.c_str(), NULL, 0, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  }

  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();

  if( current_user_id_ == IDUserToDestroy ){
    current_user_id_ = -1;
  }
  
  return 0;
}

//Get

int CallBackGetUserID(void *data, int numColumns, char **ColumData, char **ColumnName){
  
  int current_id = -1;
  
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "ID") == 0){
      
      current_id = std::stoi(ColumData[i]);
      *(int*)data = current_id;
    }
    
  } 
  return 0;
}

int DataBase::getUserID(const char *user, const char *password){
  
  if(NULL == user || NULL == password){
    return -1;
  }
  
  std::string sql_query = "Select * From Users Where User_Name = '";
  sql_query.append(user); 
  sql_query += "' AND Password = '"; 
  sql_query.append(password);
  sql_query += "' ;";
  
  char *zErrMsg = NULL;
  
  int current_id = -1;
  
  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetUserID, (void*)&current_id, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  } 
  
  if( current_id == -1){
    sql_query.clear();
    return -1;
  }
  
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();
  return current_id;
}

int CallBackGetNumSaved(void *data, int numColumns, char **ColumData, char **ColumnName){
   
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "numSaved") == 0){
      
      *(int*)data = std::stoi(ColumData[i]);
      
    }
    
  } 
  return 0;
}
  
int DataBase::getNumSaveGame(int user_id){
  
  if(0 > user_id){
    return -1;
  }
  
  std::string sql_query = "Select Count(*) AS numSaved From saveGame Where saveGame.Used_ID = ";
  sql_query.append(std::to_string(user_id));
  sql_query += " ;";
  
  char *zErrMsg = NULL;
  
  int current_num = -1;
  
  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetNumSaved, (void*)&current_num, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  } 
  
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();
  return current_num;
  
  
}

int CallBackGetListSaved(void *data, int numColumns, char **ColumData, char **ColumnName){
  
  static int s_index = 0;
  
  if( ((int*)data)[0] == NULL){
    s_index = 0;
  }
  
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "ID") == 0){
      
      ((int*)data)[s_index] = std::stoi(ColumData[i]);
      s_index++;
    }
    
  } 
  return 0;
}

int* DataBase::getListSaveGame(int user_id){
  
  if(0 > user_id){
    return NULL;
  }
  
  std::string sql_query = "Select * From saveGame Where saveGame.Used_ID = ";
  sql_query.append(std::to_string(user_id));
  sql_query += " ;";
  
  char *zErrMsg = NULL;
  
  int current_num = getNumSaveGame(user_id);
  
  if ( current_num == 0 ){ return NULL; }

  int *new_list_savegame;  
  new_list_savegame = (int*)calloc(current_num, sizeof(int) );
  

  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetListSaved, (void*)new_list_savegame, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return NULL;
  } 
  
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  sql_query.clear();
  return new_list_savegame;
  
  
}

int CallBackGetNameSaved(void *data, int numColumns, char **ColumData, char **ColumnName){
   
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "SG_Name") == 0){
      
      int current_num = strlen( ColumData[i] ) + 1;
      
      *(char**)data = (char*)calloc(current_num, sizeof(char) );
      

      strcpy( *(char**)data, ColumData[i] );
     
    }
    
  } 
  return 0;
  
}

char* DataBase::getSaveGameName(int save_id){
  
  if(0 > save_id){
    return NULL;
  }
  
  std::string sql_query = "Select * From saveGame Where ID = ";
  sql_query.append(std::to_string(save_id));
  sql_query += " ;";
  
  char *zErrMsg = NULL;
      
  char *new_list_name_savegame = NULL;  
      
  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetNameSaved, (void*)&new_list_name_savegame, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    
    return NULL;
  } 
  
  return new_list_name_savegame;
  
}
  

int DataBase::getSaveGameId(const char *name){
  
  if( NULL == name ){
    return -1;
  }
  
  std::string sql_query = "Select * From saveGame Where SG_Name = '" ;
  sql_query.append(name);
  sql_query += "' ;";
  
  char *zErrMsg = NULL;
  
  int current_id = -1;
  
  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetUserID, (void*)&current_id, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    
    return -1;
  } 
  if ( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }
  if( current_id == -1){
    return -1;
  }
  
  return current_id;
  
}
  
int CallBackGetListObjSave_num(void *data, int numColumns, char **ColumData, char **ColumnName){
  
  for(int i=0;i<numColumns;i++){
    
    if( strcmp(ColumnName[i], "NumElements") == 0){
      
      (*(int*)data) = std::stoi(ColumData[i]);
      
    }
    
  } 
  return 0;
  
  
}
  

  


int DataBase::getNumObjSaveGame(int save_id){
  
  if(0 > save_id){
    return -1;
  }
  
   std::string sql_query = "Select Count(*) AS NumElements From Objects Where Objects.SG_ID = ";
   sql_query.append(std::to_string(save_id));
   sql_query += " ;";
  
  char *zErrMsg = NULL;
  
  int countBase = 0;
  
  int error = sqlite3_exec( DataBase::Instance().db_, sql_query.c_str(), CallBackGetListObjSave_num, (void*)&countBase, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  } 
  //
  
  return countBase;
  
}

void FillObjWithData(Editable *currentLevel, std::string DataString){

  BluePrint *BP = currentLevel->my_blueprint_;
  int auxIndex = currentLevel->num_data_-1;

  for ( int i = BP->vec_agroup()->size()-1; i >= 0; --i ){

    for ( int j = BP->vec_agroup()->at(i)->num_data-1; j >= 0; --j ){

      switch ( BP->vec_agroup()->at(i)->my_data_type ){
      
        case kDataType_Char:{
          assert(DataString.back() != 1);
          DataString.pop_back();

          char data = DataString.back();
          DataString.pop_back();
          
          *(static_cast<char*>(currentLevel->list_data_[auxIndex])) = data;
         
          break;
        }
        case kDataType_Int:{
          unsigned char size = DataString.back();
          DataString.pop_back();
          std::string convert;
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          int data = std::stoi(convert);
          *(static_cast<int*>(currentLevel->list_data_[auxIndex])) = data;
          convert.clear();
          break;
        }
        case kDataType_Float:{
          unsigned char size = DataString.back();
          DataString.pop_back();
          std::string convert;
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          float data = std::stof(convert);
          *(static_cast<float*>(currentLevel->list_data_[auxIndex])) = data;
          convert.clear();
          break;
        }
        case kDataType_Bool:{
          assert(DataString.back() == 1);
          DataString.pop_back();

          bool data = DataString.back() > 1;
          DataString.pop_back();

          *(static_cast<bool*>(currentLevel->list_data_[auxIndex])) = data;

          break;
        }
        case kDataType_IMG:{

          //*(static_cast<SDL_Texture**>(currentLevel->list_data_[auxIndex])) = SpriteManager::Instance().getSprite(*(static_cast<char**>(currentLevel->list_data_[auxIndex - 1])));

          for(int d =0; d < 4; ++d ){ DataString.pop_back(); }

          break;
        }
        case kDataType_String:{

          unsigned char size = DataString.back();
          DataString.pop_back();
          std::string convert;
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          char *newString = (char*)calloc(convert.length() + 1, sizeof(char));
          memcpy(newString, convert.c_str(), convert.length() * sizeof(char));
          
          *(static_cast<char**>(currentLevel->list_data_[auxIndex])) = newString;
          convert.clear();

          break;
        }
        case kDataType_Color:{
          //---------------ALFA--------------------------
          unsigned char size = DataString.back();
          DataString.pop_back();
          std::string convert;
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          int data = std::stoi(convert);
          SDL_Color *objData = (static_cast<SDL_Color*>(currentLevel->list_data_[auxIndex]));
          objData->a = static_cast<Uint8>(data);
          convert.clear();
          //---------------BLUE--------------------------
          size = DataString.back();
          DataString.pop_back();          
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          data = std::stoi(convert);          
          objData->b = static_cast<Uint8>(data);
          convert.clear();
          //---------------GREEN--------------------------
          size = DataString.back();
          DataString.pop_back();
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          data = std::stoi(convert);
          objData->g = static_cast<Uint8>(data);
          convert.clear();
          //---------------RED--------------------------
          size = DataString.back();
          DataString.pop_back();
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          data = std::stoi(convert);
          objData->r = static_cast<Uint8>(data);
          convert.clear();
          break;
        }
        case kDataType_SpriteManager:{

          unsigned char size = DataString.back();
          DataString.pop_back();
          std::string convert;
          for ( int d = 0; d < size; ++d ){
            convert += DataString.at(DataString.length() - (size - d));
          }
          for ( int d = 0; d < size; ++d ){
            DataString.pop_back();
          }

          char *newString = (char*)calloc(convert.length() + 1, sizeof(char));
          memcpy(newString, convert.c_str(), convert.length() * sizeof(char));

          *(static_cast<char**>(currentLevel->list_data_[auxIndex])) = newString;
          convert.clear();

          *(static_cast<SDL_Texture**>(currentLevel->list_data_[auxIndex+1])) = SpriteManager::Instance().getSprite(*(static_cast<char**>(currentLevel->list_data_[auxIndex])));


          break;
        }

        default:{
          assert(false);
          break;
        }

      }
    
      auxIndex--;
    }

  }

  if ( NULL != currentLevel->father_ ){
    FillObjWithData(currentLevel->father_, DataString);
  }


}

int CallBackGetListObjSave_fill(void *data, int numColumns, char **ColumData, char **ColumnName){
  
  static int s_index = 0;
  void **list_to_fill = static_cast<void**>(data);
  if ( list_to_fill[0] == NULL )s_index = 0;

  void *current_obj = NULL;

  DataBase &database_inst = DataBase::Instance();

  for ( int i = 0; i < numColumns; i++ ){

    if ( strcmp(ColumnName[i], "Obj_ClassName") == 0 ){
      
      for (unsigned int j = 0; j < database_inst.vec_obj_name_->size(); ++j ){

        if ( strcmp(ColumData[i], database_inst.vec_obj_name_->at(j)) == 0 ){
        
          current_obj = (database_inst.vec_obj_->at(j))->cloneObject();
        
        }

      }

    }
    
    if ( strcmp(ColumnName[i], "Obj_Data") == 0 && current_obj != NULL){
      
      FillObjWithData((static_cast<Entity*>(current_obj))->my_editable(), ColumData[i]);
    
    }

  
  }

  list_to_fill[s_index] = current_obj;
  s_index++;

  return 0;
}

void** DataBase::getListObjSaveGame(int save_id){
  
  if(0 > save_id){
    return NULL;
  }
  
  int totalOfElements = getNumObjSaveGame(save_id);
  if ( 0 >= totalOfElements )return NULL;

  void **ListOfObjects = NULL;
  ListOfObjects = (void**)calloc(totalOfElements, sizeof(void*));

  std::string sql_query = "Select * From Objects Where Objects.SG_ID = ";
  sql_query.append(std::to_string(save_id));
  sql_query += " ;";

  char *zErrMsg = NULL;

  int countBase = 0;

  int error = sqlite3_exec(DataBase::Instance().db_, sql_query.c_str(), CallBackGetListObjSave_fill, (void*)ListOfObjects, &zErrMsg);

  if ( error != SQLITE_OK ){

    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return NULL;
  }

  return ListOfObjects;

}
 
int DataBase::getLastSaveGameId(){
  
  std::string sql_query = "Select saveGame.ID AS numSaved From saveGame ORDER BY ID DESC LIMIT 1; ";
  
  char *zErrMsg = NULL;
  
  int save_id = -1;
  
  int error = sqlite3_exec( db_, sql_query.c_str(), CallBackGetNumSaved, (void*)&save_id, &zErrMsg );
  
  if( error != SQLITE_OK  ){
    
    printf("SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sql_query.clear();
    return -1;
  } 
  if( zErrMsg != NULL ){
    sqlite3_free(zErrMsg);
  }

  sql_query.clear();
  return save_id;
  
  
  
}

/*

CREATE TABLE Users (
    ID        INTEGER PRIMARY KEY AUTOINCREMENT,
    User_Name STRING  UNIQUE
                      NOT NULL,
    Password  STRING
);

CREATE TABLE saveGame (
    ID      INTEGER PRIMARY KEY AUTOINCREMENT,
    Used_ID INTEGER REFERENCES Users (ID) ON DELETE CASCADE,
    SG_Name STRING  NOT NULL
);

CREATE TABLE Objects (
    ID            INTEGER PRIMARY KEY AUTOINCREMENT,
    SG_ID         INTEGER REFERENCES saveGame (ID) ON DELETE CASCADE,
    Obj_ClassName STRING  NOT NULL,
    Obj_Data      STRING
);

CREATE TABLE SpriteManager (
    ID     INTEGER   PRIMARY KEY AUTOINCREMENT,
    SM_dir CHAR (50) UNIQUE
);


*/