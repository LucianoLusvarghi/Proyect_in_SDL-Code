//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __DATABASE_H__
#define __DATABASE_H__ 1

#include <vector>
#include <sqlite3.h> 

class Entity;
class Editable;
class BluePrint;
class SpriteManager;


/*
* \brief Class that manages the interaction with the database.
*/
class DataBase{

public:
  /**
  * \brief Gets the instance of the database class
  */
  static DataBase& Instance();


  ~DataBase();
 

  /** \brief Add new tipe of object to store.
  *   \param obj  The object.
  */  
  void addObj(Entity *obj);


  /**
  * \brief Save the address of all the sprites stored in the spriteManager in the database.
  */  
  void setSprites();


  /**
  * \brief Load into spriteManager all sprites stored in the database.
  */ 
  void getSprites();


  /** \brief Create a new User in the database.
  *   \param user	      Name of the new User.
  *   \param password	  Password of the new User.
  *   \return The new user ID or -1 if there is an error.
  */
  int setNewUser(const char *user, const char *password);


  /** \brief Create a new Save Game in the database.
  *   \param name	      Name of the Save Game.
  *   \param num_obj	  Number of elements in the list.
  *   \param list_obj	  The list of objects to store.
  *   \return 0 if OK or -1 if there is an error.
  */
  int setNewSaveGame(const char *name, int num_obj, void **list_obj);


  /** \brief Update current save game in the database.
  *   \param name	      New name of the current Save Game.
  *   \param num_obj	  Number of elements in the list.
  *   \param list_obj	  The list of objects to store.
  *   \return 0 if OK or -1 if there is an error.
  */
  int updateSaveGame(const char *name, int num_obj, void **list_obj);


  /** \brief Delete an existing save game in the database.
  *   \param name	    Name of the Save Game to delete.  
  *   \return 0 if OK or -1 if there is an error.
  */
  int deleteSaveGame(const char *name);


  /** \brief Delete an existing User in the database.
  *   \param user	      Name of the User.
  *   \param password	  Password of the User.
  *   \return 0 if OK or -1 if there is an error.
  */
  int deleteUser(const char *user, const char *password);


  /** \brief Get User ID in the database.
  *   \param user	      Name of the User.
  *   \param password	  Password of the User.
  *   \return The User ID or -1 if there is an error.
  */
  int getUserID(const char *user, const char *password);


  /** \brief Get the number of Save Game for the current user.
  *   \param user_id	      Id of the user in the Database.  
  *   \return number of Save Game or -1 if there is an error.
  */
  int getNumSaveGame(int user_id);


  /** \brief Get the list of Save Game ID in the database for the current user.
  *   \param user_id	      Id of the user in the Database.  
  *   \return The id list of saveGame or NULL if there is an error.
  */
  int* getListSaveGame(int user_id);


  /** \brief Get the name of Save Game in the database.
  *   \param user_id	      Id of the saveGame in the Database.  
  *   \return The name of saveGame or NULL if there is an error.
  */
  char* getSaveGameName(int save_id);


  /** \brief Get the Save Game ID in the database.
  *   \param name	      The name of the saveGame.
  *   \return The saveGame ID or -1 if there is an error.
  */
  int getSaveGameId(const char *name);


  /** \brief Get the list of objects stored in the database for the current savegame.
  *   \param save_id	      Id of the saveGame in the Database.  
  *   \return Object List or NULL if there is an error.
  */
  void** getListObjSaveGame(int save_id);


  /** \brief Get num of objects stored in the database for the current savegame.
  *   \param save_id	      Id of the saveGame in the Database.
  *   \return Number of objects or -1 if there is an error.
  */
  int getNumObjSaveGame(int save_id);


  /** \brief Get the last saveGame stored in the database.
  *   \return Id of a saveGame or -1 if there is an error.
  */
  int getLastSaveGameId();


  std::vector<Entity*> *vec_obj_;       /**< Vector with the name of the diferent objects that can be stored. */
  std::vector<char*> *vec_obj_name_;    /**< Vector with the diferent objects that can be stored. */


  int current_user_id_;       /**< The user ID in the database */
  int current_savegame_id_;   /**< The savegame ID in the database */
  sqlite3 *db_;               /**< The database */


private:
 
  DataBase();
  DataBase(const DataBase &other);


  /** \brief Internal callback for store a list of elements.
  *   \param num_obj	  Number of elements in the list.
  *   \param list_obj	The list of objects to store.
  *   \return 0 if OK or -1 if there is an error.
  */
  int setListObj(void **list_obj, int num_obj);


  /** \brief Internal callback for store a obj.
  *   \param obj	  The editable class with the data to store.
  *   \return 0 if OK or -1 if there is an error.
  */
  int setObjInDatabase(Editable *obj);


  /** \brief Internal callback for update a list of elements.
  *   \param num_obj	  Number of elements in the list.
  *   \param list_obj	The list of objects to store.
  *   \return 0 if OK or -1 if there is an error.
  */
  int updateListObj(void **list_obj, int num_obj);


  static DataBase *s_instance_;

};


#endif