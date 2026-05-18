
# include <string>



namespace utils {

    double deg2rad(double deg);
    double rad2deg(double rad);

}



class Logger {

    std::string _comment = "STATIC PARAM";
    std::string _cls;

    Logger() = default;

    public:

        static Logger& get();

        void set_context(const std::string& comment);
        void set_class(const std::string& cls);
        void log(const double& value, const std::string& name, const std::string& unit);

};


