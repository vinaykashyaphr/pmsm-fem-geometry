# include <cmath>
#include <cstddef>
# include <gmsh.h>
# include <array>
#include <iostream>
# include <numbers>
# include <utility>
# include <vector>






int main() {

    gmsh::initialize();

    namespace model = gmsh::model;
    namespace occ = model::occ;

    const std::array<double, 3> origin = {0.0, 0.0, 0.0};
    const double r_so = 80.0;
    const double r_si = 27.0;
    const double w_tb = 11.0;
    const double w_s = 2.8;
    const double d1 = 5.0;
    const double d2 = 15 * (std::numbers::pi / 180);
    const int n_s = 18;

    gmsh::model::add("out-runner");

    // Origin
    int p_origin = occ::addPoint(origin.at(0), origin.at(1), origin.at(2));

    // Stator Disc
    int s_stator_outer = occ::addDisk(
        origin.at(0), 
        origin.at(1), 
        origin.at(2), 
        r_so,
        r_so
    );

    int s_stator_inner = occ::addDisk(
        origin.at(0),
        origin.at(1),
        origin.at(2),
        r_si,
        r_si
    );

    std::vector<std::pair<int, int>> stator_cut_result;
    std::vector<std::vector<std::pair<int, int>>> stator_cut_result_map;

    occ::cut(
        {{2, s_stator_outer}},
        {{2, s_stator_inner}},
        stator_cut_result,
        stator_cut_result_map
    );

    int s_stator = stator_cut_result[0].second;

    // Half Stator Slot

    // dependent variables
    const double theta_p = (2.0 * std::numbers::pi) / n_s;
    std::cout << "Pitch angle (theta_p): " << theta_p << '\n';

    const double theta_o = 2.0 * asin(w_s / (2.0 * r_so));
    std::cout << "Slot opening angle (theta_o): " << theta_o << '\n';

    const double theta_s = theta_p - theta_o;
    std::cout << "Tooth tip angle (theta_s): " << theta_s << '\n';

    const double r_sm = r_so - d1;
    std::cout << "Tooth mid radius (r_sm): " << r_sm << '\n';

    const double c_sm = 2.0 * r_sm * sin(theta_s / 2.0);
    std::cout << "Tooth mid chord (c_sm): " << c_sm << '\n';

    const double delta_x = 0.5 * (c_sm - w_tb);
    std::cout << "Horizontal dist b/w edge of tooth body to edge of tooth mid (delta_x): " << delta_x << '\n';

    const double delta_y = delta_x * tan(d2);
    std::cout << "Verical dist b/w stator neck to tooth mid (delta_y): " << delta_y << '\n';

    const double r_sn = r_so - d1 - delta_y;
    std::cout << "Stator neck radius (r_sn): " << r_sn << '\n';

    const double theta_sn = 2.0 * asin(w_tb / (2.0 * r_sn));
    std::cout << "Stator neck angle (theta_sn): " << theta_sn << '\n';

    const double r_sb = w_tb + r_si;
    std::cout << "Stator base radius (r_sb): " << r_sb << '\n';

    const double theta_sb = 2.0 * asin(w_tb / (2.0 * r_sb));
    std::cout << "Stator base angle (theta_sb): " << theta_sb << '\n';

    // coordinates

    const std::array<std::pair<double, double>, 8> slot_pt_data = {{
        {r_sb, theta_sb / 2.0},
        {r_sn, theta_sn / 2.0},
        {r_sm, theta_s / 2.0},
        {r_so, theta_s / 2.0},
        {r_so, theta_p / 2.0},
        {r_so, theta_p - (theta_s / 2.0)},
        {r_sm, theta_p - (theta_s / 2.0)},
        {r_sn, theta_p - (theta_sn / 2.0)},
    }};

    std::array<std::pair<double, double>, 8> slot_points{};
    std::array<int, 8> slot_point_tags{};

    std::size_t i = 0;
    for (const auto& [r, theta] : slot_pt_data) {

        double x = (r * sin(theta));
        double y = (r * cos(theta));

        if (r == r_so) {
            double r_actual = sqrt(x*x + y*y);
            x = x * (r_so / r_actual);
            y = y * (r_so / r_actual);
        }

        slot_points.at(i) = {x, y};

        std::cout << "P" << i+1 << ": " << x << ", " << y << '\n';

        slot_point_tags.at(i) = occ::addPoint(x, y, 0);
        i++;

    }

    // std::cout << sqrt(pow(15.2530, 2) + pow(78.4569, 2)) << '\n';


    // // Slot lines

    int slot_l12 = occ::addLine(slot_point_tags.at(0), slot_point_tags.at(1));
    int slot_l23 = occ::addLine(slot_point_tags.at(1), slot_point_tags.at(2));
    int slot_l34 = occ::addLine(slot_point_tags.at(2), slot_point_tags.at(3));

    // Arc P4->P5->P6 along r_so, centered at origin
    int slot_open_arc  = occ::addCircleArc(slot_point_tags.at(3), p_origin, slot_point_tags.at(5));

    int slot_l67 = occ::addLine(slot_point_tags.at(5), slot_point_tags.at(6));
    int slot_l78 = occ::addLine(slot_point_tags.at(6), slot_point_tags.at(7));
    int slot_l81 = occ::addLine(slot_point_tags.at(7), slot_point_tags.at(0));

    int c_slot = occ::addCurveLoop({
        slot_l12, slot_l23, slot_l34,
        slot_open_arc,
        slot_l67, slot_l78, slot_l81
    });

    int s_slot = occ::addPlaneSurface({c_slot});


    std::vector<std::pair<int, int>> all_slots{{2, s_slot}};
    
    for (int k = 1; k < n_s; ++k) {

        std::vector<std::pair<int, int>> rotated;
        occ::copy({{2, s_slot}}, rotated);
        occ::rotate(
            rotated,
            0, 0, 0,
            0, 0, 1,
            k * theta_p
        );

        all_slots.push_back(rotated.at(0));

    }

    std::vector<std::pair<int,int>> slot_cut_result;
    std::vector<std::vector<std::pair<int,int>>> slot_cut_map;

    occ::cut(
        {{2, s_stator}},
        all_slots,
        slot_cut_result,
        slot_cut_map
    );

    int s_stator_slotted = slot_cut_result[0].second;

    occ::synchronize();


    int g_origin = model::addPhysicalGroup(0, {p_origin});
    model::setPhysicalName(0, g_origin, "Origin");

    int g_stator_disc = model::addPhysicalGroup(2, {s_stator_slotted});
    model::setPhysicalName(2, g_stator_disc, "Stator Disc");

    int g_slot_p1 = model::addPhysicalGroup(0, {slot_point_tags.at(0)});
    model::setPhysicalName(0, g_slot_p1, "P1");

    int g_slot_p2 = model::addPhysicalGroup(0, {slot_point_tags.at(1)});
    model::setPhysicalName(0, g_slot_p2, "P2");

    int g_slot_p3 = model::addPhysicalGroup(0, {slot_point_tags.at(2)});
    model::setPhysicalName(0, g_slot_p3, "P3");

    int g_slot_p4 = model::addPhysicalGroup(0, {slot_point_tags.at(3)});
    model::setPhysicalName(0, g_slot_p4, "P4");

    int g_slot_p5 = model::addPhysicalGroup(0, {slot_point_tags.at(4)});
    model::setPhysicalName(0, g_slot_p5, "P5");

    int g_slot_p6 = model::addPhysicalGroup(0, {slot_point_tags.at(5)});
    model::setPhysicalName(0, g_slot_p6, "P6");

    int g_slot_p7 = model::addPhysicalGroup(0, {slot_point_tags.at(6)});
    model::setPhysicalName(0, g_slot_p7, "P7");

    int g_slot_p8 = model::addPhysicalGroup(0, {slot_point_tags.at(7)});
    model::setPhysicalName(0, g_slot_p8, "P8");

    gmsh::model::mesh::generate(2);
    gmsh::fltk::run();
    gmsh::finalize();

}