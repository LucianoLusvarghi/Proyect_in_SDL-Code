//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __BULLET_H__
#define __BULLET_H__ 1

#include "collider.h"
#include "../deps/Lusvarghi_lib/Include/my_lib.h"

class BluePrint;
class Editable;

/**
* \brief Class that acts as bullet and explosion animation.
*/
class Bullet : public Collider{

public:  
  Bullet();


  Bullet(const Bullet& other);
  

  /** \brief Constructor with name.
  *   \param name   the name of the object. If is NULL will be "DefaultName".
  */
  Bullet(const char *name);
 

  ~Bullet() override;


  /**
  * \brief Method to update the object.
  */
  void update() override;


  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw() override;


  /** \brief spawn the object in a position and set the direction.
  *   \param position     Position to spawn.
  *   \param direction    Set the direction.
  */
  void spawn(Vec2 position, Vec2 direction);


  /** \brief Set speed movement.
  *   \param newValue   Speed movement in pixels per frame.
  */
  void set_speed(int newValue);


  /** \brief Get speed movement.
  *   \return movement speed
  */
  int speed()const;


  /** \brief Set direction movement.
  *   \param newDirection   Direction movement.
  */
  void set_direction(Vec2 newDirection);


  /** \brief Set direction movement.
  *   \param new_direction_x    Direction movement in the x axis.
  *   \param new_direction_y    Direction movement in the x axis.
  */
  void set_direction(int new_direction_x, int new_direction_y);


  /** \brief Get direction movement.
  *   \return direction movement
  */
  Vec2 direction()const;


  /** \brief Method to get the BluePrint of this class.
  *   \return the blueprint of this class
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


  /** \brief Method to clone the object.
  *   \return A copy of this object.
  */
  void* cloneObject() override;
  

  Vec2 direction_;                  /**< current direction. */
  bool alive_;                      /**< current state. */
  bool explosion_;                  /**< current explosion state. */
  int speed_;                       /**< current speed. */

private:
  int delay_time_;                  /**< Current time passed. */
  SDL_Texture *texture_explosion_;  /**< Texture contains the explosion sprites. */
  int sequence_;                    /**< Explocion sprite sequence. */
  

  static BluePrint* s_my_blueprint_;/**< The BluePrint of this Class. */
  Editable *my_editable_;           /**< The editable Class of this object. */
};

#endif

