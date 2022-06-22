//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __EDITOR_H__
#define __EDITOR_H__ 1

#include "system_manager.h"
#include "blueprint.h"

class Scene;
class Editable;
class Text;

const int kEditorWidth = 400; /**< width in pixels of the edit menu. */

/*
* \brief Class that manages the operation of the editing tools.
*/
class Editor{

public:

  Editor();
  ~Editor();


  /** \brief Add a BluePrint for content management, required for editable class.
  *   \param new_blueprint	  Pointer to blueprint, no problem adding the same blueprint multiple times.
  */
  void addBlueprint(BluePrint *new_blueprint);


  /** \brief Add Class that contains the modifiable data.
  *   \param new_editable	  Pointer to class that contains the modifiable data, no problem adding the same editable multiple times.
  */
  void addEditable(Editable *new_editable);


  /** \brief Remove an editable element from the manager
  * \param old_editable	    Pointer to class that contains the modifiable data.
  */
  void lessEditable(Editable *old_editable);


  /** \brief Check if the position hits any element of the internal list of elements.
  *   \param x	  Position x of the screen
  *   \param y	  Position y of the screen
  */
  void searchInWindows(int x, int y);


  /** \brief draw the manager on the left side of the screen.
  *   \param level    Manager internal parameter, do not use.
  */
  void draw(int level = 0);


  /** \brief Interaction of the input with the current item in edit.
  *   \param input_event    Pointer to the structure containing the input.
  */
  void editionUpdate(sInput *input_event);


  /**
  * \brief Apply the changes to the corresponding component.  
  */  
  void editionSet();


  /** \brief Get all BluePrint and editable classes from an list of objects.
  *   \param listObjt     List of objects.
  *   \param num          Number of elements in the list.
  */
  void setListObj(void **listObjt, int num);


  int scroll_y_;        /**< Vertical offset to draw the content of the editor. */
  Scene *my_Scene_;     /**< Scene to which the editor is linked. */


private:
  /**
  * \brief Small window that shows the current data in edition.
  */
  void editionDraw();

  /** \brief Check if the position hits any parameters of the editor.
  * \param x	      Screen position x.
  * \param y	      Screen position y.
  * \param level    Manager internal parameter, do not use.
  */
  void searchInEditor(int x, int y, int level = 0);

  int num_blueprint_;               /**< Number of elements in the bluePrint list. */
  BluePrint **li_blueprint_;        /**< The bluePrint list. */


  int num_editables_;               /**< Number of elements in the list of editable class. */
  Editable **li_editables_;         /**< The Editable class list. */


  Editable *selected_;              /**< The current editable class selected. */
  Editable *return_selection_img_;  /**< The Editable class that changes an image.  */
  int cont_editable_;               /**< Internal counter of the data position within the editable class. */


  Editable *selection_img_;         /**< Editable class of the sprite manager */


  Text *text_draw_;                 /**< Text class to draw all the elements of the editor.*/

  
  void *edition_data_;              /**< Current data in edition. */
  void *edition_obj_;               /**< Current object in edition. */
  DataType edition_type_;           /**< Data type of the current data in the edition. */
  SDL_Rect hitbox_text_;            /**< Rect that acts as the background for the editable text.*/
  char buffer_text_[20];            /**< Character buffer for data editing.*/
  int edition_index_;               /**< Temporary variable between functions*/

  bool display_list_;               /**< Control to draw the list of elements.*/

  Editor(const Editor& other);
};



#endif