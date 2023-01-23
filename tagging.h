#pragma once
#include "lib/json.hpp"
#include <vector>

namespace tagging
{
    /* checks for metadata / creates missing metadata / returns database */
    nlohmann::json init(std::string basedir);
    /* returns the db filtered by tag */
    nlohmann::json filtered(nlohmann::json db, std::vector<std::string> tag);
    /* updates the on-disk metadata with a new database */
    bool update(std::string basedir, nlohmann::json nu);
    #ifdef AIDS
    /* returns the new state of the node */
    std::vector<std::string> addtag(std::string filename, std::string tag);
    /* returns the new state of the node */
    std::vector<std::string> rmtag(std::string filename, std::string tag);
    #endif
}