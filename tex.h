#include <SDL2/SDL_image.h>
#include <vector>
#include <map>

namespace texmgr
{
    SDL_Texture* load_image(SDL_Renderer* r, std::string path);
    std::map<std::string, SDL_Texture*> load_images(SDL_Renderer* r, std::string base, std::vector<std::string> files);
}