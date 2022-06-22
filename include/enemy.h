//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __ENEMY_H__
#define __ENEMY_H__ 1

#include "collider.h"
#include "../deps/Lusvarghi_lib/Include/my_lib.h"

class Bullet;
class BluePrint;
class Editable;

/**
* \brief class that works as Enemy and player.
*/
class Enemy : public Collider{

public:

  Enemy();


  /** \brief Constructor with name.
  *   \param name   the name of the object. If is NULL will be "DefaultName".
  */
  Enemy(const char *name);


  Enemy(const Enemy& other);


  ~Enemy() override;


  /** \brief Method to react to an input.
  *   \param input_event    Pointer to the structure containing the input.
  */
  void input(sInput *input_event)override;


  /**
  * \brief Method to update the object.
  */
  void update() override;


  /**
  * \brief Method to draw the object.
  */
  void draw() override;


  /**
  * \brief Change the position for any of the spawn points and set alive as true;
  */
  void spawn();


  /**
  * \brief Set the spawn points, maximum 15.
  * \param points   List with the spawn points.
  * \param num      Number of elements in the list
  */
  void set_spawn_points(const Vec2 *points, int num);


  /** \brief Set speed movement.
  *   \param newValue     speed movement in pixels per frame.
  */
  void set_speed(int newValue);


  /** \brief Get speed movement.
  *   \return movement speed
  */
  int speed()const;


  /** \brief Set direction movement.
  *   \param newDirection     Direction movement.
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


  /** \brief Set time delay to re-spawn.
  *   \param newValue     Time in milliseconds.
  */
  void set_time_to_respawn(int newValue);


  /** \brief Get time delay to re-spawn.
  *   \return re-spawn time.
  */
  int TimeToRevive()const;


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


  Bullet *bullet_;        /**< Bullet object */
  bool alive_;            /**< current state*/


  bool is_player_;        /**< Control if is player*/
  int delay_time_;        /**< Current time passed*/
  int time_to_respawn_;   /**< re-spawn time*/
private:


  Vec2 direction_;        /**< current direction*/
  bool shoot_;            /**< Can shoot*/
  int speed_;             /**< current speed*/
  

  int num_spawn_points_;          /**< Current num of spawn points*/
  int last_num_spawn_points_;     /**< Max num of spawn points*/
  Vec2 *list_spawn_points_;       /**< List of spawn points*/


  static BluePrint* s_my_blueprint_;  /**< The BluePrint of this Class */
  Editable *my_editable_;             /**< The editable Class of this Class */
};

#endif
