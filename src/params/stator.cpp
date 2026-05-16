# include <cmath>
# include <numbers>

# include "params/stator.hpp"
# include "config/config.hpp"
# include "utils/utils.hpp"

# define LOG(var, unit) Logger::get().log(var, #var, unit)



DerivedStatorConfig::DerivedStatorConfig(const StatorConfig& cfg): 
    _cfg(cfg)
{

    Logger& logger = Logger::get();
    logger.set_context("DERIVED STATOR PARAM");
    logger.set_class(Config::tag.stator);

    compute();

}



void DerivedStatorConfig::compute_theta_p() {

    const double theta_p = (2.0 * std::numbers::pi) / _cfg.n_s;
    LOG(theta_p, "deg");

    _theta_p = theta_p;

}



void DerivedStatorConfig::compute_theta_o() {

    const double theta_o = 2.0 * asin(_cfg.w_s / (2.0 * _cfg.r_so));
    LOG(theta_o, "deg");

    _theta_o = theta_o;

}



void DerivedStatorConfig::compute_theta_s() {

    const double theta_s = _theta_p - _theta_o;
    LOG(theta_s, "deg");

    _theta_s = theta_s;

}



void DerivedStatorConfig::compute_r_sm() {

    const double r_sm = _cfg.r_so - _cfg.d1;
    LOG(r_sm, "mm");

    _r_sm = r_sm;

}



void DerivedStatorConfig::compute_c_sm() {

    const double c_sm = 2.0 * _r_sm * sin(_theta_s / 2.0);
    LOG(c_sm, "mm");

    _c_sm = c_sm;

}



void DerivedStatorConfig::compute_delta_x() {

    const double delta_x = 0.5 * (_c_sm - _cfg.w_tb);
    LOG(delta_x, "mm");

    _delta_x = delta_x;

}



void DerivedStatorConfig::compute_delta_y() {

    const double delta_y = _delta_x * tan(_cfg.d2);
    LOG(delta_y, "mm");

    _delta_y = delta_y;

}



void DerivedStatorConfig::compute_r_sn() {

    const double r_sn = _cfg.r_so - _cfg.d1 - _delta_y;
    LOG(r_sn, "mm");

    _r_sn = r_sn;

}



void DerivedStatorConfig::compute_theta_sn() {

    const double theta_sn = 2.0 * asin(_cfg.w_tb / (2.0 * _r_sn));
    LOG(theta_sn, "deg");

    _theta_sn = theta_sn;

}



void DerivedStatorConfig::compute_r_sb() {

    const double r_sb = _cfg.w_tb + _cfg.r_si;
    LOG(r_sb, "mm");

    _r_sb = r_sb;

}



void DerivedStatorConfig::compute_theta_sb() {

    const double theta_sb = 2.0 * asin(_cfg.w_tb / (2.0 * _r_sb));
    LOG(theta_sb, "deg");

    _theta_sb = theta_sb;

}



void DerivedStatorConfig::compute() {

    compute_theta_p();
    compute_theta_o();
    compute_theta_s();
    compute_r_sm();
    compute_c_sm();
    compute_delta_x();
    compute_delta_y();
    compute_r_sn();
    compute_theta_sn();
    compute_r_sb();
    compute_theta_sb();

}



double DerivedStatorConfig::theta_p() const {
    return _theta_p;
};



double DerivedStatorConfig::theta_o() const {
    return _theta_o;
};



double DerivedStatorConfig::theta_s() const {
    return _theta_s;
};



double DerivedStatorConfig::r_sm() const {
    return _r_sm;
};



double DerivedStatorConfig::c_sm() const {
    return _c_sm;
};



double DerivedStatorConfig::delta_x() const {
    return _delta_x;
};



double DerivedStatorConfig::delta_y() const {
    return _delta_y;
};



double DerivedStatorConfig::r_sn() const {
    return _r_sn;
};



double DerivedStatorConfig::theta_sn() const {
    return _theta_sn;
};



double DerivedStatorConfig::r_sb() const {
    return _r_sb;
};



double DerivedStatorConfig::theta_sb() const {
    return _theta_sb;
};


# undef LOG


