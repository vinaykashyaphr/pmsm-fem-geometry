# include <gmsh.h>
# include <array>



int main() {

    gmsh::initialize();

    namespace model = gmsh::model;
    namespace geo = model::geo;
    namespace occ = model::occ;

    std::array<double, 3> origin = {0.0, 0.0, 0.0};
    double stator_ext_dia = 125.0;

    gmsh::model::add("in-runner");

    int point_origin = geo::addPoint(origin.at(0), origin.at(1), origin.at(2));
    int curve_stator_outer_dia = occ::addDisk(origin.at(0), origin.at(1), origin.at(2))

    geo::synchronize();

    int group_origin = model::addPhysicalGroup(0, {point_origin}, 1);
    model::setPhysicalName(0, group_origin, "origin");

    gmsh::fltk::run();
    gmsh::finalize();

}