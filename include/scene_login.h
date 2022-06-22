//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SCENE_LOGIN_H__
#define __SCENE_LOGIN_H__ 1


#include "scene.h"

/**
* \brief scene to load a User
*/
class SceneLogin : Scene{
  
  public:
  SceneLogin();
  
  ~SceneLogin() override;

  /**
  * \brief Method to init a scene.
  */
  void init() override;

  /**
  * \brief Method execute the input
  */
  void input() override;

  /**
  * \brief Method execute the update
  */
  int update() override;

  /**
  * \brief Method execute the draw
  */
  void draw() override;

  /**
  * \brief Method exit a scene.
  */
  void exit() override;
  
  private:
  SceneLogin(const SceneLogin& other);
  int change_scene_;     /**< Next scene to move*/
 
};


#endif