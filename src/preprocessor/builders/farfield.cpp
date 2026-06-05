# include <gmsh.h>
# include <format>

# include "preprocessor/builders/farfield.hpp"


namespace model = gmsh::model;
namespace occ = model::occ;




FarFieldBuilder::FarFieldBuilder(const Config& cfg):
    _cfg(cfg)
{
    build();
}



void FarFieldBuilder::build() {

    int s_farfield_outer = occ::addDisk(
        _cfg.origin.x,
        _cfg.origin.y,
        _cfg.origin.z,
        _cfg.rotor.r_ro * _cfg.farfield.ff,
        _cfg.rotor.r_ro * _cfg.farfield.ff
    );

    int s_farfield_inner = occ::addDisk(
        _cfg.origin.x,
        _cfg.origin.y,
        _cfg.origin.z,
        _cfg.rotor.r_ro,
        _cfg.rotor.r_ro
    );

    std::vector<std::pair<int, int>> farfield_cut_result;
    std::vector<std::vector<std::pair<int, int>>> farfield_cut_result_map;

    occ::cut(
        {{2, s_farfield_outer}},
        {{2, s_farfield_inner}},
        farfield_cut_result,
        farfield_cut_result_map
    );

    _s_farfield = farfield_cut_result[0].second;

}



std::string FarFieldBuilder::field() const {

    // lc(r) = lcmin​ + ((lcmax ​− lcmin​) * (r - r_min / (​r_max ​− r_min))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({0} - {1}) / ({2} - {1})) "
        "* (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2))"
        "+ 1e8 * (1 - (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _cfg.rotor.r_ro,
        _cfg.farfield.ff * _cfg.rotor.r_ro,
        _cfg.farfield.mesh_min,
        _cfg.farfield.mesh_max - _cfg.farfield.mesh_min
    );

}




std::pair<int, int> FarFieldBuilder::s_farfield() const {
    return {2, _s_farfield};
}


