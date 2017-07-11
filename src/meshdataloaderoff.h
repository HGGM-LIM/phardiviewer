#ifndef MESHDATALOADEROFF_H
#define MESHDATALOADEROFF_H

#include <QString>
#include <QStringList>

class MeshData;

namespace MeshDataLoaderOff {

MeshData * loadMeshDataFromOff(const QString &fileName);
QStringList supportedFileTypes();

}

#endif // MESHDATALOADEROFF_H
