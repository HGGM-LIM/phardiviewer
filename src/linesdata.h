#ifndef LINESDATA_H
#define LINESDATA_H

#include <armadillo>

using namespace arma;

class LinesData
{
public:
    LinesData();
    ~LinesData();

    fmat vertexMatData() const;
    void setVertexMat(fmat vertexMat);

    umat stretchMatData() const;
    void setStretchMat(umat stretchMat);

    fmat colorMatData() const;
    void setColorMat(fmat colorMat);

    int vertexCount() const { return m_vertexMat.n_rows; }
    int stretchCount() const { return m_stretchMat.n_rows; }

    void dbg() const; // print mesh data to debug output

private:
    fmat m_vertexMat;
    umat m_stretchMat;
    fmat m_colorMat;
};

#endif // MESHDATA_H
