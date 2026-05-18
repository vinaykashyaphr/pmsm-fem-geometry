# include "preprocessor/preprocessor.hpp"
# include "preprocessor/params/config.hpp"
# include "preprocessor/params/stator.hpp"
# include "preprocessor/params/rotor.hpp"


Preprocessor::Preprocessor(const fs::path& config_file, const fs::path& msh_file):
    _cfg_file(fs::absolute(config_file).string()), 
    _msh_file(fs::absolute(msh_file).string())
{}


Params Preprocessor::configure() {

    const Config cfg(_cfg_file);
    const DerivedStatorConfig scfg(cfg.stator);
    const DerivedRotorConfig rcfg(cfg.rotor);

    return {cfg, scfg, rcfg};
}