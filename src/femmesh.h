#ifndef FEMMESH_H
#define FEMMESH_H

#include <Qt3DRender/qgeometryrenderer.h>

#include "armadillo"

class MeshData;

class FemMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    explicit FemMesh(Qt3DCore::QNode *parent = 0);
    ~FemMesh();

    void setCustomGeometry(arma::fmat vertices, arma::umat faces, arma::fmat colors = arma::fmat(), arma::fmat normals = arma::fmat());

    void getMeshCoordinateSystem(QVector3D *center, float *maxDistX, float *maxDistY, float *maxDistZ);

public slots:
    void setFromMeshData(MeshData *meshData);
    void clear();

  //  void init();

private:
    MeshData *m_meshData;
};

#endif // FEMMESH_H
