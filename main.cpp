#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <SDL2/SDL.h>
#include "lib/tinydir.h"
#include "lib/json.hpp"
#include "lib/imgui/imgui.h"
#include "lib/imgui/backends/imgui_impl_sdl.h"
#include "lib/imgui/backends/imgui_impl_sdlrenderer.h"
#include "tagging.h"
#include "tex.h"


int main(int argc, char* argv[])
{
    std::string BASEDIR = argv[1];
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window* w = SDL_CreateWindow("volta-chan cute librarian maid girl image tagger", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640*2, 360*2, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_version compiled;
    SDL_version linked;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsClassic();
    ImGui_ImplSDL2_InitForSDLRenderer(w, r);
    ImGui_ImplSDLRenderer_Init(r);

    SDL_Event e;
    bool quit = false;

    nlohmann::json db = tagging::init(BASEDIR);
    int ci = 1;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if( e.type == SDL_QUIT )
                {quit = true;}
            if (e.type == SDL_KEYUP)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_DOWN:
                        ci++;
                        break;
                    case SDLK_UP:
                        ci--;
                        break;
                    default:
                        break;
                }
            }
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2 {0.f, 0.f});
        ImGui::Begin("search", NULL);

        ImVec2 p = ImGui::GetWindowPos();
        ImVec2 s = ImGui::GetWindowSize();

        static char buffer[128] = "DICKS EVERYWHERE";
        std::string obuffer = std::string(buffer);
        ImGui::InputText("tag", buffer, 128);

        std::vector<std::string> spltags;
        std::stringstream stream(buffer);
        std::string elem;
        while (std::getline(stream, elem, ','))
            {spltags.push_back(elem);}

        static std::map<std::string, std::vector<std::string>> mdb = tagging::filtered(db, spltags);
        if (obuffer != std::string(buffer))
            {mdb = tagging::filtered(db, {spltags});}
        std::vector<const char*> keys;
        for (auto const& i : mdb)
            {keys.push_back(i.first.c_str());}
        if (!keys.empty())
            {ci %= keys.size();}
        const char* lbl = !keys.empty() ? keys.at(ci) : "nothing~";

        ImGui::ListBox(lbl, &ci, keys.data(), keys.size());

        if (!keys.empty())
        {
            std::string tagss = "tags: ";
            std::vector<std::string> tags = db[std::string(lbl)];
            for (auto it = tags.begin(); it != tags.end(); it++)
            {
                tagss.append(*it);
                tagss.append(", ");
            }
            ImGui::Text("%s", tagss.c_str());

            static char tagbuf[128];
            ImGui::InputText("tag (add/rm)", tagbuf, 128);

            if (ImGui::Button("add tag"))
            {
                bool dupe = false;
                for (auto it = tags.begin(); it != tags.end(); it++)
                {
                    if (*it == std::string(tagbuf))
                    {
                        dupe = true;
                        break;
                    }
                }
                if (!dupe)
                    {db[std::string(lbl)].push_back(std::string(tagbuf));}
                tagging::update(BASEDIR, db);
            }
            if (ImGui::Button("rm tag"))
            {
                std::vector<std::string> a = db[std::string(lbl)];
                a.erase(std::remove(a.begin(), a.end(), std::string(tagbuf)), a.end());
                db[std::string(lbl)] = a;
                tagging::update(BASEDIR, db);
            }
            if (ImGui::Button("open image"))
            {
                std::string cmd = "xdg-open \"";
                cmd.append(BASEDIR);
                cmd.append("/");
                cmd.append(std::string(lbl));
                cmd.append("\"");
                system(cmd.c_str());
            }
        }

        ImGui::Separator();
        if (ImGui::Button("update database"))
            {db = tagging::init(BASEDIR);}

        ImGui::End();
        ImGui::Render();
        SDL_SetRenderDrawColor(r, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(r);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        if (!keys.empty())
        {
            //SDL_Texture* tex = texmap[std::string(lbl)];
            std::string path = BASEDIR;
            path.append("/");
            path.append(std::string(lbl));
            SDL_Texture* tex = texmgr::load_image(r, path);
            int w, h;
            SDL_QueryTexture(tex, NULL, NULL, &w, &h);


            SDL_Rect rect{(int)(p.x + s.x), 1, 300, (int)((float)h*(300/(float)w))};
            SDL_RenderCopy(r, tex, NULL, &rect);
            //SDL_DestroyTexture(tex);
            //ImGui::Image();
        }
        SDL_RenderPresent(r);

    }
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
	SDL_Quit();
    return 0;
}