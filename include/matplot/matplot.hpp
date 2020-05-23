#ifndef MAT_PLOT_H
#define MAT_PLOT_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <Python.h>

#include "./include/pythonAPI.hpp"


template <class M>

class MatPlot
{   
    public:

        MatPlot(int fig_num=1);

        void plot2D(M vec1, M vec2, std::string fmt="b", 
            int linewidth=2, int markersize=5, std::string lab="");

        void show(void);

        void grid(void);

        void subplot(int rows, int cols, int fig);

        void set_xlabel(std::string text, bool latex=true, int fontsize=11, 
            std::string font="serif");
        
        void set_ylabel(std::string text, bool latex=true, int fontsize=11, 
            std::string font="serif");
        
        void set_title(std::string text, bool latex=true, int fontsize=11, 
            std::string font="serif");
        
        void savefig(std::string filename);

    private:
        const std::string lib_rel_path = "./include/matplot/";
        const std::string script_rel_dir = lib_rel_path + "scripts/";
        const std::string share_rel_dir = lib_rel_path + "share/";
        const std::string script_name = "python_matplot"; 

        std::string vector_to_text(M matrix, std::string id);

        int get_current_path(void);

};

template <class M>
MatPlot<M>::MatPlot(int fig_num)
{
    std::string function_name = "fig_init";
    std::string args[1] = {std::to_string(fig_num)};

    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::show(void)
{
    std::string function_name = "show";
    std::string args[1] = {""};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::savefig(std::string filename)
{
    std::string function_name = "savefig";
    std::string args[1] = {filename};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::grid(void)
{
    std::string function_name = "grid";
    std::string args[1] = {""};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::subplot(int rows, int cols, int fig)
{
    std::string function_name = "subplot";
    std::string args[3] = {std::to_string(rows), std::to_string(cols), std::to_string(fig)};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::set_xlabel(std::string text, bool latex, int fontsize, 
    std::string font)
{
    std::string latex_state = (latex) ? ("True") : ("False");
    std::string function_name = "set_xlabel";
    std::string args[4] = {text, latex_state, std::to_string(fontsize), font};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::set_ylabel(std::string text, bool latex, int fontsize, 
    std::string font)
{
    std::string latex_state = (latex) ? ("True") : ("False");
    std::string function_name = "set_ylabel";
    std::string args[4] = {text, latex_state, std::to_string(fontsize), font};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}

template <class M>
void MatPlot<M>::set_title(std::string text, bool latex, int fontsize, 
    std::string font)
{
    std::string latex_state = (latex) ? ("True") : ("False");
    std::string function_name = "set_title";
    std::string args[4] = {text, latex_state, std::to_string(fontsize), font};
    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0])); 
}


template <class M>
void MatPlot<M>::plot2D(M vec1, M vec2, std::string fmt, 
    int linewidth, int markersize, std::string lab)
{
    // Create txt file from vector
    std::string filepath_vec1 = vector_to_text(vec1, "vec1");
    std::string filepath_vec2 = vector_to_text(vec2, "vec2");   

    // Python function
    std::string function_name = "plot2D";
    std::string args[6] = {filepath_vec1, filepath_vec2, fmt, 
        std::to_string(linewidth), std::to_string(markersize), lab};

    PythonAPI::python_function_call(script_rel_dir, script_name, function_name, 
        args, sizeof(args)/sizeof(args[0]));
}


template <class M>
std::string MatPlot<M>::vector_to_text(M vec, std::string id)
{   
    struct stat st = {0};
    
    if (stat(share_rel_dir.c_str(), &st) == -1) {
        mkdir(share_rel_dir.c_str(), 0700);
    }

    std::string filename = "/" + id + ".txt";

    std::string filepath = share_rel_dir + filename;
    std::ofstream file(filepath.c_str());

    if (file.is_open()) { file << vec; }
    
    file.close();
    return filepath;
}


#endif

