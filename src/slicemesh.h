#ifndef SLICEMESH_H
#define SLICEMESH_H

#include "image.h"

using namespace arma;

class SliceMesh
{

public:
    SliceMesh();

    static surface generateSliceMesh(std::string filename, int slice, Axis axis);

private:

};

#endif // SLICEMESH_H
