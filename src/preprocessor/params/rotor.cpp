# include <numbers>

# include "preprocessor/params/rotor.hpp"
# include "preprocessor/utils/utils.hpp"

# define LOG(var, unit) Logger::get().log(var, #var, unit)



DerivedRotorConfig::DerivedRotorConfig(const RotorConfig& cfg):
    _cfg(cfg)
{

    Logger& logger = Logger::get();
    logger.set_context("DERIVED ROTOR PARAM");
    logger.set_class(Config::tag.rotor);

    compute();

}



void DerivedRotorConfig::compute_theta_p() {

    const double theta_p = (2.0 * std::numbers::pi) / _cfg.n_m;
    LOG(theta_p, "deg");

    _theta_p = theta_p;

}



void DerivedRotorConfig::compute_r_mo() {

    const double r_mo = _cfg.r_ri + _cfg.l_m;
    LOG(r_mo, "mm");

    _r_mo = r_mo;

}



void DerivedRotorConfig::compute() {

    compute_theta_p();
    compute_r_mo();

}



double DerivedRotorConfig::theta_p() const {
    return _theta_p;
}



double DerivedRotorConfig::r_mo() const {
    return _r_mo;
}



# undef LOG

