#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  SDL_FreeSurface(footballSurface);
  delete footballFrame;
  SDL_FreeSurface(backgroundSurface);
  delete backgroundFrame;
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),

  footballSurface( io.loadAndSet(
    Gamedata::getInstance().getXmlStr("football/file"), 
    Gamedata::getInstance().getXmlBool("football/transparency")) ),
  footballFrame( new Frame("football", footballSurface) ),
  footballs(),
  numberOfFootballs(Gamedata::getInstance().getXmlInt("football/number")),

  backgroundSurface( io.loadAndSet(
    Gamedata::getInstance().getXmlStr("background/file"), 
    Gamedata::getInstance().getXmlBool("background/transparency")) ),
  backgroundFrame( new Frame("background", backgroundSurface) ),
  background("background", backgroundFrame),


  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance().getXmlStr("screenTitle") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
  for(int i=0;i<numberOfFootballs;i++){
    footballs.push_back( Sprite("football", footballFrame));
  }
}

void Manager::drawBackground() const {

  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( backgroundSurface, NULL, screen, &dest );
}

void Manager::draw() const {
  drawBackground();
  
  for(unsigned int i=0;i<(unsigned)footballs.size();i++){
    footballs[i].draw();
  }
  io.printMessageCenteredAt(TITLE, 10);
  io.printMessageValueAt("fps: ", clock.getFps(), 10, 10);
  SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  
  for(unsigned int i=0;i<(unsigned)footballs.size();i++){
    footballs[i].update(ticks);
   }

  if ( makeVideo && frameCount < frameMax ) {
    std::stringstream strm;
    strm << "frames/" << username<< '.' 
         << std::setfill('0') << std::setw(4) 
         << frameCount++ << ".bmp";
    std::string filename( strm.str() );
    std::cout << "Making frame: " << filename << std::endl;
    SDL_SaveBMP(screen, filename.c_str());
  }
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( not done ) {

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }

      if (keystate[SDLK_F4] && !makeVideo) {
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
      if (keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }
    }

    draw();
    update();
  }
}
