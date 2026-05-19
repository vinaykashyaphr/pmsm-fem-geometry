# include <gmsh.h>

# include "preprocessor/preprocessor.hpp"
# include "preprocessor/params/config.hpp"
# include "preprocessor/params/params.hpp"
# include "preprocessor/params/stator.hpp"
# include "preprocessor/params/rotor.hpp"
# include "preprocessor/processors/meshing.hpp"
# include "preprocessor/processors/naming.hpp"




GmshSession::GmshSession(const std::string& model_name) {
    gmsh::initialize();
    gmsh::model::add(model_name);
}



GmshSession::~GmshSession() {
    gmsh::finalize();
}



Preprocessor::Preprocessor(const fs::path& config_file): 
    _params(configure(fs::absolute(config_file).string())),
    _gmsh(config_file.stem().string()),
    _builders(_params)
{}



void Preprocessor::run() {
    synchronize();
    optimize();
    synchronize();
    mesh();
    name();
}




Params Preprocessor::configure(const fs::path& config_file) {

    const Config cfg(config_file);
    const DerivedStatorConfig scfg(cfg.stator);
    const DerivedRotorConfig rcfg(cfg.rotor);

    return {cfg, scfg, rcfg};

}



void Preprocessor::optimize() {

    std::vector<std::pair<int,int>> all_surfaces = {
        _builders.abuilder.s_airgap(),
        _builders.rbuilder.s_rotor_back(),
    };

    const auto& stator_slots = _builders.sbuilder.s_stator_slotted();
    const auto& rotor_magnets = _builders.rbuilder.s_rotor_cut();

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



void Preprocessor::synchronize() {
    gmsh::model::occ::synchronize();
}



void Preprocessor::mesh() {

    MeshBuilder mesh;
    mesh.add_region(_builders.sbuilder.field());
    mesh.add_region(_builders.abuilder.field());
    mesh.add_region(_builders.rbuilder.field_pm());
    mesh.add_region(_builders.rbuilder.field_yoke());
    mesh.generate();

}



void Preprocessor::name() {

    GroupNamer namer(_builders);
    namer.start_naming();

}



void Preprocessor::export_mesh(const fs::path& msh_file) const {
    gmsh::write(fs::absolute(msh_file).string());
}



void Preprocessor::view_mesh() const {
    gmsh::fltk::run();
}



