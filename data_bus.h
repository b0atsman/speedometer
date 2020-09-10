#pragma once

// Data bus library for IPC exchange of value of type 'double'

namespace data_bus {

    // Initialization routine
    int init();

    // Deinitialization routine
    void deInit();

    // Writing value of type 'double'
    void populate(double data);

    // Reading value of type 'double'
    double consume();
}
