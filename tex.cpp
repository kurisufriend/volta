#include "tex.h"
#include <iostream>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>


SDL_Texture* texmgr::load_image(SDL_Renderer *r, std::string path)
{
    static std::map<std::string, SDL_Texture*> texcache;
    if (texcache.count(path))
        {return texcache[path];}

    SDL_Surface* surf = IMG_Load(path.c_str());
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    texcache[path] = tex;
    SDL_FreeSurface(surf);
    return tex;
}

std::map<std::string, SDL_Texture*> texmgr::load_images(SDL_Renderer* r, std::string base, std::vector<std::string> files)
{
    std::map<std::string, SDL_Texture*> res;
    for (auto it = files.begin(); it != files.end(); it++)
    {
        std::string path = base;
        path.append("/");
        path.append(*it);
        std::cout << "loadan " << path << std::endl;
        SDL_Surface* surf = IMG_Load(path.c_str());
        SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
        SDL_FreeSurface(surf);
        res.emplace(*it, tex);
    }
    return res;
}