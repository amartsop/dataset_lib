#ifndef PYTHON_API_H
#define PYTHON_API_H

#include <Python.h>
#include <iostream>

class PythonAPI
{
    public:
        PythonAPI() {};

        static void python_function_call(std::string script_abs_dir, 
            std::string script_name, std::string function_name, 
            std::string *args, int arg_num);
    private:
};


void PythonAPI::python_function_call(std::string script_abs_dir,
    std::string script_name, std::string function_name, std::string *args, 
    int arg_num)
{

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    Py_Initialize();
    PyObject *sysmodule = PyImport_ImportModule("sys");
    PyObject *syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyString_FromString(script_abs_dir.c_str()));
    Py_DECREF(syspath);
    Py_DECREF(sysmodule);

    pName = PyString_FromString(script_name.c_str());
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
    
        pFunc = PyObject_GetAttrString(pModule, function_name.c_str());

        if (pFunc && PyCallable_Check(pFunc)) {
             
            pArgs = PyTuple_New(arg_num);        

            for (int i = 0; i < arg_num; ++i) {
                pValue = PyString_FromString(args[i].c_str());
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                }
                // Set item
                PyTuple_SetItem(pArgs, i, pValue);
            }

            pValue = PyObject_CallObject(pFunc, pArgs);

            Py_DECREF(pArgs);
            if (pValue != NULL) {
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
            }
        }
        else 
        {
            if (PyErr_Occurred())
            {
                PyErr_Print();

            }
            fprintf(stderr, "Cannot find function \"%s\"\n", function_name.c_str());
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }


    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", script_name.c_str());
    }

    // Py_Finalize();

}

#endif