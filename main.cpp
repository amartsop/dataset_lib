#include <iostream>
#include <fstream>
#include <vector>
#include "./include/axial_force_dataset/axial_force_dataset.hpp"

int main(int argc, char *argv[])
{

    std::vector<std::string> data_vec = {"Data0"};

    
    AxialForceDataset axial_data(data_vec);

    return 0;
}

