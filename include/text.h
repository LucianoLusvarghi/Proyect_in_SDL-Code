//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __TEXT_H__
#define __TEXT_H__ 1

#include "rect.h"

class BluePrint;
class Editable;

/**
* \brief class representing a text string.
*/
class Text : public Rect{
      
public:    
  Text();


  Text(const Text& other);  


  /** \brief Constructor with font name.
  *   \param fontName   The addresses and name of the font name. The addresses is relative to data/fonts/*
  */
  Text(const char *fontName);


  /** \brief Constructor with font name and font size.
  *   \param fontName   The addresses and name of the font name. The addresses is relative to data/fonts/*
  *   \param size       The font size.
  */
  Text(const char *fontName, int size);


  /** \brief Constructor with font name, font size and set the text.
  *   \param fontName   The addresses and name of the font name. The addresses is relative to data/fonts/*
  *   \param size       the font size.
  *   \param text       the text string.
  */
  Text(const char *fontName, int size, const char *text);


  /** \brief Constructor with font name, font size, color and set the text.
  *   \param fontName   The addresses and name of the font name. The addresses is relative to data/fonts/*
  *   \param size       the font size.
  *   \param text       the text string.
  *   \param color      Color in RGBA format.
  */
  Text(const char *fontName, int size, const char *text, const SDL_Color& color);
  
  
  ~Text();
  

  /** \brief Set the text string.
  *   \param text    The text string.
  */
  void set_text(const char *text);


  /** \brief Get the text string.
  *   \return the current text string.
  */
  const char* text()const;
  

  /** \brief Change the text font
  *   \param name    The addresses and name of the font name. The addresses is relative to data/fonts/*
  */
  void changeFont(const char *name);


  /** \brief Get the name of the current font.
  *   \return the current font name.
  */
  const char* font_name()const;
  

  /** \brief Get the text color.
  *   \return the current text color in RGBA format.
  */
  SDL_Color GetColor()const;
  

  /**
  * \brief Method of drawing the object on the screen.
  */
  void draw()override;
    

  /** \brief Change the size text
  *   \param newSize   the new size text     
  */
  void changeSize(int newSize);


  /** \brief Get the size text
  *   \return the size text
  */
  int size() const;
  

  /** \brief Method to set the text color.
  *   \param r,g,b,a    The colors in RGBA format.
  */
  void set_color(unsigned char R,unsigned char G,unsigned char B, unsigned char A = 255) override;
  

  /** \brief Method to set the text color.
  *   \param colorRGBA    The colors in RGBA format.
  */
  void set_color(const SDL_Color& colorRGBA);
  

  /** \brief Get the height of the object;
  *   \return the height of the text
  */
  int height();


  /** \brief Get the width of the object;
  *   \return the width of the text
  */
  int width();
  

  /** \brief Method to get the BluePrint of this class.
  *   \return BluePrint
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


  /**
  * \brief Method to clone this object;
  */
  void* cloneObject() override;
  
private:
  char *text_;                        /**< Text string*/
  char *font_name_;                   /**< font name*/
  TTF_Font *font_;                    /**< current font*/
  int size_;                          /**< current size text*/
  SDL_Texture *texture_text_;         /**< Texture with the text */
  
  static BluePrint* s_my_blueprint_;  /**< The BluePrint of this Class */
  Editable *my_editable_;             /**< The editable Class of this Class */
  
};
#endif 