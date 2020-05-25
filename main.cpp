#include <iostream>
#include <fstream>
#include <vector>
#include "./include/axial_force_dataset/axial_force_dataset.hpp"


int main(int argc, char *argv[])
{

    std::vector<std::string> data_vec = {"Data1"};

    // std::string data_vec = "Data1";
    
    AxialForceDataset axial_data[1];

    for (int i = 0; i < sizeof(axial_data) / sizeof(axial_data[0]); i++)
    {
        axial_data[i].get_data(data_vec[i]);
    }


    return 0;
}

