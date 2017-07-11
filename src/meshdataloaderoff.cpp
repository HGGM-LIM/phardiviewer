#include "meshdataloaderoff.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector3D>

#include <QDebug>

#include "meshdata.h"
#include "vertexdata.h"
#include "triangledata.h"

#include <armadillo>

using namespace arma;

// -----------------------------------------------------------------------------
// Begin: Helper functions

static QString readSkip(QTextStream &in)
{
    while (!in.atEnd()) {
        const QString result = in.readLine().split('#').at(0).simplified();
        if (!result.isEmpty()) return result;
    }
    return "";
}


static bool readHeader(QTextStream &in)
{
    return readSkip(in) == "OFF";
}


static bool readVertexCountAndTriangleCount(QTextStream &in, int &vertexCount, int &triangleCount)
{
    const QStringList lst = readSkip(in).split(' ');
    if (lst.size()<2) return false;
    int idx = 0;
    bool ok = false;
    vertexCount = lst.at(idx++).toInt(&ok); if (!ok) return false;
    triangleCount = lst.at(idx++).toInt(&ok); if (!ok) return false;
    return true;
}

static bool readVertexData(QTextStream &in, QVector3D &position, QVector3D &color)
{
    const QStringList lst = readSkip(in).split(' ');
    if (lst.size()<3) return false; // TODO: 7
    int idx = 0;
    bool ok = false;
    position.setX( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;
    position.setY( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;
    position.setZ( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;

    color.setX( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;
    color.setY( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;
    color.setZ( lst.at(idx++).toFloat(&ok) ); if (!ok) return false;

    return true;
}


static bool readVertices(QTextStream &in, int vertexCount, MeshData &meshData)
{
    fmat vertexMat(vertexCount, 6);

    QVector3D position;
    QVector3D color;
    for (int i=0; i<vertexCount; i++) {
        if (readVertexData(in, position, color)) {
            vertexMat(i,0) = position.x();
            vertexMat(i,1) = position.y();
            vertexMat(i,2) = position.z();
            vertexMat(i,3) = color.x();
            vertexMat(i,4) = color.y();
            vertexMat(i,5) = color.z();
        }
    }

    meshData.setVertexMat(vertexMat);

    return true;
}


static bool readTriangleData(QTextStream &in, int &vertexIndex0, int &vertexIndex1, int &vertexIndex2)
{
    const QStringList lst = readSkip(in).split(' ');
    if (lst.size()<4) return false;
    int idx = 0;
    bool ok = false;
    idx++; // skip first element. it always contains "3"
    vertexIndex0 = lst.at(idx++).toFloat(&ok); if (!ok) return false;
    vertexIndex1 = lst.at(idx++).toFloat(&ok); if (!ok) return false;
    vertexIndex2 = lst.at(idx++).toFloat(&ok); if (!ok) return false;
    return true;
}


static bool readTriangles(QTextStream &in, int triangleCount, MeshData &meshData)
{
    umat indexMat(triangleCount, 3);

    int vertexIndex0 = 0;
    int vertexIndex1 = 0;
    int vertexIndex2 = 0;
    for (int i=0; i<triangleCount; i++) {
        if (readTriangleData(in, vertexIndex0, vertexIndex1, vertexIndex2)) {
            indexMat(i,0) = vertexIndex0;
            indexMat(i,1) = vertexIndex1;
            indexMat(i,2) = vertexIndex2;
        }
    }

    meshData.setTriangleMat(indexMat);
    return true;
}

// End: Helper functions
// -----------------------------------------------------------------------------


MeshData * MeshDataLoaderOff::loadMeshDataFromOff(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    QTextStream in(&file);

    MeshData * meshData = new MeshData;
    int vertexCount = 0;
    int triangleCount = 0;

    if (readHeader(in)) {
        if (readVertexCountAndTriangleCount(in, vertexCount, triangleCount)) {
            if (readVertices(in, vertexCount, *meshData)) {
                if (readTriangles(in, triangleCount, *meshData)) {
                    file.close();
                    return meshData;
                }
            }
        }
    }
    file.close();
    delete meshData;
    return 0;
}


QStringList MeshDataLoaderOff::supportedFileTypes()
{
    return QStringList() << "off";
}
