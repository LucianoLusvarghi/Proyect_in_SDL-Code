//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

#ifndef __BLUEPRINT_H__
#define __BLUEPRINT_H__ 1

#include <string>
#include <vector>

/*
* \brief Type of data.
*/
enum DataType{  
  kDataType_Char = 0,
  kDataType_Int,
  kDataType_Float,
  kDataType_Bool,
  kDataType_IMG,
  kDataType_String,
  kDataType_Color,
  kDataType_SpriteManager,
};

/*
* \brief structure to store data name.
*/
struct DataName{
  std::string *name;  
};


/*
* \brief structure to group data of the same type and function.
*/
struct AGroup{  
  char *name;              /**< The name of the group. */
  DataType my_data_type;   /**< Data type. */
  int num_data;            /**< Number of elements. */
  DataName *data;          /**< stored data name. */
  bool editable;           /**< Internal check for Editor class */
};


/*
* \brief class indicating the grouping of the editable class data.
*/
class BluePrint{
  
  public:    
  BluePrint();


  /** \brief Constructor with name.
  *   \param name   The name of the blueprint. If is NULL will be "Default".
  */
  BluePrint(char *name);  

  
  ~BluePrint();
  

  /** \brief Set the bluePrint father for the current BluePrint.
  *   \param father   BluePrint Father.
  */
  void set_father(BluePrint *father);


  /** \brief Get the bluePrint father for the current BluePrint.
  *   \return father.  
  */
  BluePrint* father()const;
  

  /** \brief Create a new Data group with a name.
  *   \param name   The name of the group. If is NULL will be "Default".
  *   \return new AGroup.
  */
  AGroup* createAgroup(const char *name);


  /** \brief Set the data type of a AGroup.
  *   \param dest        The agroup to set the type.
  *   \param data_tipe   The tipe of data.
  */
  void set_my_data_type(AGroup* dest, DataType data_tipe);


  /** \brief Set if the AGroup can be edited in the editor.
  *   \param dest       The group to set if can be editable.
  *   \param data_tipe   The tipe of data.
  */
  void set_editable(AGroup* dest, bool editable);
  

  /** \brief Set the data name in the AGroup.
  *   \param dest        The group to set the data name.
  *   \param data_tipe   The data name.
  */
  void addData( AGroup *dest, char *data);
  

  /** \brief Add in the current BluePrint a new Agroup.
  *   \param agrup       The group to add. 
  */
  void addAgroup(AGroup *agrup ); 
  

  /** \brief Get the vector of AGroup.
  *   \return the vector of Aproup.
  */
  std::vector<AGroup*>* vec_agroup();
  

  /** \brief Set the name of the current blueprint;
  *   \param name     The name of the bluePrint.
  */
  void set_name(char *name);
  

  char *name_;  /**< The Name of the BluePrint */
  

  private:
  BluePrint(const BluePrint& other);
  BluePrint *father_;                   /**< BluePrint Father. */
  std::vector<AGroup*> *vec_agroup_;    /**< Data groups vector. */
};
#endif