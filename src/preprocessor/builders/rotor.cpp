# include <format>

# include <gmsh.h>

# include "preprocessor/builders/rotor.hpp"


namespace occ = gmsh::model::occ;



RotorBuilder::RotorBuilder(const Config& cfg, const DerivedRotorConfig& rcfg, const OriginBuilder& obuilder): 
    _cfg(cfg), _rcfg(rcfg), _obuilder(obuilder)
{};



void RotorBuilder::build_rotor_back_annulus() {

    std::vector<std::pair<int, int>> rotor_cut_results;
    std::vector<std::vector<std::pair<int, int>>> rotor_cut_results_map;

    int s_outer = occ::addDisk(_cfg.origin.x, _cfg.origin.y, _cfg.origin.z, _cfg.rotor.r_ro, _cfg.rotor.r_ro);
    int s_inner = occ::addDisk(_cfg.origin.x, _cfg.origin.y, _cfg.origin.z, _cfg.rotor.r_ri + _cfg.rotor.l_m, _cfg.rotor.r_ri + _cfg.rotor.l_m);

    occ::cut(
        {{2, s_outer}},
        {{2, s_inner}},
        rotor_cut_results,
        rotor_cut_results_map
    );

    _s_rotor_back = rotor_cut_results.at(0).second;

}



void RotorBuilder::build_rotor_pm_annulus() {

    std::vector<std::pair<int, int>> pm_disk_cut_results;
    std::vector<std::vector<std::pair<int, int>>> pm_disk_cut_results_map;

    int s_outer = occ::addDisk(_cfg.origin.x, _cfg.origin.y, _cfg.origin.z, _rcfg.r_mo(), _rcfg.r_mo());
    int s_inner = occ::addDisk(_cfg.origin.x, _cfg.origin.y, _cfg.origin.z, _cfg.rotor.r_ri, _cfg.rotor.r_ri);

    occ::cut(
        {{2, s_outer}},
        {{2, s_inner}},
        pm_disk_cut_results,
        pm_disk_cut_results_map
    );

    _s_rotor = pm_disk_cut_results.at(0).second;

}



void RotorBuilder::build_polegap_vertices() {

    const std::array<std::pair<double, double>, 4> polegap_pt_data = {{
        {_cfg.rotor.r_ri, _cfg.rotor.theta_m / 2.0},
        {_rcfg.r_mo(),    _cfg.rotor.theta_m / 2.0},
        {_rcfg.r_mo(),    _rcfg.theta_p() - (_cfg.rotor.theta_m / 2.0)},
        {_cfg.rotor.r_ri, _rcfg.theta_p() - (_cfg.rotor.theta_m / 2.0)},
    }};

    std::array<std::pair<double, double>, 4> vertices{};

    std::size_t i = 0;
    for (const auto& [r, theta] : polegap_pt_data) {

        double x = (r * sin(theta));
        double y = (r * cos(theta));

        // Normalizing r_so for accurate suface cut
        if (r == _cfg.rotor.r_ri || r == _rcfg.r_mo()) {

            double r_actual = sqrt(x*x + y*y);

            x = x * (r / r_actual);
            y = y * (r / r_actual);

        }

        double r_check = sqrt(x*x + y*y);
        vertices.at(i) = {x, y};

        _polegap_vertices.at(i) = occ::addPoint(x, y, 0);
        i++;

    }

}



void RotorBuilder::build_polegap_profile() {

    int polegap_l12 = occ::addLine(_polegap_vertices.at(0), _polegap_vertices.at(1));

    // Arc P2->P3: the polegap opening at r_mo is an arc centered at origin, not taking chord
    int polegap_c23  = occ::addCircleArc(
        _polegap_vertices.at(1), 
        _obuilder.p_origin().second, 
        _polegap_vertices.at(2)
    );

    int polegap_l34 = occ::addLine(_polegap_vertices.at(2), _polegap_vertices.at(3));

    // Arc P4->P1: the polegap opening at r_ri is an arc centered at origin, not taking chord
    int polegap_c41  = occ::addCircleArc(
        _polegap_vertices.at(3), 
        _obuilder.p_origin().second, 
        _polegap_vertices.at(0)
    );

    int c_polegap = occ::addCurveLoop({
        polegap_l12, polegap_c23, polegap_l34, polegap_c41
    });

    _s_polegap = occ::addPlaneSurface({c_polegap});

}



void RotorBuilder::replicate_polegaps() {

    _all_polegaps.push_back({2, _s_polegap});

    for (int k = 1; k < _cfg.rotor.n_m; ++k) {

        std::vector<std::pair<int, int>> rotated;

        occ::copy({{2, _s_polegap}}, rotated);
        occ::rotate(
            rotated,
            0, 0, 0,
            0, 0, 1,
            k * _rcfg.theta_p()
        );

        _all_polegaps.push_back(rotated.at(0));

    }

}



void RotorBuilder::cut_polegap_profiles() {

    std::vector<std::vector<std::pair<int,int>>> polegap_cut_map;

    occ::cut(
        {{2, _s_rotor}},
        _all_polegaps,
        _s_rotor_cut,
        polegap_cut_map
    );

}



void RotorBuilder::build() {

    build_rotor_back_annulus();
    build_rotor_pm_annulus();
    build_polegap_vertices();
    build_polegap_profile();
    replicate_polegaps();
    cut_polegap_profiles();

}


std::string RotorBuilder::field_yoke() const {

    // lc(r) = lcmin​ + ((lcmax ​− lcmin​) * (r - r_min / (​r_max ​− r_min))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({0} - {1}) / ({2} - {1})) "
        "* (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2))"
        "+ 1e8 * (1 - (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _rcfg.r_mo(),
        _cfg.rotor.r_ro,
        _cfg.rotor.yoke_mesh_min,
        _cfg.rotor.yoke_mesh_max - _cfg.rotor.yoke_mesh_min
    );

}


std::string RotorBuilder::field_pm() const {

    // lc(r) = lcmin​ + ((lcmax ​− lcmin​) * (r - r_min / (​r_max ​− r_min))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({0} - {1}) / ({2} - {1})) "
        "* (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2))"
        "+ 1e8 * (1 - (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _cfg.rotor.r_ri,
        _rcfg.r_mo(),
        _cfg.rotor.pm_mesh_min,
        _cfg.rotor.pm_mesh_max - _cfg.rotor.pm_mesh_min
    );

}




std::pair<int, int> RotorBuilder::s_rotor_back() const {
    return {2, _s_rotor_back};
}



std::vector<std::pair<int,int>> RotorBuilder::s_rotor_cut() const {
    return _s_rotor_cut;
}


