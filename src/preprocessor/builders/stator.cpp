# include <iostream>
# include <format>

# include <gmsh.h>

# include "preprocessor/builders/stator.hpp"


namespace model = gmsh::model;
namespace occ = model::occ;



StatorBuilder::StatorBuilder(const Config& cfg, const DerivedStatorConfig& scfg, const OriginBuilder& obuilder): 
    _cfg(cfg), _scfg(scfg), _obuilder(obuilder)
{

    std::cout << "Building Stator ..." << '\n';

}



void StatorBuilder::build_stator_annulus() {

    int s_stator_outer = occ::addDisk(
        _cfg.origin.x, 
        _cfg.origin.y, 
        _cfg.origin.z, 
        _cfg.stator.r_so,
        _cfg.stator.r_so
    );

    int s_stator_inner = occ::addDisk(
        _cfg.origin.x, 
        _cfg.origin.y, 
        _cfg.origin.z, 
        _cfg.stator.r_si,
        _cfg.stator.r_si
    );

    std::vector<std::pair<int, int>> stator_cut_result;
    std::vector<std::vector<std::pair<int, int>>> stator_cut_result_map;

    occ::cut(
        {{2, s_stator_outer}},
        {{2, s_stator_inner}},
        stator_cut_result,
        stator_cut_result_map
    );

    _s_stator = stator_cut_result.at(0).second;

}



void StatorBuilder::build_slot_vertices() {

    const std::array<std::pair<double, double>, 8> slot_pt_data = {{
        {_scfg.r_sb(),      _scfg.theta_sb() / 2.0},
        {_scfg.r_sn(),      _scfg.theta_sn() / 2.0},
        {_scfg.r_sm(),      _scfg.theta_s() / 2.0},
        {_cfg.stator.r_so,  _scfg.theta_s() / 2.0},
        {_cfg.stator.r_so,  _scfg.theta_p() / 2.0},
        {_cfg.stator.r_so,  _scfg.theta_p() - (_scfg.theta_s() / 2.0)},
        {_scfg.r_sm(),      _scfg.theta_p() - (_scfg.theta_s() / 2.0)},
        {_scfg.r_sn(),      _scfg.theta_p() - (_scfg.theta_sn() / 2.0)},
    }};

    std::array<std::pair<double, double>, 8> vertices{};

    std::size_t i = 0;
    for (const auto& [r, theta] : slot_pt_data) {

        double x = (r * sin(theta));
        double y = (r * cos(theta));

        // Normalizing r_so for accurate suface cut
        if (r == _cfg.stator.r_so) {

            double r_actual = sqrt(x*x + y*y);

            x = x * (_cfg.stator.r_so / r_actual);
            y = y * (_cfg.stator.r_so / r_actual);

        }

        vertices.at(i) = {x, y};

        std::cout << "P" << i+1 << ": " << x << ", " << y << '\n';

        _slot_vertices.at(i) = occ::addPoint(x, y, 0);
        i++;

    }

}



void StatorBuilder::build_slot_profile() {

    int slot_l12 = occ::addLine(_slot_vertices.at(0), _slot_vertices.at(1));
    int slot_l23 = occ::addLine(_slot_vertices.at(1), _slot_vertices.at(2));
    int slot_l34 = occ::addLine(_slot_vertices.at(2), _slot_vertices.at(3));

    // Arc P4->P5->P6: the slot opening is an arc centered at origin, not taking chord
    int slot_open_arc  = occ::addCircleArc(
        _slot_vertices.at(3), 
        _obuilder.p_origin().second, 
        _slot_vertices.at(5)
    );

    int slot_l67 = occ::addLine(_slot_vertices.at(5), _slot_vertices.at(6));
    int slot_l78 = occ::addLine(_slot_vertices.at(6), _slot_vertices.at(7));
    int slot_l81 = occ::addLine(_slot_vertices.at(7), _slot_vertices.at(0));

    int c_slot = occ::addCurveLoop({
        slot_l12, slot_l23, slot_l34,
        slot_open_arc,
        slot_l67, slot_l78, slot_l81
    });

    _s_slot = occ::addPlaneSurface({c_slot});

}



void StatorBuilder::replicate_slots() {

    _all_slots.push_back({2, _s_slot});

    for (int k = 1; k < _cfg.stator.n_s; ++k) {

        std::vector<std::pair<int, int>> rotated;

        occ::copy({{2, _s_slot}}, rotated);
        occ::rotate(
            rotated,
            0, 0, 0,
            0, 0, 1,
            k * _scfg.theta_p()
        );

        _all_slots.push_back(rotated.at(0));

    }

}



void StatorBuilder::cut_slot_profiles() {

    std::vector<std::vector<std::pair<int,int>>> slot_cut_map;

    occ::cut(
        {{2, _s_stator}},
        _all_slots,
        _s_stator_slotted,
        slot_cut_map
    );

}



void StatorBuilder::build() {

    build_stator_annulus();
    build_slot_vertices();
    build_slot_profile();
    replicate_slots();
    cut_slot_profiles();

}



std::string StatorBuilder::field() const {

    // lc(r) = lcmin​ + ((lcmax​−lcmin​) * (rso​−r / (​rso ​− rsb))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({1} - {0}) / ({1} - {5})) "
        "* (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2)) "
        "+ 1e8 * (1 - (((Sign({1} - {0}) + 1) / 2) * ((Sign({0} - {2}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _cfg.stator.r_so,
        _cfg.stator.r_si,
        _cfg.stator.mesh_min,
        _cfg.stator.mesh_max - _cfg.stator.mesh_min,
        _scfg.r_sb()
    );

}



std::vector<std::pair<int,int>> StatorBuilder::s_stator_slotted() const {
    return _s_stator_slotted;
}


