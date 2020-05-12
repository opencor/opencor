/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// CellML file runtime
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileissue.h"
#include "cellmlsupportglobal.h"

//==============================================================================

#include <QIcon>
#include <QList>
#include <QMap>
#ifdef Q_OS_WIN
    #include <QSet>
    #include <QVector>
#endif

//==============================================================================

#include "cellmlapibegin.h"
    #include "cellml-api-cxx-support.hpp"

    #include "IfaceCCGS.hxx"
    #include "IfaceCellML_APISPEC.hxx"
#include "cellmlapiend.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Compiler {
    class CompilerEngine;
} // namespace Compiler

//==============================================================================

namespace CellMLSupport {

//==============================================================================

class CellmlFile;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRuntimeParameter
{
public:
    enum class Type {
        Unknown,
        Voi,
        State,
        Rate,
        Constant,
        ComputedConstant,
        Algebraic,
        Floating,
        LocallyBound,
        Data
    };

    explicit CellmlFileRuntimeParameter(const QString &pName, int pDegree,
                                        const QString &pUnit,
                                        const QStringList &pComponentHierarchy,
                                        Type pType, int pIndex,
                                        double *pData = nullptr);

    static bool compare(CellmlFileRuntimeParameter *pParameter1,
                        CellmlFileRuntimeParameter *pParameter2);

    QString name() const;
    int degree() const;
    QString unit() const;
    QStringList componentHierarchy() const;
    Type type() const;
    int index() const;
    double * data() const;

    QString formattedName() const;
    QString formattedComponentHierarchy() const;
    QString fullyFormattedName() const;

    QString formattedUnit(const QString &pVoiUnit) const;

    static QMap<int, QIcon> icons();
    static QIcon icon(Type pParameterType);

private:
    QString mName;
    int mDegree;
    QString mUnit;
    QStringList mComponentHierarchy;
    Type mType;
    int mIndex;
    double *mData;
};

//==============================================================================

using CellmlFileRuntimeParameters = QList<CellmlFileRuntimeParameter *>;

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileRuntime : public QObject
{
    Q_OBJECT

public:
    using InitializeConstantsFunction = void (*)(double *CONSTANTS, double *RATES, double *STATES);
    using ComputeComputedConstantsFunction = void (*)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);
    using ComputeVariablesFunction = void (*)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);
    using ComputeRatesFunction = void (*)(double VOI, double *CONSTANTS, double *RATES, double *STATES, double *ALGEBRAIC);

    explicit CellmlFileRuntime(CellmlFile *pCellmlFile);
    ~CellmlFileRuntime() override;

    void update(CellmlFile *pCellmlFile, bool pAll = true);

    bool isValid() const;

    bool needNlaSolver() const;

    void importData(const QString &pName,
                    const QStringList &pComponentHierarchy, int pIndex,
                    double *pData);

    int constantsCount() const;
    int statesCount() const;
    int ratesCount() const;
    int algebraicCount() const;

    InitializeConstantsFunction initializeConstants() const;
    ComputeComputedConstantsFunction computeComputedConstants() const;
    ComputeVariablesFunction computeVariables() const;
    ComputeRatesFunction computeRates() const;

    CellmlFileIssues issues() const;

    CellmlFileRuntimeParameters parameters() const;
    CellmlFileRuntimeParameters dataParameters(const double *pData = nullptr) const;

    CellmlFileRuntimeParameter * voi() const;

private:
    bool mAtLeastOneNlaSystem = false;

    ObjRef<iface::cellml_services::CodeInformation> mCodeInformation = nullptr;

    int mConstantsCount = 0;
    int mStatesRatesCount = 0;
    int mAlgebraicCount = 0;

    Compiler::CompilerEngine *mCompilerEngine = nullptr;

    CellmlFileIssues mIssues;

    CellmlFileRuntimeParameter *mVoi = nullptr;
    CellmlFileRuntimeParameters mParameters;

    InitializeConstantsFunction mInitializeConstants = nullptr;
    ComputeComputedConstantsFunction mComputeComputedConstants = nullptr;
    ComputeVariablesFunction mComputeVariables = nullptr;
    ComputeRatesFunction mComputeRates = nullptr;

    void resetCodeInformation();

    void resetFunctions();

    void reset(bool pRecreateCompilerEngine, bool pResetIssues, bool pResetAll);

    void couldNotGenerateModelCodeIssue(const QString &pExtraInfo);
    void unknownProblemDuringModelCodeGenerationIssue();

    void checkCodeInformation(iface::cellml_services::CodeInformation *pCodeInformation);

    void retrieveCodeInformation(iface::cellml_api::Model *pModel);

    QString cleanCode(const std::wstring &pCode);
    QString methodCode(const QString &pCodeSignature, const QString &pCodeBody);
    QString methodCode(const QString &pCodeSignature,
                       const std::wstring &pCodeBody);

    QStringList componentHierarchy(iface::cellml_api::CellMLElement *pElement);
};

//==============================================================================

} // namespace CellMLSupport
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
