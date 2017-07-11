#include "femmeshgeometry.h"

#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>

#include <QByteArray>
#include <QVector3D>

#include "meshdata.h"
#include "vertexdata.h"
#include "triangledata.h"

#include <armadillo>

using namespace arma;

FemMeshGeometry::FemMeshGeometry(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometry(parent)
    , m_positionAttribute(new Qt3DRender::QAttribute(this))
    , m_colorAttribute(new Qt3DRender::QAttribute(this))
    , m_normalsAttribute(new Qt3DRender::QAttribute(this))
    , m_vertexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this))
    , m_indexAttribute(new Qt3DRender::QAttribute(this))
    , m_indexBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this))
{
}


void FemMeshGeometry::setFromMeshData(MeshData *meshData)
{
    // Geometry with up to 2^16 vertices. Limitation due to IndexBuffer, see below.

    const int vertexCount = meshData->vertexCount();

    const fmat vertexMat = meshData->vertexMatData();
    const fmat colorMat = meshData->colorMatData();
    const fmat normalsMat = meshData->vertexNormalMatData();

    fmat vertexAndColor = join_horiz(vertexMat,colorMat);
    vertexAndColor = join_horiz(vertexAndColor,normalsMat);

    fmat tVertexMat = vertexAndColor.t();
    tVertexMat.reshape(vertexCount*9,1);
    std::vector<float> v = conv_to< std::vector<float> >::from(tVertexMat);

    QByteArray vertexBufferData(reinterpret_cast<const char*>(v.data()), v.size() * sizeof(float));
    m_vertexBuffer->setData(vertexBufferData);

    // Configure our attributes

    // Qt3DRender::QAttribute::defaultPositionAttributeName() return "vertexPosition"
    m_positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
    m_positionAttribute->setDataSize(3); // 3 fields (vec3 of float)
    m_positionAttribute->setCount(vertexCount);
    m_positionAttribute->setByteOffset(0 * sizeof(float)); // is first attribute in m_vertexBuffer
    m_positionAttribute->setByteStride(9 * sizeof(float)); // 3+3+1, see above
    m_positionAttribute->setBuffer(m_vertexBuffer);
    addAttribute(m_positionAttribute);

    m_colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());
    m_colorAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
    m_colorAttribute->setDataSize(3); // 3 fields (vec3 of float)
    m_colorAttribute->setCount(vertexCount);
    m_colorAttribute->setByteOffset(3 * sizeof(float)); // Begins after 3 position floats
    m_colorAttribute->setByteStride(9 * sizeof(float)); // 3+3+1, see above
    m_colorAttribute->setBuffer(m_vertexBuffer);
    addAttribute(m_colorAttribute);

    m_normalsAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_normalsAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());
    m_normalsAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
    m_normalsAttribute->setDataSize(3); // 3 fields (vec3 of float)
    m_normalsAttribute->setCount(vertexCount);
    m_normalsAttribute->setByteOffset(6 * sizeof(float)); // Begins after 3 position floats
    m_normalsAttribute->setByteStride(9 * sizeof(float)); // 3+3+1, see above
    m_normalsAttribute->setBuffer(m_vertexBuffer);
    addAttribute(m_normalsAttribute);

//    m_temperatureAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
//    m_temperatureAttribute->setName("vertexTemperature");
//    m_temperatureAttribute->setDataType(Qt3DRender::QAttribute::Float); // type of field
//    m_temperatureAttribute->setDataSize(1); // Only 1 field (float)
//    m_temperatureAttribute->setCount(vertexCount);
//    m_temperatureAttribute->setByteOffset(6 * sizeof(float)); // Begins after 3 position floats and 3 force floats
//    m_temperatureAttribute->setByteStride(7 * sizeof(float)); // 3+3+1, see above
//    m_temperatureAttribute->setBuffer(m_vertexBuffer);
//    addAttribute(m_temperatureAttribute);

    // ----- IndexBuffer -----
    // Allows up to 2^16 triangles, ushort.
    // If that's not enough one can increase this by using uint.

    const int triangleCount = meshData->triangleCount();

    const umat triangleMat = meshData->triangleMatData();

    umat tTriangleMat = triangleMat.t();
    tTriangleMat.reshape(triangleCount*3,1);
    std::vector<unsigned> t = conv_to< std::vector<unsigned> >::from(tTriangleMat);

    QByteArray indexBufferData(reinterpret_cast<const char*>(t.data()), t.size() * sizeof(unsigned));
    m_indexBuffer->setData(indexBufferData);

    // The IndexAttribute is a special feature of openGL. It's not like the other attributes.
    // There can only be one IndexAttribute. It has special values and no name.
    m_indexAttribute->setName("indices");
    m_indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    m_indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
    m_indexAttribute->setDataSize(1); // always 1
    m_indexAttribute->setCount(triangleCount); // sic!
    m_indexAttribute->setByteOffset(0); // always 0
    m_indexAttribute->setByteStride(sizeof(unsigned)); // always 0
    m_indexAttribute->setBuffer(m_indexBuffer);
    addAttribute(m_indexAttribute);

    vertexAndColor.clear();
    tVertexMat.clear();
    v.clear();
    tTriangleMat.clear();
    t.clear();
}
