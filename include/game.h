//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __GAME_H__
#define __GAME_H__ 1

class Scene;

/**
* \brief Control the main events in the game
*/
class Game{
  
  public:
  Game(); 
  ~Game();
  
  /**
  * \brief Init the game
  */
  int init();


  /**
  * \brief Start Game Loop
  */
  void loop();


  /**
  * \brief exit the game
  */
  void exit();
  

  /** \brief Add an scene to the Game
  *   \param new_scene   The new Scene
  */
  void addScene(Scene *new_scene);


  int num_scenes_;       /**< Number scenes in the game*/
  Scene **list_scenes_;  /**< List of Scenes in the Game*/
  Scene *current_scene_; /**< Current Scene in execution*/
  private:
  Game(const Game& other);
  
  
  
};
#endif