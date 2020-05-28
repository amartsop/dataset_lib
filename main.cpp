#include <iostream>
#include <fstream>
#include <vector>
#include "./include/axial_force_dataset/axial_force_dataset.hpp"


int main(int argc, char *argv[])
{

    std::vector<std::string> data_vec = {"Data0", "Data1"};

    // std::string data_vec = "Data1";
    
    AxialForceDataset axial_data[2];

    
    for (int i = 0; i < sizeof(axial_data) / sizeof(axial_data[0]); i++)
    {
        axial_data[i].data_parsing(data_vec[i]);

        arma::fvec time = axial_data[i].get_time();
        arma::fvec displacemt_x = axial_data[i].get_displ_x();


        if (axial_data[i].is_tissue_biological())
        {
            auto tissue_description = axial_data[i].get_tissue_description();
            int organ = axial_data[i].bio_tissue_organ_index;
            std::cout << tissue_description[organ][1] << std::endl;
        }
    }


    


    return 0;
}

