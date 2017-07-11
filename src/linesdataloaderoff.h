#ifndef LINESDATALOADEROFF_H
#define LINESDATALOADEROFF_H

#include <QString>
#include <QStringList>

class LinesData;

namespace LinesDataLoaderOff {

LinesData * loadLinesDataFromOff(const QString &fileName);
QStringList supportedFileTypes();

}

#endif // LINESDATALOADEROFF_H
