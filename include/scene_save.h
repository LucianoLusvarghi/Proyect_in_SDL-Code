//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SCENE_SAVE_H__
#define __SCENE_SAVE_H__ 1

#include "scene.h"

/**
* \brief scene to select slot to save the Game
*/
class SceneSave : public Scene{

public:
  SceneSave();

  ~SceneSave() override;

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
  SceneSave(const SceneSave& other);
  int change_scene_;   /**< Next scene to move*/
  int total_num_savegame_;    /**< Number of saveGame */
  int *list_savegame_;    /**< List with the ID of saveGame in the Database*/

};


#endif
