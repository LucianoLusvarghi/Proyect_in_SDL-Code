//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __COLLIDER_H__
#define __COLLIDER_H__ 1

#include "lsprite.h"

class BluePrint;
class Editable;

/**
* \brief class that works as square Collider.
*/
class Collider : public Lsprite{

public: 
  Collider();


  Collider(const Collider& other);


  /** \brief Constructor with name.
  *   \param name   the name of the object. If is NULL will be "DefaultName".
  */
  Collider(const char *name);

  
  ~Collider() override;


  /** \brief Check if the other Collider hits with this.
  *   \param other	  Other Collider object
  *   \return true if hits, else return false
  */
  bool checkCollision(const Collider& other);


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
  void* cloneObject() override;

  
protected:
  bool collision_;  /**< Last checkCollision*/
  
private:
  static BluePrint* s_my_blueprint_; /**< The BluePrint of this Class */
  Editable *my_editable_;          /**< The editable Class of this Class */
};


#endif