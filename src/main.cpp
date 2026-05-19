# include <filesystem>
# include <stdexcept>

# include "preprocessor/preprocessor.hpp"


namespace fs = std::filesystem;



int main(int argc, char* argv[]) {

    if (argc > 3)
        throw std::invalid_argument(
            "Usage: " + std::string(argv[0]) + "<input.toml> <output.msh|output.vtk>\n"
        );

    const fs::path config_filepath = (argc >= 2) ? argv[1] : "outrunner.toml";
    const fs::path output_filepath = (argc == 3) ? argv[2] : "outrunner.msh";

    Preprocessor pre_processor(config_filepath);
    pre_processor.run();
    pre_processor.export_mesh(output_filepath);
    pre_processor.view_mesh();

}


