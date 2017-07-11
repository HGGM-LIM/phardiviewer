#ifndef FEMLINESGEOMETRY_H
#define FEMLINESGEOMETRY_H

#include <Qt3DRender/qgeometry.h>

namespace Qt3DRender {
class QAttribute;
class QBuffer;
}

class LinesData;

class FemLinesGeometry : public Qt3DRender::QGeometry
{
    Q_OBJECT
public:
    FemLinesGeometry(Qt3DCore::QNode *parent = Q_NULLPTR);
    ~FemLinesGeometry();

    void setFromLinesData(LinesData *linesData);

private:
    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_colorAttribute;
    Qt3DRender::QBuffer *m_vertexBuffer;

    Qt3DRender::QAttribute *m_indexAttribute;
    Qt3DRender::QBuffer *m_indexBuffer;
};

#endif // FEMLINESGEOMETRY_H
