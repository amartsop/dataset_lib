#include <iostream>
#include <fstream>
#include <vector>
#include "./include/nlohmann/json.hpp"


class AxialForceDataset
{
public:
    AxialForceDataset(); 
    ~AxialForceDataset();

    // Getters
    u_int64_t get_dataset_size(void) { return m_dataset_size; }
    void get_data(std::string data_id);

private:

    const std::string m_lib_rel_path = "./include/axial_force_dataset/";
    const std::string m_share_rel_dir = m_lib_rel_path + "share/";
    const std::string m_data_file = m_share_rel_dir + "axial_force_data.json";

    // File handlers
    std::ifstream m_file;
    nlohmann::json m_j_file;

private:
    
    void parse_source_section(nlohmann::json &val);
    void parse_needle_section(nlohmann::json &val);
    void parse_tissue_section(nlohmann::json &val);
    void parse_meas_section(nlohmann::json &val);
    
private:
   
    /* Dataset size */
    u_int64_t m_dataset_size;

    /* Source section variables */
    std::string m_author_name;
    std::string m_paper_title;
    int m_year;
    std::string m_doi;

    /* Needle characteristics section variables */
    float m_needle_diameter; //(G)
    std::string m_tip_type;
    float m_tip_anlge; // (deg)
    std::string m_tip_sharpness; 
    std::string m_tip_lubrication;

    /* Tissue characteristics section variables */
    std::string m_tissue_type;
    int m_layers_num;
    bool m_multilayer;
    std::vector<std::vector<std::string>> m_tissue_desription;
    
    enum biol_tissue_chars
    {
        organ, animal, state
    };

    enum art_tissue_chars
    {
        name, components, concentration
    };

    /* Measurement section variables */
    enum class meas_index
    {
        displ_x, vel_x, rot_x, force_x, total
    };

    enum class meas_depend_index
    {
        time, displ_x, vel_x, rot_x, total
    };
    
    std::string m_meas_types[4];
    std::string m_meas_dependence[4];
    std::string m_meas_file[4];

protected:
    /* Standard types */

    // Standard tip types
    std::string m_tip_types_ans[7] = {"Blunt", "Beveled", "Conical", 
        "Sprotte", "Diamond", "Tuohy", "Uknown"};

    // Sharpness classification
    std::string m_sharpness_ans[3] = {"Sharp", "Blunt", "Uknown"};

    // Possible lubrication states
    std::string m_lubrication_ans[3] = {"Present", "Not present", "Uknown"};

    // Possible tissue types
    std::string m_tissue_types_ans[2] = {"Biological", "Artificial"};

    // Biological tissue organs
    std::string m_location_ans[9] = {"Liver", "Prostate", "Breast", "Brain", 
        "Heart", "Epidural", "Skin", "Muscle", "Fat"};

    // Biological tissue animals
    std::string m_animal_ans[5] = {"Human", "Bovine specimen", 
        "Porcine specimen", "Rabbit specimen", "Chicken specimen"};

    // Biological tissue states
    std::string m_state_ans[2] = {"In vivo", "Ex vivo"};

    // Measurements types
    std::string m_meas_ans[4] = {"Displacement x", "Velocity x", 
        "Rotation x", "Force x"};

    std::string m_meas_dependence_ans[4] = {"Time", "Displacement x",
        "Velocity x", "Rotation x"};
};


AxialForceDataset::AxialForceDataset()
{
    // Read json file
    m_file = std::ifstream(m_data_file);
    m_j_file = nlohmann::json::parse(m_file);
    
    // Dataset file
    m_dataset_size = m_j_file.size();
}


/**************** Methods *****************/

void AxialForceDataset::get_data(std::string data_id)
{
    auto val = m_j_file[data_id];
    parse_source_section(val);
    parse_needle_section(val);
    parse_tissue_section(val);
    parse_meas_section(val);
}


void AxialForceDataset::parse_source_section(nlohmann::json &val)
{
    /* Source section parsing */
    m_author_name = val["Source"]["Author"];
    m_paper_title = val["Source"]["Paper Title"];
    m_year = val["Source"]["Year"];
    m_doi = val["Source"]["DOI"]; 
}


void AxialForceDataset::parse_needle_section(nlohmann::json &val)
{
    /* Needle characteristics section parsing */
    m_needle_diameter = val["Needle Characteristics"]["Needle Diameter"];
    m_tip_type = val["Needle Characteristics"]["Tip Type"];
    m_tip_anlge = val["Needle Characteristics"]["Tip Angle"]; 
    m_tip_sharpness = val["Needle Characteristics"]["Tip Sharpness"];
    m_tip_lubrication = val["Needle Characteristics"]["Tip Lubrication"];
}


void AxialForceDataset::parse_tissue_section(nlohmann::json &val)
{
    /* Tissue characteristics section parsing */
    m_tissue_type = val["Tissue Characteristics"]["Tissue Type"];
    m_layers_num = val["Tissue Characteristics"]["Layers Number"];
    m_multilayer = (m_layers_num > 1);

    auto &tissue_descr = val["Tissue Characteristics"]["Tissue Description"];
    if (m_tissue_type.compare(m_tissue_types_ans[0]) == 0)
    {
        m_tissue_desription.push_back(tissue_descr.at("Organ/Location"));
        m_tissue_desription.push_back(tissue_descr.at("Animal"));
        m_tissue_desription.push_back(tissue_descr.at("State"));
    }

    else {
        m_tissue_desription.push_back(tissue_descr.at("Name"));
        m_tissue_desription.push_back(tissue_descr.at("Components"));
        m_tissue_desription.push_back(tissue_descr.at("Concentration"));
    }
}


void AxialForceDataset::parse_meas_section(nlohmann::json &val)
{
    for (int i = 0; i < static_cast<int>(meas_index::total); i++)
    { 
        auto &meas_handle = val["Measurements"][m_meas_ans[i]];
        m_meas_types[i] = meas_handle.at("Type");
        m_meas_dependence[i] = meas_handle.at("Dependence");
        m_meas_file[i] = meas_handle.at("File");
        std::cout << m_meas_dependence[i] << std::endl;
    }
}



AxialForceDataset::~AxialForceDataset()
{
    m_file.close();
}