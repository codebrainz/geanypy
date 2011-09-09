#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "geanyplugin.h"

typedef struct _SignalManager SignalManager;

struct _SignalManager
{
    GeanyPlugin *geany_plugin;
    PyObject *py_manager;
    PyObject *py_emit_func;
};


SignalManager *signal_manager_new(GeanyPlugin *geany_plugin);
void signal_manager_free(SignalManager *signal_manager);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SIGNALMANAGER_H */
