# include <gmsh.h>
# include <format>

# include "preprocessor/builders/airgap.hpp"


namespace model = gmsh::model;
namespace occ = model::occ;




AirGapBuilder::AirGapBuilder(const Config& cfg):
    _cfg(cfg)
{}



void AirGapBuilder::build() {

    int s_airgap_outer = occ::addDisk(
        _cfg.origin.x,
        _cfg.origin.y,
        _cfg.origin.z,
        _cfg.rotor.r_ri,
        _cfg.rotor.r_ri
    );

    int s_airgap_inner = occ::addDisk(
        _cfg.origin.x,
        _cfg.origin.y,
        _cfg.origin.z,
        _cfg.stator.r_so,
        _cfg.stator.r_so
    );

    std::vector<std::pair<int, int>> airgap_cut_result;
    std::vector<std::vector<std::pair<int, int>>> airgap_cut_result_map;

    occ::cut(
        {{2, s_airgap_outer}},
        {{2, s_airgap_inner}},
        airgap_cut_result,
        airgap_cut_result_map
    );

    _s_airgap = airgap_cut_result[0].second;

}



std::string AirGapBuilder::field() const {

    return std::format(
        "{3}"
        "* (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2))"
        "+ (1e8) * (1 - (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _cfg.rotor.r_ri,
        _cfg.stator.r_so,
        std::to_string(_cfg.airgap.mesh_size)
    );

}



std::pair<int, int> AirGapBuilder::s_airgap() const {
    return {2, _s_airgap};
}


