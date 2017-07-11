#include "linesdata.h"

#include <QDebug>


LinesData::LinesData()
{
}


LinesData::~LinesData()
{
    m_vertexMat.clear();
    m_stretchMat.clear();
}

void LinesData::setVertexMat(fmat vertexMat)
{
    this->m_vertexMat = vertexMat;
}

fmat LinesData::vertexMatData() const
{
    return m_vertexMat;
}

void LinesData::setStretchMat(umat stretchMat)
{
    this->m_stretchMat = stretchMat;
}

umat LinesData::stretchMatData() const
{
    return m_stretchMat;
}

void LinesData::setColorMat(fmat colorMat)
{
    this->m_colorMat = colorMat;
}

fmat LinesData::colorMatData() const
{
    return m_colorMat;
}

void LinesData::dbg() const
{
    qDebug() << QString("%1 %2").arg(vertexCount()).arg(stretchCount());
    m_vertexMat.print();
    m_stretchMat.print();
}
