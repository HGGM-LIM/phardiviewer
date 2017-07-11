#include "meshdataloader.h"

#include <QDebug>

#include <QStringList>

#include "meshdata.h"
#include "meshdataloaderoff.h"


// -----------------------------------------------------------------------------
// Begin: Helper functions

static QString guessFileType(const QUrl &url, QString fileType, const QString &defaultFileType)
{
    fileType = fileType.simplified().toLower();
    if (fileType.isEmpty()) {
        QStringList lst = url.fileName().split('.');
        if (lst.size()>1) {
            fileType = lst.last().simplified().toLower();
        } else {
            fileType = defaultFileType;
        }
    }
    return fileType;
}


static QString toLocalFileName(const QUrl &url)
{
    QString fileName = url.toString();
    fileName.replace("file://", "");
    fileName.replace("qrc://", ":");
    fileName.replace("qrc:/", ":"); // Some people use that. Not sure if it's standard conform but it works.
    return fileName;
}

// End: Helper functions
// -----------------------------------------------------------------------------


MeshDataLoader::MeshDataLoader()
{
}


MeshData * MeshDataLoader::load(const QUrl &url, QString fileType) const
{
    // TODO: can only handle local files and files from Qt ressources
    const QString fileName = toLocalFileName(url);

    fileType = guessFileType(url, fileType, "off");

    if (MeshDataLoaderOff::supportedFileTypes().contains(fileType)) {
        return MeshDataLoaderOff::loadMeshDataFromOff(fileName);
    }

    return 0;
}
