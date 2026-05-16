# include <gmsh.h>

# include "builders/origin.hpp"


namespace occ = gmsh::model::occ;



OriginBuilder::OriginBuilder(const OriginConfig& cfg): _cfg(cfg) 
{}



void OriginBuilder::build() {

    _p_origin = occ::addPoint(_cfg.x, _cfg.y, _cfg.z);

}



std::pair<int, int> OriginBuilder::p_origin() const {
    return {0, _p_origin};
}



