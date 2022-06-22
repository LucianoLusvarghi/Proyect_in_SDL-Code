//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __SCENE_H__
#define __SCENE_H__ 1

class Editor;

/**
* \brief base class for all classes treated as scene
*/
class Scene{
  
  public:
  Scene();  
  virtual ~Scene();

  /**
  * \brief Virtual method to init a scene.
  */
  virtual void init();

  /**
  * \brief Virtual method execute the input 
  */
  virtual void input();

  /**
  * \brief Virtual method execute the update
  */
  virtual int update();

  /**
  * \brief Virtual method execute the draw
  */
  virtual void draw();

  /**
  * \brief Virtual method to exit a scene.
  */
  virtual void exit();
  
  /** \brief Add an object in the scene
  *   \param new_obj    The new object.
  */
  void addObject(void *new_obj);


  /** \brief Get last object in the scene
  *   \return last object
  */
  void* getLastObject() const;


  /** \brief remove an object in the scene
  *   \param oldObject  The object to remove from the scene.
  */
  void lessObject(void *oldObject);
    

  int num_obj_;               /**< Number of objects in the scene*/
  void **list_obj_;           /**< list of objects in the scene*/
  

  Editor *editor_manager_;    /**< Editor*/
  

  bool editor_mode_;          /**< Is editor open*/
  bool exit_;                 /**< Set to exit the scene*/
  
  private:
  Scene(const Scene& other);
  
};
#endif