# include <gmsh.h>

# include "origin/builders.hpp"
# include "rotor/builders.hpp"
# include "rotor/derived_params.hpp"
# include "stator/builders.hpp"
# include "airgap/builders.hpp"
# include "params.hpp"
# include "stator/derived_params.hpp"
# include "mesh_builder.hpp"
# include "naming.hpp"



namespace model = gmsh::model;


int main() {

    params::StatorParams    stator_params;
    params::AirGapParams    airgap_params;
    params::RotorParams     rotor_params;
    params::OriginParams    origin_params;
    DerivedStatorParams     derived_stator_params;
    DerivedRotorParams      derived_rotor_params;

    gmsh::initialize();
    model::add("outrunner");

    OriginBuilder origin_builder(origin_params);
    origin_builder.build();

    StatorBuilders stator_builders(derived_stator_params, origin_params, origin_builder);
    stator_builders.build();

    AirGapBuilders airgap_builders(origin_params, stator_params, airgap_params, rotor_params);
    airgap_builders.build();

    RotorBuilders rotor_builders(origin_params, origin_builder, derived_rotor_params);
    rotor_builders.build();

    std::vector<std::pair<int,int>> all_surfaces = {
        airgap_builders.s_airgap(),
        rotor_builders.s_rotor_back(),
    };

    const auto& stator_slots = stator_builders.s_stator_slotted();
    const auto& rotor_magnets = rotor_builders.s_rotor_cut();

    all_surfaces.insert(all_surfaces.end(), rotor_magnets.begin(), rotor_magnets.end());
    all_surfaces.insert(all_surfaces.end(), stator_slots.begin(), stator_slots.end());

    std::vector<std::pair<int,int>> fragment_out;
    std::vector<std::vector<std::pair<int,int>>> fragment_map;

    gmsh::model::occ::fragment(
        all_surfaces,
        {},                  // no tool surfaces, fragment is symmetric
        fragment_out,
        fragment_map
    );

    model::occ::synchronize();
    
    MeshBuilder mesh;
    mesh.add_region(stator_builders.field());
    mesh.add_region(airgap_builders.field());
    mesh.add_region(rotor_builders.field_pm());
    mesh.add_region(rotor_builders.field_yoke());
    mesh.generate();

    GroupNamer namer(origin_builder, stator_builders, airgap_builders);
    namer.start_naming();

    gmsh::fltk::run();
    gmsh::finalize();

}


