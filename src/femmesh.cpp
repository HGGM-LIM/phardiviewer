#include "femmesh.h"

#include "meshdata.h"
#include "femmeshgeometry.h"

#include <Qt3DRender/qattribute.h>

#include "meshdataloader.h"


FemMesh::FemMesh(QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{

}


FemMesh::~FemMesh()
{
    clear();
    m_meshData;
    QNode::deleteLater();
}

void FemMesh::setFromMeshData(MeshData *meshData)
{
    clear();
    FemMeshGeometry *geom = new FemMeshGeometry(this);
    geom->setFromMeshData(meshData);
    setInstanceCount(1);
    setFirstVertex(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    // setPrimitiveCount has a misleading name.
    // It takes this: (number of triangles) * (number of vertices per triangle)
    setVertexCount(meshData->triangleCount()*3); // it has to be nIndex*3
    setGeometry(geom);
}

void FemMesh::setCustomGeometry(fmat vertices, umat faces, fmat colors, fmat normals)
{
    m_meshData = new MeshData;
    m_meshData->setVertexMat(vertices);
    m_meshData->setTriangleMat(faces);
    m_meshData->setColorMat(colors);

    if (normals.is_empty())
        m_meshData->generateAveragedNormals();
    else
        m_meshData->setVertexNormalMatData(normals);

    setFromMeshData(m_meshData);
}

void FemMesh::getMeshCoordinateSystem(QVector3D *center, float *maxDistX, float *maxDistY, float *maxDistZ)
{
    fmat meanValues(1,3);
    meanValues = mean(m_meshData->vertexMatData(),0);
    center->setX(meanValues.at(0));
    center->setY(meanValues.at(1));
    center->setZ(meanValues.at(2));

    (*maxDistX) = max(abs(m_meshData->vertexMatData().col(0) - meanValues.at(0)));
    (*maxDistY) = max(abs(m_meshData->vertexMatData().col(1) - meanValues.at(1)));
    (*maxDistZ) = max(abs(m_meshData->vertexMatData().col(2) - meanValues.at(2)));
}

void FemMesh::clear()
{
    if (geometry())
        geometry()->deleteLater();
    setGeometry(Q_NULLPTR);
}
