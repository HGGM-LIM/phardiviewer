#include "odfgenerator.h"

ODFGenerator::ODFGenerator(std::__cxx11::string filename, int xSlice, int ySlice, int zSlice)
{
    m_imageODF = Image4DType::New();
    ReadImage< Image4DType >(filename, m_imageODF);

    m_xSlice = xSlice;
    m_ySlice = ySlice;
    m_zSlice = zSlice;

    m_vertexScheme = fmat(0,0);
    m_facetsScheme = umat(0,0);
    m_duplicateScheme = false;
    isSchemeDuplicated = false;

    // (x,y,z)^T = D * S2 * (i,j,k)^T + O
    spacingMatrix.resize(1,3);
    originMatrix.resize(3,1);
    rotMatrix.resize(3,3);

    for (int i = 0; i < 3; i++)
    {
        originMatrix.at(i) = m_imageODF->GetOrigin()[i];
        spacingMatrix.at(i) = m_imageODF->GetSpacing()[i];

        for (int j = 0; j < 3; j++)
        {
            rotMatrix.at(i,j) = m_imageODF->GetDirection()[i][j];
        }
    }

    spacingMatrix = diagmat(spacingMatrix);
}

void ODFGenerator::setXSlice(int xSlice)
{
    m_xSlice = xSlice;
}

void ODFGenerator::setYSlice(int ySlice)
{
    m_ySlice = ySlice;
}

void ODFGenerator::setZSlice(int zSlice)
{
    m_zSlice = zSlice;
}

void ODFGenerator::setMask(std::__cxx11::string filename)
{
    QFileInfo check_file(QString(filename.c_str()));
    if (check_file.exists() && check_file.isFile())
        m_imageMaskFile = filename;
    else
        m_imageMaskFile = "";
}

void ODFGenerator::setVertAndFacetsScheme(std::__cxx11::string vertFilename, std::__cxx11::string facetsFilename, bool duplicateScheme)
{
    m_vertexScheme.load(vertFilename, arma::raw_ascii);
    m_facetsScheme.load(facetsFilename);

    m_duplicateScheme = duplicateScheme;
}

surface ODFGenerator::generateODFSurfaces(Axis axis)
{
    surface odf;

    if (m_vertexScheme.is_empty() || m_facetsScheme.is_empty())
    {
        qDebug() << "Loading 362 vertex reconstruction scheme";
        m_vertexScheme = ODFScheme::ODFDirscheme_362();
        m_facetsScheme = ODFScheme::ODFFacetsscheme_362();

        m_duplicateScheme = true;
    }

    if (m_xSlice < 0 && m_ySlice < 0 && m_zSlice < 0 &&
            m_imageMaskFile.empty())
    {
        qDebug() << "Error! No slices or image mask loaded.";
        return odf;
    }

    Image4DType::RegionType regionODF   = m_imageODF->GetBufferedRegion();

    int xdiff = regionODF.GetSize()[0];
    int ydiff = regionODF.GetSize()[1];
    int zdiff = regionODF.GetSize()[2];

    int Nd;
    if (isSchemeDuplicated)
        Nd = m_vertexScheme.n_rows / 2;
    else
        Nd = m_vertexScheme.n_rows;

    if (!m_imageMaskFile.empty())
    {
        readMask();
    }
    else
    {
        emptyMask(xdiff, ydiff, zdiff);
    }

    uvec indexLocMat;
    float offsetX = 0, offsetY = 0, offsetZ = 0;

    if (axis == Axis::X)
    {
        indexLocMat = find(locMat.row(0) == m_xSlice);
        offsetX = 0.5f;
    }

    if (axis == Axis::Y)
    {
        indexLocMat = find(locMat.row(1) == m_ySlice);
        offsetY = 0.5f;
    }

    if (axis == Axis::Z)
    {
        indexLocMat = find(locMat.row(2) == m_zSlice);
        offsetZ = 0.5f;
    }

    if (axis == Axis::NONE)
    {
        indexLocMat = linspace<uvec>(0,locMat.n_cols-1,locMat.n_cols);
    }


    if (indexLocMat.n_elem == 0)
    {
        return odf;
    }

    fmat VdiffMat = fmat(Nd, indexLocMat.n_elem);

#pragma omp parallel for
    for (int w = 0; w < regionODF.GetSize()[3]; ++w) {

        Index4DType coord_4d;
        coord_4d[3] = w;

        for (int i = 0; i < indexLocMat.n_elem; ++i) {
            coord_4d[0] = locMat(0,indexLocMat(i));
            coord_4d[1] = locMat(1,indexLocMat(i));
            coord_4d[2] = locMat(2,indexLocMat(i));
            VdiffMat(w,i) = m_imageODF->GetPixel(coord_4d);
        }
    }

    frowvec pixelMask = sum(VdiffMat,0);
    frowvec odfMax = arma::max(VdiffMat,0);
    uvec inda_vec = find(pixelMask != 0);
    Mat<uword> inda = conv_to<Mat<uword>>::from(inda_vec);

    if (m_duplicateScheme)
    {
        VdiffMat = repmat(VdiffMat,2,1);
        if (!isSchemeDuplicated)
        {
            m_vertexScheme = join_vert(m_vertexScheme, -1*m_vertexScheme);
            isSchemeDuplicated = true;
        }
    }

    odf.vertices.resize(m_vertexScheme.n_rows*inda.n_elem,3);
    odf.colors.resize(m_vertexScheme.n_rows*inda.n_elem,3);
    odf.faces.resize(m_facetsScheme.n_rows*inda.n_elem,3);
    odf.normals = repmat(m_vertexScheme,inda.n_elem,1);

    int vertexFilColVal = 0;
    int facetsFilColVal = 0;

    for (uword j = 0; j < inda.n_elem; ++j)
    {
        frowvec offset(3);
        offset << locMat(0,indexLocMat(inda(j))) + offsetX << locMat(1,indexLocMat(inda(j))) + offsetY << locMat(2,indexLocMat(inda(j))) + offsetZ;

        vertexFilColVal = j*m_vertexScheme.n_rows;
        facetsFilColVal = j*m_facetsScheme.n_rows;

        odf.vertices(span(vertexFilColVal, vertexFilColVal + m_vertexScheme.n_rows-1),span::all) = m_vertexScheme % repmat(0.4f*(VdiffMat.col(inda(j))/odfMax(inda(j))),1,3) + repmat(offset ,m_vertexScheme.n_rows,1);
        odf.colors(span(vertexFilColVal, vertexFilColVal + m_vertexScheme.n_rows-1),span::all) = abs(m_vertexScheme)*0.94f;

        odf.faces(span(facetsFilColVal, facetsFilColVal + m_facetsScheme.n_rows-1),span::all) = m_facetsScheme + vertexFilColVal;
    }

    odf.vertices = rotMatrix * spacingMatrix * odf.vertices.t() + repmat(originMatrix,1,odf.vertices.n_rows);
    odf.vertices = odf.vertices.t();

    locMat.clear();
    VdiffMat.clear();

    return odf;
}

