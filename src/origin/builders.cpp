# include <gmsh.h>

# include "origin/builders.hpp"



namespace occ = gmsh::model::occ;



OriginBuilder::OriginBuilder(const params::OriginParams& o): _o(o) {}



void OriginBuilder::build() {
    _p_origin = occ::addPoint(_o.x, _o.y, _o.z);
}



std::pair<int, int> OriginBuilder::p_origin() const {
    return {0, _p_origin};
}


