//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __EDITABLE_H__
#define __EDITABLE_H__ 1

class BluePrint;

/*
* \brief Class that manages editable data from other classes
*/
class Editable{
  
  public:  
  Editable();


  Editable(const Editable& other);


  ~Editable();


  /** \brief Constructor with the corresponding BluePrint .
  *   \param my_plantilla   The bluePrint for the current editable class.
  */
  Editable(BluePrint *my_plantilla);
  

  /** \brief Set the Editable class father for the current editable class.
  *   \param father   Editable class Father
  */
  void set_father(Editable *father);

  /** \brief Set the bluePrint for the current editable class.
  *   \param my_plantilla   The bluePrint for the current editable class.
  */
  void set_my_blueprint(BluePrint *my_plantilla);
  
  /** \brief Link the object position with the editable.
  *   \param x    Pointer to position on x axis.
  *   \param y    Pointer to position on y axis.
  */
  void set_position(int *x, int *y);

  /** \brief Link the object scale with the editable.
  *   \param w    Pointer to the scale width.
  *   \param h    Pointer to the scale height.
  */
  void set_dimension(int *w, int *h);

  /** \brief Link the object rotation with the editable.
  *   \param r   Pointer to the rotation of the object.
  */
  void set_rotation(int *r);
  
  /** \brief Set the Id of the object linked with the editable.
  *   \param id     The Id of the object.
  */
  void set_id(int id);

  /**
  * \brief Get the id of the object linked with the editable.
  */
  int id();
  
  /** \brief Add pointer to data editable, the order to include them must be the same as the order in which they were added in the bluePrint.
  *   \param data   Pointer to data editable.
  */
  void addData(void *data);
  
  /** \brief Set the object linked with this Editable Class.
  *   \param data   Pointer to object.
  */
  void set_object(void *obj);
  
  
  BluePrint *my_blueprint_; /**< The BluePrint of the current editable class. */
  Editable *father_;        /**< Editable class Father */

  int num_data_;            /**< Number of editable data */
  void **list_data_;        /**< List with the editable data */
  int id_;                  /**< Id of the object linked with the current Editable class */

  int *pos_x_, *pos_y_;     /**< Object Position in world, used in the search for the editor */
  int *width_, *height_;    /**< Object Scale, used in the search for the editor */
  int *rotation;            /**< Object Rotation, used in the search for the editor */

  void *object_;            /**< The object linked with the current Editable class */

};
#endif