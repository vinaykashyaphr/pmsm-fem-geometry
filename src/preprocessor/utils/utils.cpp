# include <iostream>
# include <numbers>
# include <sstream>

# include "preprocessor/utils/utils.hpp"



Logger& Logger::get() {

    // Created only once during execution
    static Logger instance;
    return instance;

}



void Logger::log(const double& value, const std::string& name, const std::string& unit) {

    const double qty = (unit == "deg") ? utils::rad2deg(value) : value;

    std::stringstream m;
    m << "[" << _comment << "]" << '\t' << _cls << ":" << '\t' << name << " (" << unit << "):" << '\t' << qty << '\n';
    std::cout << m.str();

};



void Logger::set_context(const std::string& comment) {
    _comment = comment;
}



void Logger::set_class(const std::string& cls) {
    _cls = cls;
}



double utils::deg2rad(double deg) {
    return deg * (std::numbers::pi / 180);
}



double utils::rad2deg(double rad) {
    return rad * (180 / std::numbers::pi);
}


