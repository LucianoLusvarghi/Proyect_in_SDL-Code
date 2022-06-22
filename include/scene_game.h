//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__ 1

#include "scene.h"

/**
* \brief Scene for the Game
*/
class SceneTest : Scene{
  
  public:
  SceneTest();  
  ~SceneTest() override;

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
  SceneTest(const SceneTest& other);
  
  int offset_x_; /**< draw offset in editor mode on*/

};


#endif