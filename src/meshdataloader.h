#ifndef MESHDATALOADER_H
#define MESHDATALOADER_H

#include <QUrl>

class MeshData;

class MeshDataLoader
{
public:
    MeshDataLoader();

    // Expects a local file (file://).
    // Returned object is owned by caller.
    MeshData * load(const QUrl &url, QString fileType = "") const;
};

#endif // MESHDATALOADER_H
