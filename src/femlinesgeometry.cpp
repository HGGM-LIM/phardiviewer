#include "femlinesgeometry.h"

#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>

#include <QByteArray>
#include <QVector3D>

#include "linesdata.h"

#include <armadillo>

using namespace arma;

FemLinesGeometry::FemLinesGeometry(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent)
    , m_positionAttribute(new Qt3DRender::QAttribute(this))
    , m_colorAttribute(new Qt3DRender::QAttribute(this))
    , m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))

    , m_indexAttribute(new Qt3DRender::QAttribute(this))
    , m_indexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this))
{
}

FemLinesGeometry::~FemLinesGeometry()
{
    m_positionAttribute->deleteLater();
    m_colorAttribute->deleteLater();
    m_vertexBuffer->deleteLater();
    m_indexAttribute->deleteLater();
    m_indexBuffer->deleteLater();
}

void FemLinesGeometry::setFromLinesData(LinesData *linesData)
{
    // Geometry with up to 2^16 vertices. Limitation due to IndexBuffer, see below.

    const int vertexCount = linesData->vertexCount();

    const fmat vertexMat = linesData->vertexMatData();

    fmat tVertexMat = vertexMat.t();
    tVertexMat.reshape(vertexCount*6,1);
    std::vector<float> v = conv_to< std::vector<float> >::from(tVertexMat);

    QByteArray vertexBufferData(reinterpret_cast<const char*>(v.data()), v.size() * sizeof(float));
    m_vertexBuffer->setData(vertexBufferData);

    // Configure our attributes

    m_positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
    m_positionAttribute->setDataSize(3); // 3 fields (vec3 of float)
    m_positionAttribute->setCount(vertexCount);
    m_positionAttribute->setByteOffset(0 * sizeof(float)); // is first attribute in m_vertexBuffer
    m_positionAttribute->setByteStride(6 * sizeof(float)); // 3+3+1, see above
    m_positionAttribute->setBuffer(m_vertexBuffer);
    addAttribute(m_positionAttribute);

    m_colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());
    m_colorAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
    m_colorAttribute->setDataSize(3); // 3 fields (vec3 of float)
    m_colorAttribute->setCount(vertexCount);
    m_colorAttribute->setByteOffset(3 * sizeof(float)); // Begins after 3 position floats
    m_colorAttribute->setByteStride(6 * sizeof(float)); // 3+3+1, see above
    m_colorAttribute->setBuffer(m_vertexBuffer);
    addAttribute(m_colorAttribute);

    // ----- IndexBuffer -----
    // Allows up to 2^16 triangles, ushort.
    // If that's not enough one can increase this by using uint.

    const int stretchCount = linesData->stretchCount();

    const umat stretchMat = linesData->stretchMatData();

    umat tStretchMat = stretchMat.t();
    tStretchMat.reshape(stretchCount*2,1);
    std::vector<unsigned> t = conv_to< std::vector<unsigned> >::from(tStretchMat);

    QByteArray indexBufferData(reinterpret_cast<const char*>(t.data()), t.size() * sizeof(unsigned));
    m_indexBuffer->setData(indexBufferData);

    // The IndexAttribute is a special feature of openGL. It's not like the other attributes.
    // There can only be one IndexAttribute. It has special values and no name.
    m_indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    m_indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
    m_indexAttribute->setDataSize(1); // always 1
    m_indexAttribute->setCount(stretchCount); // sic!
    m_indexAttribute->setByteOffset(0); // always 0
    m_indexAttribute->setByteStride(sizeof(unsigned)); // always 0
    m_indexAttribute->setBuffer(m_indexBuffer);
    addAttribute(m_indexAttribute);

    tVertexMat.clear();
    v.clear();
    vertexBufferData.clear();
    tStretchMat.clear();
    t.clear();
    indexBufferData.clear();
}
