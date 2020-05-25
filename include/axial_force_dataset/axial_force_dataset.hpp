#include <iostream>
#include <fstream>
#include <vector>
#include <armadillo>
#include "include/armaext.hpp"
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
    void measurements_processing(void);
    void linear_extr_correction(arma::fmat *tbe_mat, arma::fmat *full_mat);
    float linear_extrapolation(float tn, arma::fvec t_vec, arma::fvec f_vec);
    void resampling(arma::fmat *matr, float ts);
    // void map_str_to_variable(std::string in_str, arma::fvec x);

private:
   
    /* Dataset size */
    u_int64_t m_dataset_size;

    /* Dataset name */
    std::string m_data_id;

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
    
    enum class biol_tissue_chars
    {
        organ, animal, state
    };

    enum class art_tissue_chars
    {
        name, components, concentration
    };

    /* Measurement section variables */
    enum class meas_index
    {
        displ_x, vel_x, rot_x, force_x, total
    };

    float m_sampling_frequency;
    std::vector<std::vector<std::string>> m_meas_ind_vars;
    std::vector<std::vector<std::string>> m_meas_dep_vars;
    std::vector<std::vector<std::string>> m_meas_file;
    int m_file_num;
    std::vector<arma::fmat> m_x_y;
    arma::fvec m_time;
    arma::fvec m_displ_x;
    arma::fvec m_vel_x;
    arma::fvec m_rot_x;

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
    m_data_id = data_id;
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

    auto &meas_handle = val["Measurements"];
    m_sampling_frequency = meas_handle.at("Sampling Frequency");
    m_meas_ind_vars.push_back(meas_handle.at("Indepedent Variables"));
    m_meas_dep_vars.push_back(meas_handle.at("Depedent Variables"));
    m_meas_file.push_back(meas_handle.at("Files Names"));
    m_file_num = m_meas_file[0].size();

    for(int i = 0; i < m_file_num; i++)
    {
        std::string file = m_share_rel_dir + m_data_id + "/" + m_meas_file[0][i];
        arma::fmat x_y_data;
        x_y_data.load(file, arma::csv_ascii);
        ArmaExt::sortrows<arma::fmat>(&x_y_data, true);
        m_x_y.push_back(x_y_data);
    }
    measurements_processing();
}


void AxialForceDataset::measurements_processing(void)
{
    arma::fmat x_sort(m_file_num, 2);

    for(int i = 0; i < m_file_num; i++)
    {
        arma::fmat x_y_mat =  m_x_y.at(i); arma::fvec x_vec = x_y_mat.col(0);
        x_sort.at(i, 0) = x_vec.back(); x_sort.at(i, 1) = (float) i; 
    }
    ArmaExt::sortrows<arma::fmat>(&x_sort, true);
    
    float sampling_period = 1.0f / m_sampling_frequency;
    int index_max = (float) x_sort.at(x_sort.n_rows-1, 1);

    for(int i = 0; i < m_file_num - 1; i++)
    {
        int index = (float) x_sort.at(i, 1);
        linear_extr_correction(&m_x_y.at(index), &m_x_y.at(index_max));
        resampling(&m_x_y.at(index), sampling_period);
    }

    resampling(&m_x_y.at(index_max), sampling_period);

}


void AxialForceDataset::linear_extr_correction(arma::fmat *tbe_mat, 
    arma::fmat *full_mat)
{
    arma::fvec x_tbe = tbe_mat->col(0);
    arma::fvec y_tbe = tbe_mat->col(1);

    arma::fvec x_full = full_mat->col(0);
    arma::fvec y_full = full_mat->col(1);

    float extra_x = x_full.back();
    float extra_y = linear_extrapolation(extra_x, x_tbe, y_tbe);
    arma::fvec push_vec = {extra_x, extra_y};
    tbe_mat->insert_rows(tbe_mat->n_rows, push_vec.t());
}


float AxialForceDataset::linear_extrapolation(float tn, arma::fvec t_vec, 
    arma::fvec f_vec)
{
    float tc = arma::as_scalar(t_vec.row(t_vec.n_rows - 1));
    float to = arma::as_scalar(t_vec.row(t_vec.n_rows - 2));

    float fc = arma::as_scalar(f_vec.row(f_vec.n_rows - 1));
    float fo = arma::as_scalar(f_vec.row(f_vec.n_rows - 2));

    return (fc + ((fc - fo) / (tc - to)) * (tn - to));
}


void AxialForceDataset::resampling(arma::fmat *matr, float ts)
{
    arma::fvec t = (*matr).col(0);
    arma::fvec tu = arma::regspace<arma::fvec>(t.front(), ts, t.back());
    arma::fmat mat_u(tu.n_rows, (*matr).n_cols); mat_u.zeros();
    mat_u(0, arma::span::all) = (*matr)(0, arma::span::all);

    for (int64_t i = 1; i < mat_u.n_rows; i++)
    {   
        arma::uvec q1 =  arma::find(t < tu(i));
        arma::uvec q2 = arma::find(t > tu(i));

        // Indices that tu(i) belongs in
        int64_t index1 = q1.back(); int64_t index2 = q2.front();

        // Times that tu(i) belongs in
        float t1 = t(index1); float t2 = t(index2);

        // Corresponding values for t1 and t2
        arma::fvec a1 = (*matr)(index1, arma::span(1, matr->n_cols - 1));
        arma::fvec a2 = (*matr)(index2, arma::span(1, matr->n_cols - 1));

        // Interpolation
        arma::fvec a = a2 - ((a2 - a1) / (t2 - t1)) * (t2 - tu(i));

        mat_u(i, 0) = tu(i);
        mat_u(i, arma::span(1, mat_u.n_cols - 1)) = a;
    }
    
    (*matr) = mat_u;
}
// void AxialForceDataset::map_str_to_variable(std::string in_str, arma::fvec x)
// {
//     // if (in_str.compare(m_meas_ans[static_cast<int>(meas_index::displ_x)]) == 0)
//     // {
//     //     m_displ_x = x;
//     // }

//     // else if (in_str.compare(m_meas_ans[static_cast<int>(meas_index::displ_x)]) == 0)
//     // {
//     //     m_displ_x = x;
//     // }

// }


AxialForceDataset::~AxialForceDataset()
{
    m_file.close();
}