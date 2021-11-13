
#ifndef fmu4foam_PYTHONSTATE_HPP
#define fmu4foam_PYTHONSTATE_HPP

#include <Python.h>
#include <iostream>

namespace fmu4foam
{

class PyState
{
public:
    PyState()
    {
        was_initialized_ = Py_IsInitialized();

        if (!was_initialized_) {
            Py_SetProgramName(L"./fmu4foam");
            Py_Initialize();
            PyEval_InitThreads();
            _mainPyThread = PyEval_SaveThread();
        }
    }

    ~PyState()
    {
        if (!was_initialized_) {
            PyEval_RestoreThread(_mainPyThread);
            Py_Finalize();
        }
    }

private:
    bool was_initialized_;
    PyThreadState* _mainPyThread;
};

} // namespace fmu4foam

#endif //fmu4foam_PYTHONSTATE_HPP
