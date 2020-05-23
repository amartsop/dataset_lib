#include <iostream>
#include <fstream>
#include <vector>
#include "./include/nlohmann/json.hpp"


class AxialForceDataset
{
public:
    AxialForceDataset(std::string data_id); 
    AxialForceDataset(std::vector<std::string> data_ids);
    AxialForceDataset();
    ~AxialForceDataset();



private:

    const std::string m_lib_rel_path = "./include/axial_force_dataset/";
    const std::string m_share_rel_dir = m_lib_rel_path + "share/";
    const std::string m_data_file = m_share_rel_dir + "axial_force_data.json";

    // File handlers
    std::ifstream m_file;
    nlohmann::json m_j_file;

private:
    void get_data(std::string data_n);
    void read_json_file(void);
    void parse_json(nlohmann::json &val);

private:

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
};


AxialForceDataset::AxialForceDataset(std::string data_id)
{
    read_json_file();
    get_data(data_id);
}


AxialForceDataset::AxialForceDataset(std::vector<std::string> data_ids)
{
    read_json_file();
    for(std::vector<std::string>::size_type i = 0; i != data_ids.size(); i++ )
    {
        get_data(data_ids[i]);
    }
}


AxialForceDataset::AxialForceDataset()
{
    read_json_file();
}


/**************** Methods *****************/
void AxialForceDataset::read_json_file(void)
{
    // Read json file
    m_file = std::ifstream(m_data_file);
    m_j_file = nlohmann::json::parse(m_file);
}

void AxialForceDataset::get_data(std::string data_n)
{

    auto it_des = m_j_file.find(data_n);
    // nlohmann::json &val = it_des.value();
    parse_json(it_des.value());
}


void AxialForceDataset::parse_json(nlohmann::json &val)
{
    /* Source section parsing */
    m_author_name = val["Source"]["Author"];
    m_paper_title = val["Source"]["Paper Title"];
    m_year = val["Source"]["Year"];
    m_doi = val["Source"]["DOI"]; 

    /* Needle characteristics section parsing */
    m_needle_diameter = val["Needle Characteristics"]["Needle Diameter"];
    m_tip_type = val["Needle Characteristics"]["Tip Type"];
    m_tip_anlge = val["Needle Characteristics"]["Tip Angle"]; 
    m_tip_sharpness = val["Needle Characteristics"]["Tip Sharpness"];
    m_tip_lubrication = val["Needle Characteristics"]["Tip Lubrication"];
         
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
    // std::cout << m_tissue_desription[biol_tissue_chars::organ][] << std::endl;
    }

    else 
    {
        // m_tissue_desription.push_back(tissue_descr.at("Name"));
        // m_tissue_desription.push_back(tissue_descr.at("Components"));
        // m_tissue_desription.push_back(tissue_descr.at("Concentration"));

    // std::cout << m_tissue_desription[biol_tissue_chars::organ][] << std::endl;
    }
}



AxialForceDataset::~AxialForceDataset()
{
    m_file.close();
}