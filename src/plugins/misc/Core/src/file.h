//==============================================================================
// File
//==============================================================================

#ifndef FILE_H
#define FILE_H

//==============================================================================

#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

class File
{
public:
    enum Status
    {
        Changed,
        Unchanged,
        Deleted
    };

    explicit File(const QString &pFileName);

    QString fileName() const;

    Status check();

    bool isModified() const;
    void setModified(const bool &pModified);

private:
    QString mFileName;
    QString mSha1;

    bool mModified;

    QString sha1() const;
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
