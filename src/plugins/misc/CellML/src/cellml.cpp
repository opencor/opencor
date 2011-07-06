#include "cellml.h"
#include "common.h"

#include <QApplication>
#include <QDate>
#include <QFile>

namespace OpenCOR {
namespace CellML {

QString cellmlVersionString(const CellmlVersion &pCellmlVersion)
{
    switch (pCellmlVersion)
    {
        case Cellml_1_1:
            return "1.1";
        default:
            return "1.0";
    }
}

bool newCellmlFile(const QString &pFileName, const QString &pModelName,
                   const CellmlVersion &pCellmlVersion)
{
    QFile cellmlFile(pFileName);

    if (cellmlFile.open(QIODevice::WriteOnly)) {
        QFile cellmlTemplate(":cellmlTemplate");

        cellmlTemplate.open(QIODevice::ReadOnly);

        QString cellmlVerStr = cellmlVersionString(pCellmlVersion);

        cellmlFile.write(QString(cellmlTemplate.readAll().data()).arg(cellmlVersionString(pCellmlVersion),
                                                                      QDate::currentDate().toString(),
                                                                      QTime::currentTime().toString(),
                                                                      qApp->applicationName(),
                                                                      OpencorHomepageUrl,
                                                                      pModelName,
                                                                      pModelName,
                                                                      cellmlVerStr,
                                                                      cellmlVerStr).toLatin1());

        cellmlFile.close();
        cellmlTemplate.close();

        // Everything went fine, so...

        return true;
    } else {
        // The CellML file couldn't be created, so...

        return false;
    }
}

} }
