//==============================================================================
// CellML model class
//==============================================================================

#include "cellmlmodel.h"

//==============================================================================

#include <QDebug>
#include <algorithm>

//==============================================================================

#include "CCGSBootstrap.hpp"
#include "CellMLBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

const wchar_t*
TypeToString(iface::cellml_services::VariableEvaluationType vet)
{
  switch (vet)
  {
  case iface::cellml_services::VARIABLE_OF_INTEGRATION:
    return L"variable of integration";
  case iface::cellml_services::CONSTANT:
    return L"constant";
  case iface::cellml_services::STATE_VARIABLE:
    return L"state variable";
  case iface::cellml_services::PSEUDOSTATE_VARIABLE:
    return L"pseudo-state variable";
  case iface::cellml_services::ALGEBRAIC:
    return L"algebraic variable";
  case iface::cellml_services::FLOATING:
    return L"uncomputed";
  case iface::cellml_services::LOCALLY_BOUND:
    return L"locally bound";
  }

  return L"invalid type";
}

//==============================================================================

CellmlModel::CellmlModel(const QUrl &pUrl) :
    mUrl(pUrl),
    mModel(0),
    mCodeInformation(0)
{
    // Get a bootstrap object

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();

    // Retrieve the model loader

    ObjRef<iface::cellml_api::DOMModelLoader> modelLoader = cellmlBootstrap->modelLoader();

    // Load the CellML model which URL we were given

    try {
        mModel = modelLoader->loadFromURL(pUrl.toString().toStdWString().c_str());
    } catch (iface::cellml_api::CellMLException &) {
        // Something went wrong, so generate an error message

        mErrorMessage = QString("CellML model loader error: %1").arg(QString::fromWCharArray(modelLoader->lastErrorMessage()));

        return;
    }

    // In the case of a non CellML 1.0 model, we want all imports to be fully
    // instantiated

    if (QString::fromWCharArray(mModel->cellmlVersion()).compare(Cellml_1_0))
        mModel->fullyInstantiateImports();

    // Make sure that we can generate some code and get the initial code
    // information

    ObjRef<iface::cellml_services::CodeGeneratorBootstrap> codeGeneratorBootstrap = CreateCodeGeneratorBootstrap();
    ObjRef<iface::cellml_services::CodeGenerator> codeGenerator = codeGeneratorBootstrap->createCodeGenerator();

    try {
        mCodeInformation = codeGenerator->generateCode(mModel);
    } catch (...) {
        mErrorMessage = "CellML code genereation error.";

        resetProperties();

        return;
    }

    // Check that no problem occurred during the code generation

    QString codeInformationErrorMessage = QString::fromWCharArray(mCodeInformation->errorMessage());

    if (!codeInformationErrorMessage.isEmpty()) {
        mErrorMessage = QString("CellML code generation error: %1").arg(codeInformationErrorMessage);

        resetProperties();

        return;
    }

    // Now that we have the code information, we can generate the corresponding
    // binary code
//---GRY--- Currently a rough copy/paste of the WriteCode function from the
//          CellML2C CCGS test

    qDebug() << "=======================================";

    iface::cellml_services::ModelConstraintLevel modelConstraintLevel = mCodeInformation->constraintLevel();

    if (modelConstraintLevel == iface::cellml_services::UNDERCONSTRAINED) {
        qDebug("/* The model is underconstrained");
        qDebug(" * List of undefined targets follows...");

        ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = mCodeInformation->iterateTargets();
        iface::cellml_services::ComputationTarget* computationTarget;
      std::vector<std::wstring> messages;
      while (true)
      {
        computationTarget = computationTargetIterator->nextComputationTarget();
        if (computationTarget == NULL)
          break;
        if (computationTarget->type() != iface::cellml_services::FLOATING)
        {
          computationTarget->release_ref();
          continue;
        }
        iface::cellml_api::CellMLVariable* v = computationTarget->variable();
        wchar_t* n = v->name();
        wchar_t* c = v->componentName();
        std::wstring str = L" * * ";
        uint32_t deg = computationTarget->degree();
        if (deg != 0)
        {
          str += L"d^";
          wchar_t buf[20];
          any_swprintf(buf, 20, L"%u", deg);
          str += buf;
          str += L"/dt^";
          str += buf;
          str += L" ";
        }
        str += n;
        str += L" (in ";
        str += c;
        str += L")\n";
        free(n);
        free(c);
        messages.push_back(str);
        v->release_ref();
        computationTarget->release_ref();
      }
      // Sort the messages...
      std::sort(messages.begin(), messages.end());
      std::vector<std::wstring>::iterator msgi;
      for (msgi = messages.begin(); msgi != messages.end(); msgi++)
          qDebug(QString::fromStdWString(*msgi).toLatin1().constData());
      qDebug(" */");
      return;
    } else if (modelConstraintLevel == iface::cellml_services::OVERCONSTRAINED) {
      qDebug("/* Model is overconstrained.");
      qDebug(" * List variables defined at time of error follows...");
      ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = mCodeInformation->iterateTargets();
      iface::cellml_services::ComputationTarget* computationTarget;
      std::vector<std::wstring> messages;
      while (true)
      {
        computationTarget = computationTargetIterator->nextComputationTarget();
        if (computationTarget == NULL)
          break;
        if (computationTarget->type() == iface::cellml_services::FLOATING)
        {
          computationTarget->release_ref();
          continue;
        }
        iface::cellml_api::CellMLVariable* v = computationTarget->variable();
        wchar_t* n = v->name();
        std::wstring str = L" * * ";
        uint32_t deg = computationTarget->degree();
        if (deg != 0)
        {
          str += L"d^";
          wchar_t buf[20];
          any_swprintf(buf, 20, L"%u", deg);
          str += buf;
          str += L"/dt^";
          str += buf;
          str += L" ";
        }
        str += n;
        free(n);
        str += L"\n";
        messages.push_back(str);
        v->release_ref();
        computationTarget->release_ref();
      }

      // Sort the messages...
      std::sort(messages.begin(), messages.end());
      std::vector<std::wstring>::iterator msgi;
      for (msgi = messages.begin(); msgi != messages.end(); msgi++)
          qDebug(QString::fromStdWString(*msgi).toLatin1().constData());

      // Get flagged equations...
      iface::mathml_dom::MathMLNodeList* mnl = mCodeInformation->flaggedEquations();
      qDebug(" * Extraneous equation was:");
      iface::dom::Node* n = mnl->item(0);
      mnl->release_ref();
      iface::dom::Element* el =
        reinterpret_cast<iface::dom::Element*>(n->query_interface("dom::Element"));
      n->release_ref();

      wchar_t* cmeta = el->getAttribute(L"id");
      if (!wcscmp(cmeta, L""))
        qDebug(" *   <equation with no cmeta ID>");
      else
        qDebug(" *   %s", cmeta);
      free(cmeta);

      n = el->parentNode();
      el->release_ref();

      if (n != NULL)
      {
        el = reinterpret_cast<iface::dom::Element*>
          (n->query_interface("dom::Element"));
        n->release_ref();

        cmeta = el->getAttribute(L"id");
        if (!wcscmp(cmeta, L""))
          qDebug(" *   in <math with no cmeta ID>");
        else
          qDebug(" *   in math with cmeta:id %s", cmeta);
        free(cmeta);

        el->release_ref();
      }

      qDebug(" */");
      return;
    } else if (modelConstraintLevel == iface::cellml_services::UNSUITABLY_CONSTRAINED) {
      qDebug("/* Model is unsuitably constrained (i.e. would need capabilities");
      qDebug(" beyond those of the CCGS to solve).");
      qDebug(" * The status of variables at time of error follows...");
      ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = mCodeInformation->iterateTargets();
      iface::cellml_services::ComputationTarget* computationTarget;
      std::vector<std::wstring> messages;
      while (true)
      {
        computationTarget = computationTargetIterator->nextComputationTarget();
        if (computationTarget == NULL)
          break;
        std::wstring str = L" * * ";
        if (computationTarget->type() == iface::cellml_services::FLOATING)
          str += L" Undefined: ";
        else
          str += L" Defined: ";

        uint32_t deg = computationTarget->degree();
        if (deg != 0)
        {
          str += L"d^";
          wchar_t buf[20];
          any_swprintf(buf, 20, L"%u", deg);
          str += buf;
          str += L"/dt^";
          str += buf;
          str += L" ";
        }
        iface::cellml_api::CellMLVariable* v = computationTarget->variable();
        wchar_t* n = v->name();
        str += n;
        free(n);
        str += L"\n";
        messages.push_back(str);
        v->release_ref();
        computationTarget->release_ref();
      }

      // Sort the messages...
      std::sort(messages.begin(), messages.end());
      std::vector<std::wstring>::iterator msgi;
      for (msgi = messages.begin(); msgi != messages.end(); msgi++)
          qDebug(QString::fromStdWString(*msgi).toLatin1().constData());

      qDebug(" */");
      return;
    }

    qDebug("/* Model is correctly constrained.");
    iface::mathml_dom::MathMLNodeList* mnl = mCodeInformation->flaggedEquations();
    uint32_t i, l = mnl->length();
    if (l == 0)
      qDebug(" * No equations needed Newton-Raphson evaluation.");
    else
      qDebug(" * The following equations needed Newton-Raphson evaluation:");

    std::vector<std::wstring> messages;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = mnl->item(i);
      iface::dom::Element* el =
        reinterpret_cast<iface::dom::Element*>(n->query_interface("dom::Element"));
      n->release_ref();

      wchar_t* cmeta = el->getAttribute(L"id");
      std::wstring str;
      if (!wcscmp(cmeta, L""))
        str += L" *   <equation with no cmeta ID>\n";
      else
      {
        str += L" *   ";
        str += cmeta;
        str += L"\n";
      }
      free(cmeta);

      n = el->parentNode();
      el->release_ref();

      el = reinterpret_cast<iface::dom::Element*>
        (n->query_interface("dom::Element"));
      n->release_ref();

      cmeta = el->getAttribute(L"id");
      if (!wcscmp(cmeta, L""))
        str += L" *   in <math with no cmeta ID>\n";
      else
      {
        str += L" *   in math with cmeta:id ";
        str += cmeta;
        str += L"\n";
      }
      free(cmeta);
      el->release_ref();

      messages.push_back(str);
    }
    mnl->release_ref();

    // Sort the messages...
    std::sort(messages.begin(), messages.end());
    std::vector<std::wstring>::iterator msgi;
    for (msgi = messages.begin(); msgi != messages.end(); msgi++)
      qDebug(QString::fromStdWString(*msgi).toLatin1().constData());

    qDebug(" * The rate and state arrays need %u entries.", mCodeInformation->rateIndexCount());
    qDebug(" * The algebraic variables array needs %u entries.", mCodeInformation->algebraicIndexCount());
    qDebug(" * The constant array needs %u entries.", mCodeInformation->constantIndexCount());
    qDebug(" * Variable storage is as follows:");

    messages.clear();
    ObjRef<iface::cellml_services::ComputationTargetIterator> computationTargetIterator = mCodeInformation->iterateTargets();
    while (true)
    {
      iface::cellml_services::ComputationTarget* computationTarget = computationTargetIterator->nextComputationTarget();
      if (computationTarget == NULL)
        break;
      iface::cellml_api::CellMLVariable* v = computationTarget->variable();
      iface::cellml_api::CellMLElement* el = v->parentElement();
      iface::cellml_api::CellMLComponent* c =
        reinterpret_cast<iface::cellml_api::CellMLComponent*>
        (el->query_interface("cellml_api::CellMLComponent"));
      el->release_ref();

      std::wstring str;
      wchar_t* vn = v->name(), * cn = c->name();
      str += L" * * Target ";
      uint32_t deg = computationTarget->degree();
      if (deg != 0)
      {
        str += L"d^";
        wchar_t buf[20];
        any_swprintf(buf, 20, L"%u", deg);
        str += buf;
        str += L"/dt^";
        str += buf;
        str += L" ";
      }
      str += vn;
      str += L" in component ";
      str += cn;
      str += L"\n";
      free(vn);
      free(cn);

      c->release_ref();
      v->release_ref();

      str += L" * * * Variable type: ";
      str += TypeToString(computationTarget->type());
      str += L"\n * * * Variable index: ";
      wchar_t buf[40];
      any_swprintf(buf, 40, L"%u\n", computationTarget->assignedIndex());
      str += buf;

      str += L" * * * Variable storage: ";
      wchar_t * vsn;
      vsn = computationTarget->name();
      str += vsn;
      free(vsn);
      str += '\n';

      computationTarget->release_ref();

      messages.push_back(str);
    }

    // Sort the messages...
    std::sort(messages.begin(), messages.end());
    for (msgi = messages.begin(); msgi != messages.end(); msgi++)
        qDebug(QString::fromStdWString(*msgi).toLatin1().constData());

    qDebug(" */");

    wchar_t* frag = mCodeInformation->functionsString();
    qDebug(QString::fromWCharArray(frag).toLatin1().constData());
    free(frag);

    // Now start the code...
    frag = mCodeInformation->initConstsString();
    qDebug("void SetupFixedConstants(double* CONSTANTS, double* RATES, double* STATES)");
    qDebug("{\n%s}", QString::fromWCharArray(frag).toLatin1().constData());
    free(frag);

    frag = mCodeInformation->variablesString();
    qDebug("void EvaluateVariables(double VOI, double* CONSTANTS, double* RATES, double* STATES, double* ALGEBRAIC)");
    qDebug("{\n%s}", QString::fromWCharArray(frag).toLatin1().constData());
    free(frag);

    frag = mCodeInformation->ratesString();
    qDebug("void ComputeRates(double VOI, double* STATES, double* RATES, double* CONSTANTS, double* ALGEBRAIC)");
    qDebug("{\n%s}", QString::fromWCharArray(frag).toLatin1().constData());
    free(frag);
}

//==============================================================================

void CellmlModel::resetProperties()
{
    // We arrived here because something went wrong with our use of the CellML
    // API, so we must delete anything that may have been allocated and reset
    // it, so that the class user doesn't get confused

    delete mModel;           mModel = 0;
    delete mCodeInformation; mCodeInformation = 0;
}

//==============================================================================

bool CellmlModel::isValid()
{
    // The CellML model object is valid if there is no error message

    return mErrorMessage.isEmpty();
}

//==============================================================================

QString CellmlModel::errorMessage()
{
    // Return the error message

    return mErrorMessage;
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