void ODFGenerator::readMask()
{

    m_imageMask = Image3DType::New();
    ReadImage< Image3DType >(m_imageMaskFile, m_imageMask);

    Image3DType::RegionType regionMask = m_imageMask->GetBufferedRegion();

    std::vector<unsigned> locVec;

//#pragma omp parallel for
    for (int x = 0; x < regionMask.GetSize()[0]; ++x) {
        Index3DType coord_3d;
        coord_3d[0]=x;
        for (int y = 0; y < regionMask.GetSize()[1]; ++y) {
            coord_3d[1]=y;
            for (int z = 0; z < regionMask.GetSize()[2]; ++z) {
                coord_3d[2]=z;

                if (m_imageMask->GetPixel(coord_3d) != 0 &&
                        ((m_xSlice > -1 && m_xSlice == x) ||
                         (m_ySlice > -1 && m_ySlice == y) ||
                         (m_zSlice > -1 && m_zSlice == z)))
                {
                    locVec.push_back(x);
                    locVec.push_back(y);
                    locVec.push_back(z);
                }
                else if (m_imageMask->GetPixel(coord_3d) != 0 &&
                         ((m_xSlice == -1) &&
                          (m_ySlice == -1) &&
                          (m_zSlice == -1)))
                {
                    locVec.push_back(x);
                    locVec.push_back(y);
                    locVec.push_back(z);
                }
            }
        }
    }

    locMat = conv_to<umat>::from(locVec);
    locMat.reshape(3,locVec.size()/3);
}


// TODO fix this, we need to recalculate the mask if x,y or z changes
void ODFGenerator::emptyMask(int xdiff, int ydiff, int zdiff)
{

    locMat.clear();

    if (m_xSlice > -1)
    {
        umat xindex, yindex, zindex;

        yindex = linspace<urowvec>(0,ydiff-1,ydiff);
        yindex = repmat(yindex,zdiff,1);
        yindex = vectorise(yindex,0);

        xindex = ones<umat>(yindex.n_rows,1) * m_xSlice;

        zindex = linspace<umat>(0,zdiff-1,zdiff);
        zindex = vectorise(zindex);
        zindex = repmat(zindex,ydiff,1);

        umat aux(3,ydiff*zdiff);
        aux.row(0) = xindex.t();
        aux.row(1) = yindex.t();
        aux.row(2) = zindex.t();
        locMat = join_horiz(locMat, aux);
    }

    if (m_ySlice > -1)
    {
        umat xindex, yindex, zindex;

        xindex = linspace<urowvec>(0,xdiff-1,xdiff);
        xindex = repmat(xindex,zdiff,1);
        xindex = vectorise(xindex,0);

        yindex = ones<umat>(xindex.n_rows,1) * m_ySlice;

        zindex = linspace<umat>(0,zdiff-1,zdiff);
        zindex = vectorise(zindex);
        zindex = repmat(zindex,xdiff,1);

        umat aux(3,xdiff*zdiff);
        aux.row(0) = xindex.t();
        aux.row(1) = yindex.t();
        aux.row(2) = zindex.t();
        locMat = join_horiz(locMat, aux);
    }

    if (m_zSlice > -1)
    {
        umat xindex, yindex, zindex;

        xindex = linspace<urowvec>(0,xdiff-1,xdiff);
        xindex = repmat(xindex,ydiff,1);
        xindex = vectorise(xindex,0);

        yindex = linspace<umat>(0,ydiff-1,ydiff);
        yindex = vectorise(yindex);
        yindex = repmat(yindex,xdiff,1);

        zindex = ones<umat>(yindex.n_rows,1) * m_zSlice;

        umat aux(3,xdiff*ydiff);
        aux.row(0) = xindex.t();
        aux.row(1) = yindex.t();
        aux.row(2) = zindex.t();
        locMat = join_horiz(locMat, aux);
    }
}


void ODFGenerator::getImageSize(int *x, int *y, int *z)
{
    Image4DType::RegionType regionODF = m_imageODF->GetBufferedRegion();

    (*x) = regionODF.GetSize()[0];
    (*y) = regionODF.GetSize()[1];
    (*z) = regionODF.GetSize()[2];
}
