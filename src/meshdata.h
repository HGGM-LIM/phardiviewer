#ifndef MESHDATA_H
#define MESHDATA_H

#include <armadillo>

#include "vertexdata.h"
#include "triangledata.h"

class VertexData;
class TriangleData;

class QVector3D;

using namespace arma;

class MeshData
{
public:
    MeshData();
    ~MeshData();

    fmat vertexMatData() const;
    void setVertexMat(fmat vertexMat);

    fmat colorMatData() const;
    void setColorMat(fmat colorMat);

    umat triangleMatData() const;
    void setTriangleMat(umat triangleMat);

    fmat vertexNormalMatData() const;
    void setVertexNormalMatData(fmat vertexNormalMat);

    int vertexCount() const { return vertexMat.n_rows; }
    int triangleCount() const { return triangleMat.n_rows; }

    void dbg() const; // print mesh data to debug output

    void generateAveragedNormals();

private:
    fmat vertexMat;
    fmat colorMat;
    umat triangleMat;
    fmat normalsMat;
};

#endif // MESHDATA_H
