//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__ 1

#include "system_manager.h"

/**
* \brief Control the Game state and their rules.
*/
class GameManager{

public:

  /**
  * \brief Gets the s_instance_ of the SpriteManager
  */
  static GameManager* Instance();


  /** \brief Method to react an input.
  *   \param input_event Pointer to the structure containing the input.
  */
  void input(sInput *input_event);


  /**
  * \brief Method update the state of the Game.
  */
  void update();


  /** \brief Set the current list of elements in the scene
  *   \param num_obj    Number of objects in the scene
  *   \param list_obj   List of objects in the scene
  */
  void setGameState(unsigned int num_obj, void **list_obj);


  /** \brief Get the number of objects in the current State
  *   \return number of objects
  */
  unsigned int getNumObjGameState()const;


  /** \brief Get the number of objects in the current State
  *   \return list of objects
  */
  void** getListObjGameState()const;


  /** \brief Set the slot to store the GameState in the database
  *   \param slot the slot to store.
  */
  void set_current_save_slot(unsigned int slot);


  /** \brief Get the current slot where is store the GameState in the database
  *   \return the slot.
  */
  unsigned int current_save_slot()const;


  /**
  * \brief Save the current Game Satate in the slot of the Database.
  */
  void saveGame(unsigned int slot);


  /**
  * \brief Load the SaveGame in the slot of the Database.
  */
  void loadGame(unsigned int slot);


  /** \brief Set if the scene have to create a new GameState
  *   \param newState   the condition
  */
  void set_is_new_game(bool newState);


  /** \brief Get if the scene have to create a new GameState
  *   \return  the condition
  */
  bool is_new_game()const;


  /** \brief Set if the scene have to load a GameState
  *   \param newState   the condition
  */
  void set_is_load_game(bool newState);


  /** \brief Get if the scene have to load a GameState
  * \return  the condition
  */
  bool is_load_game()const;


  /**
  * \brief Destroy the current Game State
  */
  void destroyState();


  /** \brief Set the name of the saveGame
  *   \param name   The name for the saveGame
  */
  void set_save_name(const char* name);


  /** \brief Get the name of the current saveGame
  *   \return   The name of the current saveGame
  */
  const char* save_name()const;


  ~GameManager();


private:
  GameManager();
  GameManager(const GameManager& other);


  int current_save_slot_;   /**< The slot of the GameState*/
  bool is_new_game_;        /**< Check condition*/
  bool is_load_game_;       /**< Check condition*/
  char* save_name_;         /**< saveGame name*/


  unsigned int num_obj_;  /**< Number of objects in the Game State*/
  void **list_obj_;       /**< List of objects in the Game State*/


};
#endif