#ifndef ODFGENERATOR_H
#define ODFGENERATOR_H

#include "armadillo"

#include "362_shell_semisphere.h"
#include "image.h"

#include <QDebug>
#include <QVector3D>
#include <QFileInfo>

using namespace arma;

class ODFGenerator
{
public:
    ODFGenerator(std::string filename, int xSlice = -1, int ySlice = -1, int zSlice = -1);

    void setMask(std::string filename);
    surface generateODFSurfaces(Axis axis = Axis::NONE);
    void setVertAndFacetsScheme(std::string vertFilename, std::string facetsFilename, bool duplicateScheme = false);
    void setXSlice(int xSlice);
    void setYSlice(int ySlice);
    void setZSlice(int zSlice);

    void getImageSize(int *x, int *y, int *z);

private:
    bool isSchemeDuplicated;

    int m_xSlice;
    int m_ySlice;
    int m_zSlice;

    std::string m_imageODFFile;
    std::string m_imageMaskFile;

    Image4DType::Pointer m_imageODF;
    Image3DType::Pointer m_imageMask;
    bool m_duplicateScheme;

    umat m_facetsScheme;
    fmat m_vertexScheme;

    fmat rotMatrix;
    fmat spacingMatrix;
    fmat originMatrix;

    umat locMat;

    void readMask();
    void emptyMask(int xdiff, int ydiff, int zdiff);
};

#endif // ODFGENERATOR_H
