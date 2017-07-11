#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <QVector3D>


class VertexData
{
public:
    VertexData(const QVector3D &p=QVector3D(), const QVector3D &f=QVector3D(), float t=0);

    void setPosition(const QVector3D &p) { m_position = p; }
    void setColours(const QVector3D &f) { m_colours = f; }
    void setTemperature(float t) { m_temperature = t; }

    QVector3D position() const { return m_position; }
    QVector3D colours() const { return m_colours; }
    float temperature() const { return m_temperature; }

private:
    QVector3D m_position;
    QVector3D m_colours;
    float m_temperature;
};

#endif // VERTEXDATA_H
