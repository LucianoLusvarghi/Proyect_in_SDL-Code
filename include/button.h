//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __BUTTON_H__
#define __BUTTON_H__ 1

#include "rect.h"

class Text;
class Lsprite;
class BluePrint;
class Editable;

/**
* \brief Class that works as input button.
*/
class Button : public Rect{
public:

  Button();


  Button(Button &other);


  /** \brief Constructor with name.
  *   \param name   The name of the sprite, and get it if it exists.
  */
  Button(const char *name);



  ~Button() override;
  

  /** \brief Check if the point hits with this button.
  *   \param mouse_x	  Point on the x axis
  *   \param mouse_y	  Point on the x axis
  *   \return true if the point hits with the button, otherwise it return false.
  */
  bool isPressed(short mouse_x, short mouse_y );
  

  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw() override;
  

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

  
  /** \brief Set the sprite name, and call sprite manager for it.
  *   \param image_name    Sprite name with out extencion. Example: "player" not "player.png".
  */
  void loadImg(const char *image_name);


  /** \brief Set the font name, font size, color and set the text.
  *   \param fontName   The addresses and name of the font. The addresses is relative to data/fonts/*
  *   \param text       The text string.
  *   \param size       The font size.
  *   \param color      Color in RGBA format.
  */
  void set_text(const char *font, const char *text, int size, const SDL_Color& color);
  

  /** \brief Set the text string.
  *   \param text    The text string.
  */
  void changeText(const char *text);


  /** \brief Change the text font
  *   \param font    The addresses and name of the font. The addresses is relative to data/fonts/*
  */
  void changeFont(const char *font);


  /** \brief Method to set the text color.
  *   \param r,g,b,a    The colors in RGBA format.
  */
  void changeColorText(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255);
  

  /** \brief Method to set the text color.
  *   \param colorRGBA    The colors in RGBA format.
  */
  void changeColorText(const SDL_Color& colorRGBA);


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
  

private:
  Text *text_;                        /**< Text class*/
  Lsprite *sprite_;                   /**< Sprite Class*/


  static BluePrint* s_my_blueprint_;  /**< The BluePrint of this Class */
  Editable *my_editable_;             /**< The editable Class of this Class */

};


#endif

