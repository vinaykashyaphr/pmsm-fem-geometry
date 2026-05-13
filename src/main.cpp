# include <gmsh.h>

# include "stator/builders.hpp"
# include "airgap/builders.hpp"
# include "params.hpp"
# include "stator/derived_params.hpp"
# include "mesh_builder.hpp"



int main() {

    StatorParams stator_params;
    DerivedStatorParams derived_stator_params;
    AirGapParams airgap_params;
    RotorParams rotor_params;

    gmsh::initialize();
    gmsh::model::add("outrunner");

    StatorBuilders stator_builders(derived_stator_params);
    stator_builders.build();

    AirGapBuilders airgap_builders(stator_params, airgap_params, rotor_params);
    airgap_builders.build();

    gmsh::model::occ::synchronize();
    
    MeshBuilder mesh;

    mesh.add_region(stator_builders.field());
    mesh.add_region(airgap_builders.field());
    mesh.generate();

    gmsh::fltk::run();
    gmsh::finalize();

    // int g_origin = model::addPhysicalGroup(0, {p_origin});
    // model::setPhysicalName(0, g_origin, "Origin");

    // int g_stator_disc = model::addPhysicalGroup(2, {s_stator_slotted});
    // model::setPhysicalName(2, g_stator_disc, "Stator Disc");

    // int g_slot_p1 = model::addPhysicalGroup(0, {slot_point_tags.at(0)});
    // model::setPhysicalName(0, g_slot_p1, "P1");

    // int g_slot_p2 = model::addPhysicalGroup(0, {slot_point_tags.at(1)});
    // model::setPhysicalName(0, g_slot_p2, "P2");

    // int g_slot_p3 = model::addPhysicalGroup(0, {slot_point_tags.at(2)});
    // model::setPhysicalName(0, g_slot_p3, "P3");

    // int g_slot_p4 = model::addPhysicalGroup(0, {slot_point_tags.at(3)});
    // model::setPhysicalName(0, g_slot_p4, "P4");

    // int g_slot_p5 = model::addPhysicalGroup(0, {slot_point_tags.at(4)});
    // model::setPhysicalName(0, g_slot_p5, "P5");

    // int g_slot_p6 = model::addPhysicalGroup(0, {slot_point_tags.at(5)});
    // model::setPhysicalName(0, g_slot_p6, "P6");

    // int g_slot_p7 = model::addPhysicalGroup(0, {slot_point_tags.at(6)});
    // model::setPhysicalName(0, g_slot_p7, "P7");

    // int g_slot_p8 = model::addPhysicalGroup(0, {slot_point_tags.at(7)});
    // model::setPhysicalName(0, g_slot_p8, "P8");

}