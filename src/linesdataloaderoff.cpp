#include "linesdataloaderoff.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector3D>

#include <QDebug>

#include "linesdata.h"

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


static bool readVertexCountAndTriangleCount(QTextStream &in, int &vertexCount, int &stretchCount)
{
    const QStringList lst = readSkip(in).split(' ');
    if (lst.size()<2) return false;
    int idx = 0;
    bool ok = false;
    vertexCount = lst.at(idx++).toInt(&ok); if (!ok) return false;
    stretchCount = lst.at(idx++).toInt(&ok); if (!ok) return false;
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


static bool readVertices(QTextStream &in, int vertexCount, LinesData &linesData)
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

    linesData.setVertexMat(vertexMat);

    return true;
}


static bool readTriangleData(QTextStream &in, int &vertexIndex0, int &vertexIndex1)
{
    const QStringList lst = readSkip(in).split(' ');
    if (lst.size()<3) return false;
    int idx = 0;
    bool ok = false;
    idx++; // skip first element. it always contains "3"
    vertexIndex0 = lst.at(idx++).toFloat(&ok); if (!ok) return false;
    vertexIndex1 = lst.at(idx++).toFloat(&ok); if (!ok) return false;
    return true;
}


static bool readTriangles(QTextStream &in, int stretchCount, LinesData &linesData)
{
    umat indexMat(stretchCount, 2);

    int vertexIndex0 = 0;
    int vertexIndex1 = 0;
    for (int i=0; i<stretchCount; i++) {
        if (readTriangleData(in, vertexIndex0, vertexIndex1)) {
            indexMat(i,0) = vertexIndex0;
            indexMat(i,1) = vertexIndex1;
        }
    }

    linesData.setStretchMat(indexMat);
    return true;
}

// End: Helper functions
// -----------------------------------------------------------------------------


LinesData *LinesDataLoaderOff::loadLinesDataFromOff(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    QTextStream in(&file);

    LinesData * linesData = new LinesData;
    int vertexCount = 0;
    int stretchCount = 0;

    if (readHeader(in)) {
        if (readVertexCountAndTriangleCount(in, vertexCount, stretchCount)) {
            if (readVertices(in, vertexCount, *linesData)) {
                if (readTriangles(in, stretchCount, *linesData)) {
                    file.close();
                    return linesData;
                }
            }
        }
    }
    file.close();
    delete linesData;
    return 0;
}


QStringList LinesDataLoaderOff::supportedFileTypes()
{
    return QStringList() << "off";
}
