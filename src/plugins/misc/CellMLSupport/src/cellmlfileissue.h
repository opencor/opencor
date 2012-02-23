//==============================================================================
// CellML file issue
//==============================================================================

#ifndef CELLMLFILEISSUE_H
#define CELLMLFILEISSUE_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <stdint.h>

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

static const QString Cellml_1_0 = "1.0";
static const QString Cellml_1_1 = "1.1";

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileIssue
{
public:
    enum Type
    {
        Error,
        Warning
    };

    explicit CellmlFileIssue(const Type &pType, const QString &pMessage,
                             const uint32_t &pLine = 0,
                             const uint32_t &pColumn = 0,
                             const QString &pImportedFile = QString());

    Type type() const;
    QString message() const;
    QString formattedMessage() const;
    uint32_t line() const;
    uint32_t column() const;
    QString importedFile() const;

private:
    Type mType;
    QString mMessage;
    uint32_t mLine;
    uint32_t mColumn;
    QString mImportedFile;
};

//==============================================================================

typedef QList<CellmlFileIssue> CellmlFileIssues;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
