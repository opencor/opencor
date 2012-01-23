//==============================================================================
// Computer parameter class
//==============================================================================

#ifndef COMPUTERPARAMETER_H
#define COMPUTERPARAMETER_H

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerParameter
{
public:
    explicit ComputerParameter(const QString &pName, const bool &pPointer);

    QString name() const;
    bool pointer() const;

private:
    QString mName;
    bool mPointer;
};

//==============================================================================

class ComputerParameters : public QList<ComputerParameter>
{
public:
    bool contains(const ComputerParameter &pParameter) const;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
