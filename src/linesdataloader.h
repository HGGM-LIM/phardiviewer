#ifndef LINESDATALOADER_H
#define LINESDATALOADER_H

#include <QUrl>

class LinesData;

class LinesDataLoader
{
public:
    LinesDataLoader();

    // Expects a local file (file://).
    // Returned object is owned by caller.
    LinesData * load(const QUrl &url, QString fileType = "") const;
};

#endif // LINESDATALOADER_H
