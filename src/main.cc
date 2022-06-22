//Autor Luciano Lusvarghi	@mail	lusvarghi@esat-alumni.com

//STD

//SDL

//MY
#include "../include/game.h"


int wmain(int argc, char **argv) {
  
  //inicializacion
  Game myGame_;
  
  if( 0 !=  myGame_.init()){
    return 0;
  }  
  
  myGame_.loop();
  
  myGame_.exit();
  
  return 0;
}