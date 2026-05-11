# include <cmath>
# include <gmsh.h>
# include <array>
#include <ios>
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
    const int k = 1;

    const double h_sb = w_tb;

    gmsh::model::add("in-runner");


    //  STATOR DISC
    int p_origin = occ::addPoint(origin.at(0), origin.at(1), origin.at(2));

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


    // STATOR SLOT


    // P1
    double d_tb = sqrt((pow(r_si, 2.0)) - pow((w_tb/2.0), 2.0));

    int slot_p1 = occ::addPoint((w_tb/2.0), d_tb + w_tb, 0);


    // P3
    double theta_o = 2.0 * asin(w_s / (2.0 * r_so));
    double theta_p = (2.0 * std::numbers::pi) / n_s;
    double theta_t = theta_p - theta_o;
    double r_tm = r_so - d1;
    double d_tm = r_tm * cos(theta_t / 2.0);
    double c_tm = 2.0 * sqrt(pow(r_tm, 2.0) - pow(d_tm, 2.0));

    int slot_p3 = occ::addPoint(c_tm / 2.0, d_tm, 0);


    // P4
    double s_o = (2.0 * r_so) * asin(w_s / (2.0 * r_so));
    double s_p = theta_p * r_so;
    double s_t = s_p - s_o;
    double c_t = (2.0 * r_so) * sin(s_t / (2.0 * r_so));
    double d_t = sqrt(pow(r_so, 2.0) - pow(c_t / 2.0, 2.0));

    int slot_p4 = occ::addPoint(c_t / 2.0, d_t, 0);


    // P5
    int slot_p5 = occ::addPoint(
        (r_so * cos(theta_o / 2.0)) * cos((std::numbers::pi / 2.0) - (theta_t / 2.0) - (theta_o / 2.0)), 
        (r_so * cos(theta_o / 2.0)) * sin( (std::numbers::pi / 2.0) - (theta_t / 2.0) - (theta_o / 2.0)), 
        0
    );

    
    // P2
    double delta_y = (0.5 * (c_tm - w_tb)) * tan(d2);

    int slot_p2 = occ::addPoint((w_tb / 2.0), (d_tm - delta_y), 0);


    

    occ::synchronize();

    int g_origin = model::addPhysicalGroup(0, {p_origin});
    model::setPhysicalName(0, g_origin, "Origin");

    int g_stator_disc = model::addPhysicalGroup(2, {s_stator});
    model::setPhysicalName(2, g_stator_disc, "Stator Disc");

    int g_slot_p1 = model::addPhysicalGroup(0, {slot_p2});
    model::setPhysicalName(0, g_slot_p1, "P2");

    // int g_slot_p2 = model::addPhysicalGroup(0, {slot_p2});
    // model::setPhysicalName(0, g_slot_p2, "P2");

    // int g_slot_p3 = model::addPhysicalGroup(0, {slot_p3});
    // model::setPhysicalName(0, g_slot_p3, "P3");

    // int g_slot_p4 = model::addPhysicalGroup(0, {slot_p4});
    // model::setPhysicalName(0, g_slot_p4, "P4");

    // int g_slot_p5 = model::addPhysicalGroup(0, {slot_p5});
    // model::setPhysicalName(0, g_slot_p4, "P5");

    gmsh::model::mesh::generate(2);

    gmsh::fltk::run();
    gmsh::finalize();

}