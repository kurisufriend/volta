#include "tagging.h"
#include <fstream>
#include "lib/tinydir.h"
#include <map>

nlohmann::json tagging::init(std::string basedir)
{
    std::string meta = basedir;
    meta.append("/volta.json");

    /* create volta.json if it doesn't already exist */
    std::fstream f;
    f.open(meta, std::ios_base::in);
    bool metadata_exists = (bool)f.good();
    f.close();

    if (!metadata_exists)
    {
        f.open(meta, std::ios_base::out);
        std::string fug = "{}";
        f.write(fug.c_str(), fug.size());
        f.close();
    }
    
    /* traverse the media directory, checking for new files and creating metadata */
    f.open(meta, std::ios_base::in);
    nlohmann::json db_current = nlohmann::json::parse(f);
    f.close();

    tinydir_dir d;
    tinydir_open(&d, basedir.c_str());
    while (d.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&d, &file);

        if (db_current[file.name].is_null())
        {
            db_current[file.name] = {"DICKS EVERYWHERE"}; // null tag
        }

        //std::cout << f.name << std::endl;
        tinydir_next(&d);
    }
    tagging::update(basedir, db_current);
    return db_current;
}

nlohmann::json tagging::filtered(nlohmann::json db, std::vector<std::string> tags)
{
    std::map<std::string, std::vector<std::string>> cur = db;
    std::map<std::string, std::vector<std::string>> nu;
    for (auto const& i : cur)
    {
        for (int j = 0; j < db[i.first].size(); j++)
            for (int k = 0; k < tags.size(); k++)
            {
                {
                    if (db[i.first].at(j) == tags.at(k))
                    {
                        nu[i.first] = i.second;
                        break;
                    }
                }
            }

    }
    nlohmann::json a(nu);
    return a;
}

bool tagging::update(std::string basedir, nlohmann::json nu)
{
    std::string path = basedir;
    path.append("/volta.json");
    std::string garbage = nu.dump(4);
    std::fstream f;
    f.open(path, std::ios_base::out);
    f.write(garbage.c_str(), garbage.size());
    f.close();
    return true;
}