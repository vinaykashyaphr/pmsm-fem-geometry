# include "preprocessor/params/config.hpp"
# include "preprocessor/utils/utils.hpp"

# define LOG(var, unit) Logger::get().log(var, #var, unit)


NameGroups Config::tag;



Config::Config(const std::string& config_file):
    _cfg(toml::parse_file(config_file))
{}



OriginConfig::OriginConfig(const toml::table* t):
    x((*t)["x"].value_or(0.0)), 
    y((*t)["y"].value_or(0.0)), 
    z((*t)["z"].value_or(0.0))
{

    Logger::get().set_class(Config::tag.origin);

    LOG(x, "mm");
    LOG(y, "mm");
    LOG(y, "mm");

}



StatorConfig::StatorConfig(const toml::table* t):
    r_so((*t)["r_so"].value<double>().value()),
    r_si((*t)["r_si"].value<double>().value()),
    w_tb((*t)["w_tb"].value<double>().value()),
    w_s((*t)["w_s"].value<double>().value()),
    d1((*t)["d1"].value<double>().value()),
    d2(utils::deg2rad((*t)["d2"].value<double>().value())),
    n_s((*t)["n_s"].value<double>().value()),
    mesh_min((*t)["mesh_min"].value<double>().value()),
    mesh_max((*t)["mesh_max"].value<double>().value())
{

    Logger::get().set_class(Config::tag.stator);

    LOG(r_so, "mm");
    LOG(r_si, "mm");
    LOG(w_tb, "mm");
    LOG(w_s, "mm");
    LOG(d1, "mm");
    LOG(d2, "deg");
    LOG(n_s, "mm");
    LOG(mesh_min, "mm");
    LOG(mesh_max, "mm");

}



AirGapConfig::AirGapConfig(const toml::table* t):
    g((*t)["g"].value<double>().value()),
    mesh_size((*t)["mesh_size"].value<double>().value())
{

    Logger::get().set_class(Config::tag.airgap);

    LOG(g, "mm");
    LOG(mesh_size, "mm");

}



RotorConfig::RotorConfig(const toml::table* t):
    r_ri((*t)["r_ri"].value<double>().value()),
    r_ro((*t)["r_ro"].value<double>().value()),
    l_m((*t)["l_m"].value<double>().value()),
    theta_m(utils::deg2rad((*t)["theta_m"].value<double>().value())),
    n_m((*t)["n_m"].value<double>().value()),
    pm_mesh_min((*t)["pm_mesh_min"].value<double>().value()),
    pm_mesh_max((*t)["pm_mesh_max"].value<double>().value()),
    yoke_mesh_min((*t)["yoke_mesh_min"].value<double>().value()),
    yoke_mesh_max((*t)["yoke_mesh_max"].value<double>().value())
{

    Logger::get().set_class(Config::tag.rotor);

    LOG(r_ri, "mm");
    LOG(r_ro, "mm");
    LOG(l_m, "mm");
    LOG(theta_m, "deg");
    LOG(n_m, "mm");
    LOG(pm_mesh_min, "mm");
    LOG(pm_mesh_max, "mm");
    LOG(yoke_mesh_min, "mm");
    LOG(yoke_mesh_max, "mm");

}


# undef LOG



