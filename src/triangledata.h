#ifndef TRIANGLEDATA_H
#define TRIANGLEDATA_H

#include <QtGlobal>

class TriangleData
{
public:
    TriangleData(int vertexIndex0=0, int vertexIndex1=0, int vertexIndex2=0);

    void setVertexIndex0(int vi) { m_vertexIndices[0] = vi; }
    void setVertexIndex1(int vi) { m_vertexIndices[1] = vi; }
    void setVertexIndex2(int vi) { m_vertexIndices[2] = vi; }

    int vertexIndex0() const { return m_vertexIndices[0]; }
    int vertexIndex1() const { return m_vertexIndices[1]; }
    int vertexIndex2() const { return m_vertexIndices[2]; }

private:
    int m_vertexIndices[3];
};

#endif // TRIANGLEDATA_H
