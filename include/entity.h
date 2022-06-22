//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __ENTITY_H__
#define __ENTITY_H__ 1

#include <stdint.h>
#include "system_manager.h"

class BluePrint;
class Editable;

/**
* \brief tags of all engine classes
*/
enum EngineObjectTag{
  kEngineObjectTag_noTag  = 0,
  kEngineObjectTag_rect,
  kEngineObjectTag_text,
  kEngineObjectTag_sprite,
  kEngineObjectTag_backGround,
  kEngineObjectTag_button,
  kEngineObjectTag_InputField,
  kEngineObjectTag_colider,
  kEngineObjectTag_enemy,
  kEngineObjectTag_bullet
};


/**
* \brief base class for all classes treated as objects
*/
class Entity{
  
  public:
  
  Entity();

  /** \brief Constructor with name.
  *   \param name   The name of the object. If is NULL will be "DefaultName".
  */
  Entity(const char *name);


  /** \brief Constructor with name and tag(not EngineTag).
  *   \param name   The name of the object. If is NULL will be "DefaultName".
  *   \param tag    The tag for this object(not a EngineTag).
  */
  Entity(const char *name, int tag);


  Entity(const Entity& other); 

  /** \brief Constructor tag, not EngineTag.
  *   \param tag    the tag for this object, not a EngineTag.
  */
  Entity(int tag);
  

  virtual ~Entity();
  

  /**
  *   \brief Toggle the state enable.
  */
  void toggleEnable();
  

  /**
  *   \brief Get the id of the current object
  */
  int id();
  

  /** \brief Virtual method to set the object position.
  *   \param x    New position on the x axis.
  *   \param y    New position on the y axis.
  */
  virtual void set_position(int x, int y);


  /** \brief Virtual method to set the object position.
  *   \param new_position    New position.
  */
  virtual void set_position(const SDL_Point &new_position);


  /** \brief Virtual method to set the rotation in degrees.
  *   \param ang    New angle in degrees.
  */
  virtual void set_rotation(int ang);


  /** \brief Virtual method to set the scale.
  *   \param w    New scale on the x axis.
  *   \param h    New scale on the y axis.
  */
  virtual void set_scale(int w, int h);


  /** \brief Virtual method to set the scale.
  *   \param new_scale    New scale
  */
  virtual void set_scale(const SDL_Point &new_scale);
  

  /** \brief Virtual method to set the color.
  *   \param r,g,b,a    The colors in RGBA format.
  */
  virtual void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
  

  /** \brief Virtual method to react to an input.
  *   \param input_event Pointer to the structure containing the input.
  */
  virtual void input(sInput *input_event);


  /**
  * \brief Virtual method update the state of the object.
  */
  virtual void update();

  /**
  * \brief Virtual method to draw the object.
  */
  virtual void draw();
  

  /**
  * \brief Set the name of the Object;
  */
  void set_my_name(const char *name);
  

  /** \brief Virtual method to get the BluePrint of this class.
  *   \return the blueprint of this class
  */
  virtual BluePrint* my_blueprint();  


  /** \brief Virtual method to get the Editable Class of this object.
  *   \return Editable class
  */
  virtual Editable* my_editable();


  /**
  * \brief Virtual method to releace the Editable Class of this object.
  */
  virtual void releaceEditable();


  /**
  * \brief Virtual method to relece the BluePrint of this class.
  */
  virtual void releaceBluePrint();


  /**
  * \brief Virtual callback to apply the editor changes.
  */
  virtual void callBack(int num_edit);


  /**
  * \brief Virtual method to clone this object;
  */
  virtual void* cloneObject();
  
  
  int tag_;                     /**< Object tag */
  EngineObjectTag engine_tag_;  /**< object EngineTag */

  
  uint8_t enable_;              /**< check if Enable */
  

  char *my_name_;               /**< Name of the object */
  

  private:  
  static int s_last_id;         /**< Control for assigning Ids */  
  int id_;                      /**< Objects Id */
  
  static BluePrint *s_my_blueprint_;  /**< The BluePrint of this Class */
  Editable *my_editable_;             /**< The editable Class of this object */
  
};
#endif 