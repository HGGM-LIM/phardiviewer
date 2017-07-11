#include "meshdata.h"

#include <QDebug>


MeshData::MeshData()
{
}


MeshData::~MeshData()
{
    vertexMat.clear();
    triangleMat.clear();
    colorMat.clear();
    normalsMat.clear();
}

void MeshData::setVertexMat(fmat vertexMat)
{
    this->vertexMat = vertexMat;
}

fmat MeshData::vertexMatData() const
{
    return vertexMat;
}

void MeshData::setColorMat(fmat colorMat)
{
    this->colorMat = colorMat;
}

fmat MeshData::colorMatData() const
{
    return colorMat;
}

void MeshData::setTriangleMat(umat triangleMat)
{
    this->triangleMat = triangleMat;
}

umat MeshData::triangleMatData() const
{
    return triangleMat;
}

fmat MeshData::vertexNormalMatData() const
{
    return normalsMat;
}

void MeshData::setVertexNormalMatData(fmat vertexNormalMat)
{
    this->normalsMat = vertexNormalMat;
}

void MeshData::generateAveragedNormals()
{
    normalsMat.resize(vertexMat.n_rows, 3);

    for (int i = 0; i < triangleMat.n_rows; ++i) {
        frowvec p1 = vertexMat.row( triangleMat(i,0) );
        frowvec p2 = vertexMat.row( triangleMat(i,1) );
        frowvec p3 = vertexMat.row( triangleMat(i,2) );

        frowvec a = p2 - p1;
        frowvec b = p3 - p1;
        frowvec n = normalise(cross(a, b));

        normalsMat.row( triangleMat(i,0) ) += n;
        normalsMat.row( triangleMat(i,1) ) += n;
        normalsMat.row( triangleMat(i,2) ) += n;

        normalsMat.row(triangleMat(i,0)) = normalise(normalsMat.row(triangleMat(i,0)));
        normalsMat.row(triangleMat(i,1)) = normalise(normalsMat.row(triangleMat(i,1)));
        normalsMat.row(triangleMat(i,2)) = normalise(normalsMat.row(triangleMat(i,2)));

        //normals.row(triangleMat[i]).print();
    }

    normalsMat = -1*normalsMat;
}

void MeshData::dbg() const
{
    qDebug() << QString("%1 %2").arg(vertexCount()).arg(triangleCount());
    vertexMat.print();
    triangleMat.print();
}
