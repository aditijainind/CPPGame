#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {

  std::map<std::string, std::vector<Frame*> >::iterator posMultiFrames = multiFrames.begin();
  while (posMultiFrames!= multiFrames.end())
  {

    for (unsigned int i = 0;i < (posMultiFrames->second).size();++i)
    {
      delete posMultiFrames->second[i];
    }
    posMultiFrames++;
  }

  std::map<std::string, Frame*>::iterator posFrames = frames.begin();
  while (posFrames != frames.end())
  {
    delete posFrames->second;
    ++posFrames;
  }
  

  std::map<std::string, std::vector<SDL_Surface*> >::iterator posMultiSurfaces = multiSurfaces.begin();
  while (posMultiSurfaces != multiSurfaces.end())
  {
    for (unsigned int i = 0;i < (posMultiSurfaces->second).size();++i)
    {
      SDL_FreeSurface(posMultiSurfaces->second[i]);
    }
    posMultiSurfaces++;
  }

  std::map<std::string, SDL_Surface*>::iterator posSurfaces= surfaces.begin();
  while (posSurfaces != surfaces.end())
  {
    SDL_FreeSurface(posSurfaces->second);
    ++posSurfaces;
  }

}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(name, surface);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}

Frame* FrameFactory::getScaledFrame(const std::string& name, double scale) {
  std::stringstream index;
  index<< name << "_" << scale;
  std::map<std::string, Frame*>::const_iterator pos = frames.find(index.str());
  if (pos == frames.end()) {
    SDL_Surface * surface = NULL;
    std::map<std::string, SDL_Surface*>::const_iterator posSurface = surfaces.find(name);
    if (posSurface == surfaces.end())
    {
      surface = IOManager::getInstance().loadAndSet(Gamedata::getInstance().getXmlStr(name + "/file"),
        Gamedata::getInstance().getXmlBool(name + "/transparency"));
      surfaces[name] = surface;
    }
    else
      surface = posSurface->second;
    
    
    SDL_Surface * const scaledSurface = rotozoomSurface(surface, 0, scale, SMOOTHING_ON);
    surfaces["scaled" + index.str()] = scaledSurface;

    Frame * const frame = new Frame(scaledSurface, scaledSurface->w, scaledSurface->h,
      Gamedata::getInstance().getXmlInt(name + "/src/x"),
      Gamedata::getInstance().getXmlInt(name + "/src/y"));
    frames[index.str()] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}