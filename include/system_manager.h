//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__ 1

//STD
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

//SDL
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/*
* \brief structure to store the input.
*/
struct sInput{    
  short mouse_key_input;
  short mouse_x;
  short mouse_y;
  short mouse_wheel_input;
  unsigned char keyboard_input;    
};

/*
* \brief Class that manages the interaction with the system and the SDL2 library.
*/
class SystemManager{
  
  public:
  
  //YOU CAN CHANGE THE VARIABLE ABOBE  
  unsigned char fps = 60;     /**< Max of FPS    */
  const int kWindowW = 1600;  /**< Window width  */
  const int kWindowH = 900;   /**< Window height */
                               
  //-------------------------- ------------
  
  Uint32 current_time;          /**<Time in the moment at call */
  Uint32 last_time;             /**<Time in the start of frame */
  Uint32 delta_time;            /**<Time pased bithin the start and end of the Frame */
  
  /**
  * \brief Gets the instance of the SystemManager
  */
  static SystemManager* Instance();


  ~SystemManager();


  /** \brief Init the SDL2 config. Need config windows name first.
  * \return true if OK or false if there is an error. 
  */
  bool initSDL();


  /**
  * \brief Close the SDL2 config.
  */
  void exitSDL();
  

  /** \brief Set the Windows Name.
  *   \param name   the name for the Windows.
  */
  void set_windows_name(const char *name);


  /** \brief Get the windows name
  *   \return  the name of the windows
  */
  const char* windows_name();
  

  /**
  * \brief Load the current System input.
  */
  void UpdateSistemInput();


  /** \brief Get the last input state loaded.
  *   \return last input state
  */
  sInput last_input();


  /**
  * \brief Clear the input state loaded.
  */
  void clearInput();
  

  SDL_Window *Main_window_;   /**< Pointer to windows, used in functions of SDL2*/
  SDL_Renderer *Main_render_; /**< Pointer to render context, used in functions of SDL2*/

  private:
  
  SystemManager();
  SystemManager(const SystemManager& other);
  

  char *windows_name_;  /**< Name of the windows*/
  sInput last_input_;   /**< input state*/
  bool is_majuscule_;   /**< check used in the load of the input*/
  
  
};


#endif