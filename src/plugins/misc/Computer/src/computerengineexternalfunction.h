//==============================================================================
// Computer engine external function class
//==============================================================================

#ifndef COMPUTERENGINEEXTERNALFUNCTION_H
#define COMPUTERENGINEEXTERNALFUNCTION_H

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerEngineExternalFunction
{
public:
    explicit ComputerEngineExternalFunction(const QString &pName,
                                            const int &pNbOfParameters);

    QString name() const;
    int nbOfParameters() const;

private:
    QString mName;
    int mNbOfParameters;
};

//==============================================================================

class ComputerEngineExternalFunctions : public QList<ComputerEngineExternalFunction>
{
public:
    bool contains(const ComputerEngineExternalFunction &pExternalFunction) const;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
