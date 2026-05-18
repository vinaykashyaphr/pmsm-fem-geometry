# pragma once

# include <filesystem>
# include <string>

# include "preprocessor/params/params.hpp"


namespace fs = std::filesystem;



class Preprocessor {

    const std::string _cfg_file;
    const std::string _msh_file;
    const Params _params;

    Preprocessor(const fs::path& config_file, const fs::path& msh_file);
    Params configure();

};


