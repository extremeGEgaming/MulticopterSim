/*
Header-only C++ class for wrapping Nengo PID controller written in Python

Copyright 2018 Simon D. Levy

MIT License
*/

#include <Python.h>
#include "PythonClass.hpp"

class NengoPidController : public PythonClass {

    public:

        NengoPidController(float Kp, float Kd, float Ki=0, int n_dims=1, float sim_time=0.001,
                int n_neurons=100, float integral_synapse=0.1, float integral_radius=1.0)
            : PythonClass("nengo_pidcontrol", "PIDController") 
        {
            // Setup args for constructor
            PyObject * pArgs = PyTuple_New(8);
            PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(Kp));
            PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(Kd));
            PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(Kd));
            PyTuple_SetItem(pArgs, 3, PyLong_FromLong(n_dims));
            PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(sim_time));
            PyTuple_SetItem(pArgs, 5, PyLong_FromLong(n_neurons));
            PyTuple_SetItem(pArgs, 6, PyFloat_FromDouble(integral_synapse));
            PyTuple_SetItem(pArgs, 7, PyFloat_FromDouble(integral_radius));

            // Create class instance with args
            _pInstance = PyObject_CallObject(_pClass, pArgs); 

            // Create tuples to hold target, actual
            _pTarget =  PyTuple_New(n_dims);
            _pActual =  PyTuple_New(n_dims);

            // Store input params for getCorrection()
            _n_dims = n_dims;
        }

        void getCorrection(float target[], float actual[], float correction[])
        {
            // Build tuples for Python method args
            for (int k=0; k<_n_dims; ++k) {
                PyTuple_SetItem(_pTarget, k, PyFloat_FromDouble(target[k]));    
                PyTuple_SetItem(_pActual, k, PyFloat_FromDouble(actual[k]));    
            }

            // Call the Python method with the arg tuples, getting the resultant correction
            PyObject * pCorrection = PyObject_CallMethod(_pInstance, "getCorrection", "(OO)", _pTarget, _pActual);

            // Copy the resultant correction tuple back out to the result array
            for (int k=0; k<_n_dims; ++k) {
                correction[k] = PyFloat_AsDouble(PyTuple_GetItem(pCorrection, k));
            }
        }

    private:

        PyObject * _pInstance;
        PyObject * _pTarget;
        PyObject * _pActual;

        int _n_dims;
};
