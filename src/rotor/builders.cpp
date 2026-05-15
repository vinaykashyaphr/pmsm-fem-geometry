# include <iostream>
# include <format>

# include <gmsh.h>

# include "rotor/builders.hpp"



namespace occ = gmsh::model::occ;



RotorBuilders::RotorBuilders(const params::OriginParams& o, const OriginBuilder& ob, const DerivedRotorParams& r): 
    _o(o), _ob(ob), _r(r)
{};



void RotorBuilders::build_rotor_back_annulus() {

    std::vector<std::pair<int, int>> rotor_cut_results;
    std::vector<std::vector<std::pair<int, int>>> rotor_cut_results_map;

    int s_outer = occ::addDisk(_o.x, _o.y, _o.z, _r.r_ro, _r.r_ro);
    int s_inner = occ::addDisk(_o.x, _o.y, _o.z, _r.r_ri + _r.l_m, _r.r_ri + _r.l_m);

    occ::cut(
        {{2, s_outer}},
        {{2, s_inner}},
        rotor_cut_results,
        rotor_cut_results_map
    );

    _s_rotor_back = rotor_cut_results.at(0).second;

}



void RotorBuilders::build_rotor_pm_annulus() {

    std::vector<std::pair<int, int>> pm_disk_cut_results;
    std::vector<std::vector<std::pair<int, int>>> pm_disk_cut_results_map;

    int s_outer = occ::addDisk(_o.x, _o.y, _o.z, _r.r_mo(), _r.r_mo());
    int s_inner = occ::addDisk(_o.x, _o.y, _o.z, _r.r_ri, _r.r_ri);

    occ::cut(
        {{2, s_outer}},
        {{2, s_inner}},
        pm_disk_cut_results,
        pm_disk_cut_results_map
    );

    _s_rotor = pm_disk_cut_results.at(0).second;

}



void RotorBuilders::build_polegap_vertices() {

    const std::array<std::pair<double, double>, 4> polegap_pt_data = {{
        {_r.r_ri,   _r.theta_m / 2.0},
        {_r.r_mo(), _r.theta_m / 2.0},
        {_r.r_mo(), _r.theta_p() - (_r.theta_m / 2.0)},
        {_r.r_ri,   _r.theta_p() - (_r.theta_m / 2.0)},
    }};

    std::array<std::pair<double, double>, 4> vertices{};

    std::size_t i = 0;
    for (const auto& [r, theta] : polegap_pt_data) {

        double x = (r * sin(theta));
        double y = (r * cos(theta));

        // Normalizing r_so for accurate suface cut
        if (r == _r.r_ri || r == _r.r_mo()) {

            double r_actual = sqrt(x*x + y*y);

            x = x * (r / r_actual);
            y = y * (r / r_actual);

        }

        double r_check = sqrt(x*x + y*y);
        std::cout << "r_check: " << r_check << " expected: " << r << '\n';

        vertices.at(i) = {x, y};

        std::cout << "P" << i+1 << ": " << x << ", " << y << '\n';

        _polegap_vertices.at(i) = occ::addPoint(x, y, 0);
        i++;

    }

}



void RotorBuilders::build_polegap_profile() {

    int polegap_l12 = occ::addLine(_polegap_vertices.at(0), _polegap_vertices.at(1));

    // Arc P2->P3: the polegap opening at r_mo is an arc centered at origin, not taking chord
    int polegap_c23  = occ::addCircleArc(
        _polegap_vertices.at(1), 
        _ob.p_origin().second, 
        _polegap_vertices.at(2)
    );

    int polegap_l34 = occ::addLine(_polegap_vertices.at(2), _polegap_vertices.at(3));

    // Arc P4->P1: the polegap opening at r_ri is an arc centered at origin, not taking chord
    int polegap_c41  = occ::addCircleArc(
        _polegap_vertices.at(3), 
        _ob.p_origin().second, 
        _polegap_vertices.at(0)
    );

    int c_polegap = occ::addCurveLoop({
        polegap_l12, polegap_c23, polegap_l34, polegap_c41
    });

    _s_polegap = occ::addPlaneSurface({c_polegap});

}



void RotorBuilders::replicate_polegaps() {

    _all_polegaps.push_back({2, _s_polegap});

    for (int k = 1; k < _r.n_m; ++k) {

        std::vector<std::pair<int, int>> rotated;

        occ::copy({{2, _s_polegap}}, rotated);
        occ::rotate(
            rotated,
            0, 0, 0,
            0, 0, 1,
            k * _r.theta_p()
        );

        _all_polegaps.push_back(rotated.at(0));

    }

}



void RotorBuilders::cut_polegap_profiles() {

    std::vector<std::vector<std::pair<int,int>>> polegap_cut_map;

    occ::cut(
        {{2, _s_rotor}},
        _all_polegaps,
        _s_rotor_cut,
        polegap_cut_map
    );

}



void RotorBuilders::build() {

    build_rotor_back_annulus();
    build_rotor_pm_annulus();
    build_polegap_vertices();
    build_polegap_profile();
    replicate_polegaps();
    cut_polegap_profiles();

}


std::string RotorBuilders::field_yoke() const {

    // lc(r) = lcmin​ + ((lcmax ​− lcmin​) * (r - r_min / (​r_max ​− r_min))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({0} - {1}) / ({2} - {1})) "
        "* (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2))"
        "+ 1e8 * (1 - (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _r.r_mo(),
        _r.r_ro,
        _r.yoke_mesh_min,
        _r.yoke_mesh_max - _r.yoke_mesh_min
    );

}


std::string RotorBuilders::field_pm() const {

    // lc(r) = lcmin​ + ((lcmax ​− lcmin​) * (r - r_min / (​r_max ​− r_min))​)
    // r = sqrt(x*x + y*y)
    return std::format(
        "({3} + {4} * ({0} - {1}) / ({2} - {1})) "
        "* (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2))"
        "+ 1e8 * (1 - (((Sign({0} - {1}) + 1) / 2) * ((Sign({2} - {0}) + 1) / 2)))",
        "sqrt(x*x+y*y)",
        _r.r_ri,
        _r.r_mo(),
        _r.pm_mesh_min,
        _r.pm_mesh_max - _r.pm_mesh_min
    );

}




std::pair<int, int> RotorBuilders::s_rotor_back() const {
    return {2, _s_rotor_back};
}



std::vector<std::pair<int,int>> RotorBuilders::s_rotor_cut() const {
    return _s_rotor_cut;
}


