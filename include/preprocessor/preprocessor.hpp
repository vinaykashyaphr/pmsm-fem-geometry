# pragma once

# include <filesystem>
# include <string>

# include "preprocessor/builders/builders.hpp"
# include "preprocessor/params/params.hpp"


namespace fs = std::filesystem;



struct GmshSession {

    GmshSession(const std::string& model_name);
    ~GmshSession();

};




class Preprocessor {

    const std::string _cfg_file;
    const std::string _msh_file;
    const Params _params;
    const GmshSession _gmsh;
    const Builders _builders;

    static Params configure(const fs::path& config_file);
    void synchronize();
    void optimize();
    void mesh();
    void name();

    public:
        Preprocessor(const fs::path& config_file);
        void run();
        void view_mesh() const;
        void export_mesh(const fs::path& msh_file) const;

};


