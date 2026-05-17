# include <filesystem>
# include <stdexcept>

# include <gmsh.h>

# include "params/config.hpp"
# include "params/params.hpp"

# include "params/stator.hpp"
# include "params/rotor.hpp"

# include "builders/origin.hpp"
# include "builders/stator.hpp"
# include "builders/airgap.hpp"
# include "builders/rotor.hpp"

# include "processors/meshing.hpp"
# include "processors/naming.hpp"


namespace model = gmsh::model;
namespace fs = std::filesystem;



Params configure(const fs::path& config_file) {

    const Config cfg(fs::absolute(config_file).string());
    const DerivedStatorConfig scfg(cfg.stator);
    const DerivedRotorConfig rcfg(cfg.rotor);

    return {cfg, scfg, rcfg};
}



Builders build(const Config& cfg, const DerivedStatorConfig& scfg, const DerivedRotorConfig& rcfg) {

    OriginBuilder origin_builder(cfg.origin);
    origin_builder.build();

    StatorBuilder stator_builder(cfg, scfg, origin_builder);
    stator_builder.build();

    AirGapBuilder airgap_builder(cfg);
    airgap_builder.build();

    RotorBuilder rotor_builder(cfg, rcfg, origin_builder);
    rotor_builder.build();

    return {
        origin_builder, 
        stator_builder, 
        airgap_builder, 
        rotor_builder
    };

}


void optimize(const Builders& builders) {

    std::vector<std::pair<int,int>> all_surfaces = {
        builders.abuilder.s_airgap(),
        builders.rbuilder.s_rotor_back(),
    };

    const auto& stator_slots = builders.sbuilder.s_stator_slotted();
    const auto& rotor_magnets = builders.rbuilder.s_rotor_cut();

    all_surfaces.insert(all_surfaces.end(), rotor_magnets.begin(), rotor_magnets.end());
    all_surfaces.insert(all_surfaces.end(), stator_slots.begin(), stator_slots.end());

    std::vector<std::pair<int,int>> fragment_out;
    std::vector<std::vector<std::pair<int,int>>> fragment_map;

    // no tool surfaces, fragment is symmetric
    gmsh::model::occ::fragment(
        all_surfaces,
        {},
        fragment_out,
        fragment_map
    );

}



void mesh(const Builders& builders) {

    MeshBuilder mesh;
    mesh.add_region(builders.sbuilder.field());
    mesh.add_region(builders.abuilder.field());
    mesh.add_region(builders.rbuilder.field_pm());
    mesh.add_region(builders.rbuilder.field_yoke());
    mesh.generate();

}


void name(const Builders& builders) {

    GroupNamer namer(builders);
    namer.start_naming();

}



int main(int argc, char* argv[]) {

    if (argc > 3)
        throw std::invalid_argument(
            "Usage: " + std::string(argv[0]) + "<input.toml> <output.msh|output.vtk>\n"
        );

    const fs::path config_filepath = (argc >= 2) ? argv[1] : "outrunner.toml";
    const fs::path output_filepath = (argc == 3) ? argv[2] : "";

    Params params = configure(config_filepath);

    gmsh::initialize();
    model::add(config_filepath.stem());

    Builders builders = build(params.cfg, params.scfg, params.rcfg);
    optimize(builders);

    model::occ::synchronize();

    mesh(builders);
    name(builders);

    if (!output_filepath.empty()) gmsh::write(output_filepath.string());
    else gmsh::fltk::run();

    gmsh::finalize();

}


