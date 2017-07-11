#include "femlines.h"

#include "linesdata.h"
#include "femlinesgeometry.h"

#include "linesdataloader.h"

#include <QElapsedTimer>

FemLines::FemLines(QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{

}

FemLines::~FemLines()
{
    //QNode::cleanup();
}

void FemLines::setFromLinesData(LinesData *linesData)
{
    clear();
    FemLinesGeometry *geom = new FemLinesGeometry(this);
    geom->setFromLinesData(linesData);
    setInstanceCount(1);
    setFirstVertex(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    // setPrimitiveCount has a misleading name.
    // It takes this: (number of triangles) * (number of vertices per triangle)
    setVertexCount(linesData->stretchCount()*2); // it has to be nIndex*2
    setGeometry(geom);
}

void FemLines::loadGeometryFile(QString filename)
{
    LinesDataLoader ldl;
    LinesData *linesData = ldl.load(QUrl::fromLocalFile(filename));
    setFromLinesData(linesData);
    //  meshData->dbg(); // print mesh data to debug output
    delete linesData;
}

void FemLines::setCustomGeometry(fmat vertices, umat stretches, fmat colors)
{
    m_linesData = new LinesData;
    m_linesData->setVertexMat(vertices);
    m_linesData->setStretchMat(stretches);
    m_linesData->setColorMat(colors);

    setFromLinesData(m_linesData);
    delete m_linesData;
}

void FemLines::clear()
{
    if (geometry())
        geometry()->deleteLater();
    setGeometry(Q_NULLPTR);
}
