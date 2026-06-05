# pragma once

# include <string>

# include "core/reader/mesh.hpp"



class MeshReader {

    Mesh _mesh;
    Region region_from_name(const std::string& name);

    public:
        Mesh read();

};


