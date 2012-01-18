//==============================================================================
// CellML model runtime class
//==============================================================================

#ifndef CELLMLMODELRUNTIME_H
#define CELLMLMODELRUNTIME_H

//==============================================================================

#include "cellmlmodelissue.h"
#include "cellmlsupportglobal.h"
#include "computerengine.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCCGS.hxx"
#include "IfaceCellML_APISPEC.hxx"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlModelRuntime : public QObject
{
    Q_OBJECT

public:
    enum ModelType
    {
        Ode,
        Dae,
        Undefined
    };

    explicit CellmlModelRuntime();

    bool isValid();

    ModelType modelType();

    QList<CellmlModelIssue> issues();

    CellmlModelRuntime * update(iface::cellml_api::Model *pModel,
                                const bool &pValidModel);

private:
    ModelType mModelType;

    ObjRef<iface::cellml_services::CodeInformation> mOdeCodeInformation;
    ObjRef<iface::cellml_services::IDACodeInformation> mDaeCodeInformation;

    QList<CellmlModelIssue> mIssues;

    void resetOdeCodeInformation();
    void resetDaeCodeInformation();
    void reset();

    void couldNotCompileModelIssue();
    void unexpectedProblemDuringModelCompilationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

    iface::cellml_services::CodeInformation * getOdeCodeInformation(iface::cellml_api::Model *pModel);
    iface::cellml_services::CodeInformation * getDaeCodeInformation(iface::cellml_api::Model *pModel);

    void handleErrors(Computer::ComputerEngine &pComputerEngine,
                      const QString &pFunctionName);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
