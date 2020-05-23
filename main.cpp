#include <iostream>
#include <fstream>
#include <vector>
#include "./include/axial_force_dataset/axial_force_dataset.hpp"

int main(int argc, char *argv[])
{

    std::vector<std::string> data_vec = {"Data0", "Data1"};
    
    // std::string data_vec = "Data1";
    
    AxialForceDataset axial_data[2];

    for (int i = 0; i < 2; i++)
    {
        axial_data[i].get_data(data_vec[i]);
    }


    return 0;
}

