//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__ 1

#include "Lsprite.h"

class BluePrint;
class Editable;

/**
*   \brief Class to use a sprite as a moving and tiled background.
*/
class BackGround : public Lsprite{

public:

  BackGround();


  BackGround(const BackGround& other);


  /** \brief Constructor with the name of the sprite.
  *   \param name       The name of the sprite. 
  */
  BackGround(const char *name);


  /** \brief Constructor with the name of the sprite and how many times it is repeated in the x and y axes.
  *   \param name       The name of the sprite.
  *   \param reply_x    Repeat on x axis.
  *   \param reply_y    Repeat on y axis.
  */
  BackGround(const char *name, int reply_x, int reply_y);


  /** \brief Constructor with the name of the sprite, the repetition and movement in the x and y axes.
  *   \param name       The name of the sprite.
  *   \param reply_x    Repeat on x axis.
  *   \param reply_y    Repeat on y axis.
  *   \param scroll_x   If it can be moved in the x axis.
  *   \param scroll_y   If it can be moved in the y axis.
  */
  BackGround(const char *name, int reply_x, int reply_y, bool scroll_x, bool scroll_y);


  ~BackGround() override;


  /** \brief Set how many times it is repeated in the x and y axes.
  *   \param reply_x    Repeat on x axis.
  *   \param reply_y    Repeat on y axis.
  */
  void set_reply(int reply_x, int reply_y);


  /** \brief Set if it can be moved in the x and y axis.
  *   \param scroll_x   If it can be moved in the x axis.
  *   \param scroll_y   If it can be moved in the y axis.
  */
  void set_scroll(bool scroll_x, bool scroll_y);


  /** \brief Set the speed of movement on the x and y axes.
  *   \param speed_x    Speed on x axis.
  *   \param speed_y    Speed on y axis.
  */
  void set_speed(int speed_x, int speed_y);


  /**
  *   \brief Method to update the object.
  */
  void update() override;


  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw() override;


  /** \brief Method to get the blueprint of the object.
  *   \return the blueprint of this class
  */
  BluePrint* my_blueprint()override;


  /** \brief Method to get the Editable Class of the object.
  *   \return   Editable class
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


  /** \brief Method to clone the object.
  *   \return A copy of this object.
  */
  void* cloneObject() override;


private:
  static BluePrint* s_my_blueprint_;    /**< The BluePrint of this Class */
  Editable *my_editable_;               /**< The editable Class of this object */

  
  int reply_x_;    /**< Repeat on x axis.*/
  int reply_y_;    /**< Repeat on y axis.*/


  bool scroll_x_;  /**< If it can be moved in the x axis.*/
  bool scroll_y_;  /**< If it can be moved in the y axis.*/


  int speed_x_;    /**< Speed on x axis.*/
  int speed_y_;    /**< Speed on y axis.*/

};


#endif