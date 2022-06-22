//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __LSPRITE_H__
#define __LSPRITE_H__ 1

#include "rect.h"

/**
* \brief class representing a sprite.
*/
class Lsprite : public Rect{

public:  
  Lsprite();


  /** \brief Constructor with name.
  *   \param name   The name of the sprite, and get it from SpriteManager if it exists.
  */
  Lsprite(const char *name);


  Lsprite(const Lsprite& other);


  ~Lsprite() override;


  /** \brief Set the sprite name, before call sprite manager for it.
  *   \param name    Sprite name with out extencion. Example: "player" not "player.png"
  */
  void set_name(const char *name);

   
  /** \brief Call the sprite manager and set the sprite if it exist.
  *   \return true if the sprite exist, else return false.
  */
  bool callSpriteManager();


  /** \brief Get the sprite name.
  *   \return the sprite name.
  */
  const char* name()const;


  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw()override;


  /** \brief Get the height of the object.
  *   \return the height of the object.
  */
  int height();


  /** \brief Get the width of the object.
  *   \return the width of the object.
  */
  int width();


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


  /**
  * \brief Method to clone the object;
  */
  void* cloneObject() override;


protected:
  char *name_;                        /**< Text string*/
  SDL_Texture *sprite_;               /**< Texture Sprite */


private:
  static BluePrint* s_my_blueprint_;  /**< The BluePrint of this Class */
  Editable *my_editable_;             /**< The editable Class of this Class */
};


#endif