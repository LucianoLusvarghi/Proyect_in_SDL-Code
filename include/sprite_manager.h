//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__ 1

#include "entity.h"

class BluePrint;
class Editable;

/*
* \brief class that stores and manages sprites.
*/
class SpriteManager : public Entity{

public:
  /**
  * \brief Get the instance of the SpriteManager
  */
  static SpriteManager& Instance();
    
 
  /** \brief Load sprite from disc.
  *   \param file_dir   The addresses and name of the sprites in the folder data/sprites/ . Example: player.png or enemys/ball.png
  */
  void addSprite(const char *file_dir);


  /** \brief Get the firts sprite  with the same name.
  *   \param sprite_name   The name of the sprite, with out the format. Example: player, not player.png
  *   \return Sprite if exist or NULL if not.
  */
  SDL_Texture* getSprite(const char *sprite_name);


  /** \brief Get the sprite in the list position.
  *   \param indice   The position in the List.
  *   \return Sprite if exist or NULL if not.
  */
  SDL_Texture* getSprite(int indice);


  /** \brief Get the sprite name in the list position.
  *   \param indice  The position in the List.
  *   \return Name if exist or NULL if not.
  */
  char* getSpriteName(int indice);


  /** \brief Get the sprite index in the list.
  *   \param sprite_name   The name of the sprite, with out the format. Example: player, not player.png
  *   \return index if exist or -1 if not.
  */
  int getSpriteIndex(const char *sprite_name);


  /** \brief Method to get the BluePrint of this class.
  *   \return BluePrint
  */
  BluePrint* my_blueprint()override;


  /** \brief Method to get the Editable Class of this object.
  *   \return Editable class
  */
  Editable* my_editable()override;


  /**
  * \brief Method to releace the Editable Class of this object.
  */
  void releaceEditable()override;


  /**
  * \brief Method to relece the BluePrint of this class.
  */
  void releaceBluePrint()override;


  /**
  * \brief Callback to apply the editor changes.
  */
  void callBack(int num_edit) override;


  ~SpriteManager();


  int num_sprites_;    /**< Number of sprites */
  char **sprites_dir_; /**< list with sprite addresses */


private:
  SDL_Texture **list_sprites_;  /**< list with sprite */
  char **sprites_name_;         /**< list with sprite name */
  char *new_sprite_;            /**< Text String used in the editor for load a new Sprite in RunTime*/
  

  SpriteManager();


  SpriteManager(const SpriteManager& other);

  

  BluePrint* s_my_blueprint_;   /**< The BluePrint of this Class */
  Editable *my_editable_;       /**< The editable Class of this Class */

};


#endif