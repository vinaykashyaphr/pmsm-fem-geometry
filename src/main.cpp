# include <gmsh.h>

# include "stator/builders.hpp"
# include "airgap/builders.hpp"
# include "params.hpp"
# include "stator/derived_params.hpp"
# include "mesh_builder.hpp"
# include "naming.hpp"



namespace model = gmsh::model;


int main() {

    params::StatorParams stator_params;
    params::AirGapParams airgap_params;
    params::RotorParams rotor_params;
    DerivedStatorParams derived_stator_params;

    gmsh::initialize();
    model::add("outrunner");

    StatorBuilders stator_builders(derived_stator_params);
    stator_builders.build();

    AirGapBuilders airgap_builders(stator_params, airgap_params, rotor_params);
    airgap_builders.build();

    model::occ::synchronize();
    
    MeshBuilder mesh;
    mesh.add_region(stator_builders.field());
    mesh.add_region(airgap_builders.field());
    mesh.generate();

    GroupNamer namer(stator_builders, airgap_builders);
    namer.start_naming();

    gmsh::fltk::run();
    gmsh::finalize();

}


