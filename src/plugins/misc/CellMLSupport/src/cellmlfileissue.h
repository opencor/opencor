//==============================================================================
// CellML file issue
//==============================================================================

#ifndef CELLMLFILEISSUE_H
#define CELLMLFILEISSUE_H

//==============================================================================

#include "cellmlsupportglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

static const QString CellML_1_0 = "1.0";
static const QString CellML_1_1 = "1.1";

//==============================================================================

class CELLMLSUPPORT_EXPORT CellMLFileIssue
{
public:
    enum Type {
        Error,
        Warning
    };

    explicit CellMLFileIssue(const Type &pType, const QString &pMessage,
                             const int &pLine = 0, const int &pColumn = 0,
                             const QString &pImportedFile = QString());

    Type type() const;
    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;
    QString importedFile() const;

private:
    Type mType;
    QString mMessage;
    int mLine;
    int mColumn;
    QString mImportedFile;
};

//==============================================================================

typedef QList<CellMLFileIssue> CellMLFileIssues;

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
