# pragma once

# include <string>
# include <vector>



class MeshBuilder {

    std::vector<int> _field_ids;

    public:

        explicit MeshBuilder();
        void add_region(const std::string expr);
        void generate(); 

};