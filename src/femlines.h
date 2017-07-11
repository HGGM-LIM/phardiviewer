#ifndef FEMLINES_H
#define FEMLINES_H

#include <Qt3DRender/qgeometryrenderer.h>
#include "armadillo"

class LinesData;

class FemLines : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    explicit FemLines(Qt3DCore::QNode *parent = 0);
    ~FemLines();

    void setCustomGeometry(arma::fmat vertices, arma::umat stretches, arma::fmat colors);
    void loadGeometryFile(QString filename);

public slots:
    void setFromLinesData(LinesData *linesData);
    void clear();

private:
    LinesData *m_linesData;
};

#endif // FEMLINES_H
