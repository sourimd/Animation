

#include "manager.h"
//IOManager* IOManager::instance = NULL;
//Gamedata* Gamedata::instance = NULL;

int main(int, char*[]) {
   try {
      Manager gameManager;
      gameManager.play();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
