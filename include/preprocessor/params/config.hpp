# pragma once

# include <string>

# include <toml.hpp>



struct OriginConfig {

    double x;
    double y;
    double z;

    explicit OriginConfig(const toml::table* t);

};



struct StatorConfig {


    // Stator outer radius
    double r_so;

    // Stator inner radius
    double r_si;
    
    // Stator tooth width
    double w_tb;
    
    // Slot opening width
    double w_s;
    
    // Slot shoe depth
    double d1;
    
    // Slot shoe ramp angle 
    double d2;
    
    // Number of Slots
    int n_s;

    // Mesh sizes
    double mesh_min;
    double mesh_max;

    explicit StatorConfig(const toml::table* t);

};



struct AirGapConfig {

    // Airgap width
    double g;

    // Airgap mesh size
    double mesh_size;

    explicit AirGapConfig(const toml::table* t);

};



struct RotorConfig {

    // Rotor inner radius
    double r_ri;
    
    // Rotor outer radius
    double r_ro;

    // Rotor magnet thickness
    double l_m;

    // Rotor magnet arc
    double theta_m;

    // Number of poles
    double n_m;

    // PM mesh sizes
    double pm_mesh_min;
    double pm_mesh_max;

    // Yoke mesh sizes
    double yoke_mesh_min;
    double yoke_mesh_max;

    explicit RotorConfig(const toml::table* t);

};



struct NameGroups {

    const std::string origin = "origin";
    const std::string stator = "stator";
    const std::string airgap = "airgap";
    const std::string rotor  = "rotor";
    const std::string pole   = "pole";
    const std::string yoke   = "yoke";

};



struct Config {

    explicit Config(const std::string& config_file);
    static void log_param(const double& qty, std::string cls, std::string name, std::string unit);

    static NameGroups tag;

    private:
        toml::parse_result _cfg;

    public:
        OriginConfig origin {_cfg[tag.origin].as_table()};
        StatorConfig stator {_cfg[tag.stator].as_table()};
        AirGapConfig airgap {_cfg[tag.airgap].as_table()};
        RotorConfig  rotor {_cfg[tag.rotor].as_table()};

};


