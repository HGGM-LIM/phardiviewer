#include "vertexdata.h"

VertexData::VertexData(const QVector3D &p, const QVector3D &f, float t)
    : m_position(p)
    , m_colours(f)
    , m_temperature(t)
{
}
