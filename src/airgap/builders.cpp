# include <gmsh.h>
# include <iostream>
# include <format>

# include "airgap/builders.hpp"


namespace model = gmsh::model;
namespace occ = model::occ;




AirGapBuilders::AirGapBuilders(
    const params::StatorParams& s, 
    const params::AirGapParams& a, 
    const params::RotorParams&  r
):
    _s(s), _a(a), _r(r)
{

    std::cout << "Building Air gap ..." << '\n';

}



void AirGapBuilders::build() {

    int s_airgap_outer = occ::addDisk(
        _s.origin.at(0), 
        _s.origin.at(1), 
        _s.origin.at(2), 
        _r.r_ri,
        _r.r_ri
    );

    int s_airgap_inner = occ::addDisk(
        _s.origin.at(0),
        _s.origin.at(1),
        _s.origin.at(2),
        _s.r_so,
        _s.r_so
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



std::string AirGapBuilders::field() const {

    return std::format(
        "{3}"
        "* (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2))"
        "+ (1e8) * (1 - (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _r.r_ri,
        _s.r_so,
        std::to_string(_a.mesh_size)
    );

}


std::pair<int, int> AirGapBuilders::s_airgap() const {
    return {2, _s_airgap};
}
