//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __RECT_H__
#define __RECT_H__ 1

#include "entity.h"

class BluePrint;
class Editable;

/**
* \brief class representing a rectangle
*/
class Rect : public Entity{
      
public:  
  Rect();


  /** \brief Constructor with name.
  *   \param name   the name of the object. If is NULL will be "DefaultName".
  */
  Rect(const char *name);


  Rect(const Rect& other);
  

  ~Rect() override;
    

  /** \brief Method to set the object position.
  *   \param new_position    New position.
  */
  void set_position(const SDL_Point& new_position) override;


  /** \brief Method to set the object position.
  *   \param x    New position on the x axis.
  *   \param y    New position on the y axis.
  */
  void set_position(int x, int y) override;


  /** \brief Method to set the rotation in degrees.
  *   \param new_rotation    New rotation angle in degrees.
  */
  void set_rotation(int new_rotation) override;


  /** \brief Method to set the scale.
  *   \param new_scale    New scale.
  */
  void set_scale(const SDL_Point& new_scale) override;


  /** \brief Method to set the scale.
  *   \param w    New scale on the x axis.
  *   \param h    New scale on the y axis.
  */
  void set_scale(int w, int h) override;
  

  /** \brief Method to set the color.
  *   \param r,g,b,a    The colors in RGBA format.
  */
  void set_color(unsigned char R,unsigned char G,unsigned char B,unsigned char A = 255) override;
  

  /** \brief Method to set the color.
  *   \param colorRGBA    The colors in RGBA format.
  */
  void set_color(const SDL_Color& colorRGBA);
  

  /** \brief Method to set if the draw the rectangle fill or not.
  * \param newState    Fill rectangle or not.
  */
  void set_solid(uint8_t newState);
  

  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw()override;
  

  /** \brief Method to get the BluePrint of this class.
  *   \return the blueprint of this class
  */
  BluePrint* my_blueprint()override;


  /** \brief Method to get the Editable Class of this object.
  *   \return the Editable class of this object
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
  

  SDL_Rect rect_;       /**< Struct with the rectangle data*/
  int rotation_;        /**< Rotation in degrees*/
  
  SDL_Color color_;     /**< Struct with RGBA Color format*/
  
  uint8_t solid_;       /**< check if fill */
  
private:
  static BluePrint* s_my_blueprint_;      /**< The BluePrint of this Class */
  Editable *my_editable_;                 /**< The editable Class of this Class */
    
};
#endif 