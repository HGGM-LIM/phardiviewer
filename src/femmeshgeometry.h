#ifndef FEMMESHGEOMETRY_H
#define FEMMESHGEOMETRY_H

#include <Qt3DRender/qgeometry.h>

namespace Qt3DRender {
class QAttribute;
class QBuffer;
}

class MeshData;

class FemMeshGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
public:
    FemMeshGeometry(Qt3DCore::QNode *parent = Q_NULLPTR);

    void setFromMeshData(MeshData *meshData);

private:
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_colorAttribute;
    Qt3DRender::QAttribute *m_normalsAttribute;
    Qt3DRender::QBuffer *m_vertexBuffer;

    Qt3DRender::QAttribute *m_indexAttribute;
    Qt3DRender::QBuffer *m_indexBuffer;
};

#endif // FEMMESHGEOMETRY_H
