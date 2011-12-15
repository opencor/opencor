//==============================================================================
// CellML model runtime class
//==============================================================================

#include "cellmlmodelruntime.h"

//==============================================================================

#include "cellml-api-cxx-support.hpp"

#include "IfaceCeVAS.hxx"
#include "IfaceCUSES.hxx"

#include "CeVASBootstrap.hpp"
#include "CUSESBootstrap.hpp"


#include <QDebug>

#include <IfaceCellML_APISPEC.hxx>
#include <IfaceCCGS.hxx>
#include <CeVASBootstrap.hpp>
#include <MaLaESBootstrap.hpp>
#include <CCGSBootstrap.hpp>
#include <CellMLBootstrap.hpp>
#include <IfaceAnnoTools.hxx>
#include <AnnoToolsBootstrap.hpp>













#include <exception>
#include "cda_compiler_support.h"
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceCCGS.hxx"
#include "IfaceAnnoTools.hxx"
#include "CCGSBootstrap.hpp"
#include "CellMLBootstrap.hpp"
#include "AnnoToolsBootstrap.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <string>
#include <set>













//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

bool CellmlModelRuntime::isValid()
{
    // The runtime is valid if no issue was found

    return mIssues.isEmpty();
}

//==============================================================================

QList<CellmlModelIssue> CellmlModelRuntime::issues()
{
    // Return the issue(s)

    return mIssues;
}

//==============================================================================

void CellmlModelRuntime::reset()
{
    // Reset all of the runtime's properties

//---GRY--- TO BE DONE...
}

//==============================================================================

CellmlModelRuntime * CellmlModelRuntime::update(iface::cellml_api::Model *pModel,
                                                const bool &pValidModel)
{
    // Reset ourselves

    reset();

    // Check that the model is valid

    if (pValidModel) {
        // The model is valid, so we can generate our runtime
        // Note: this is based on CodeGenerationState::GenerateCode() from the
        //       CellML API. Indeed, we can't use the CellML API to generate our
        //       runtime, but we can certainly re-use it, so...

        // Create a CellML Variable Association Service (CeVAS) object to help
        // us with the treatment of interconnected CellML variables

        ObjRef<iface::cellml_services::CeVASBootstrap> cevasBootstrap = CreateCeVASBootstrap();
        ObjRef<iface::cellml_services::CeVAS> cevas = cevasBootstrap->createCeVASForModel(pModel);
        QString cevasModelError = QString::fromStdWString(cevas->modelError());

        if (!cevasModelError.isEmpty()) {
            // Something went wrong with the creation of our CeVAS object, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model's variables could not be fully analysed")));

            return this;
        }

        // Create a CellML Units Simplification and Expansion Service (CUSES)
        // object to help us with the processing of physical units

        ObjRef<iface::cellml_services::CUSESBootstrap> cusesBootstrap = CreateCUSESBootstrap();
        ObjRef<iface::cellml_services::CUSES> cuses = cusesBootstrap->createCUSESForModel(pModel, false);
        QString cusesModelError = QString::fromStdWString(cuses->modelError());

        if (!cusesModelError.isEmpty()) {
            // Something went wrong with the creation of our CUSES object, so...

            mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                            tr("the model's units could not be fully processed")));

            reset();

            return this;
        }

        // Just testing the generation of 'C code'...

qDebug() << QString::fromStdWString(getModelAsCCode(pModel));

printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
customGen(pModel);
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    } else {
        // The model is not valid, so...

        mIssues.append(CellmlModelIssue(CellmlModelIssue::Error,
                                        tr("the model is not valid")));
    }

    // We are done, so return ourselves

    return this;
}

//==============================================================================

std::wstring CellmlModelRuntime::getModelAsCCode(iface::cellml_api::Model *pModel/*,void* _annotations*/)
{
//  iface::cellml_services::AnnotationSet* as = static_cast<iface::cellml_services::AnnotationSet*>(_annotations);
  std::wstring code;
  RETURN_INTO_OBJREF(cgb,iface::cellml_services::CodeGeneratorBootstrap,
    CreateCodeGeneratorBootstrap());
  RETURN_INTO_OBJREF(cg,iface::cellml_services::CodeGenerator,
    cgb->createCodeGenerator());
  /* The trunk MaLaES has been updated since the 1.5 release, so define a
   * "custom" MaLaES here
   */
  RETURN_INTO_OBJREF(mbs,iface::cellml_services::MaLaESBootstrap,
    CreateMaLaESBootstrap());
  RETURN_INTO_OBJREF(mt,iface::cellml_services::MaLaESTransform,
    mbs->compileTransformer(
      L"opengroup: (\r\n"
      L"closegroup: )\r\n"
      L"abs: #prec[H]fabs(#expr1)\r\n"
      L"and: #prec[20]#exprs[&&]\r\n"
      L"arccos: #prec[H]acos(#expr1)\r\n"
      L"arccosh: #prec[H]acosh(#expr1)\r\n"
      L"arccot: #prec[1000(900)]atan(1.0/#expr1)\r\n"
      L"arccoth: #prec[1000(900)]atanh(1.0/#expr1)\r\n"
      L"arccsc: #prec[1000(900)]asin(1/#expr1)\r\n"
      L"arccsch: #prec[1000(900)]asinh(1/#expr1)\r\n"
      L"arcsec: #prec[1000(900)]acos(1/#expr1)\r\n"
      L"arcsech: #prec[1000(900)]acosh(1/#expr1)\r\n"
      L"arcsin: #prec[H]asin(#expr1)\r\n"
      L"arcsinh: #prec[H]asinh(#expr1)\r\n"
      L"arctan: #prec[H]atan(#expr1)\r\n"
      L"arctanh: #prec[H]atanh(#expr1)\r\n"
      L"ceiling: #prec[H]ceil(#expr1)\r\n"
      L"cos: #prec[H]cos(#expr1)\r\n"
      L"cosh: #prec[H]cosh(#expr1)\r\n"
      L"cot: #prec[900(0)]1.0/tan(#expr1)\r\n"
      L"coth: #prec[900(0)]1.0/tanh(#expr1)\r\n"
      L"csc: #prec[900(0)]1.0/sin(#expr1)\r\n"
      L"csch: #prec[900(0)]1.0/sinh(#expr1)\r\n"
      L"diff: #lookupDiffVariable\r\n"
      L"divide: #prec[900]#expr1/#expr2\r\n"
      L"eq: #prec[30]#exprs[==]\r\n"
      L"exp: #prec[H]exp(#expr1)\r\n"
      L"factorial: #prec[H]factorial(#expr1)\r\n"
      L"factorof: #prec[30(900)]#expr1 % #expr2 == 0\r\n"
      L"floor: #prec[H]floor(#expr1)\r\n"
      L"gcd: #prec[H]gcd_multi(#count, #exprs[, ])\r\n"
      L"geq: #prec[30]#exprs[>=]\r\n"
      L"gt: #prec[30]#exprs[>]\r\n"
      L"implies: #prec[10(950)] !#expr1 || #expr2\r\n"
      L"int: #prec[H]defint(func#unique1, BOUND, CONSTANTS, RATES, VARIABLES, "
      L"#bvarIndex, pret)#supplement double func#unique1(double* BOUND, "
      L"double* CONSTANTS, double* RATES, double* VARIABLES, int* pret) { return #expr1; }\r\n"
      L"lcm: #prec[H]lcm_multi(#count, #exprs[, ])\r\n"
      L"leq: #prec[30]#exprs[<=]\r\n"
      L"ln: #prec[H]log(#expr1)\r\n"
      L"log: #prec[H]arbitrary_log(#expr1, #logbase)\r\n"
      L"lt: #prec[30]#exprs[<]\r\n"
      L"max: #prec[H]multi_max(#count, #exprs[, ])\r\n"
      L"min: #prec[H]multi_min(#count, #exprs[, ])\r\n"
      L"minus: #prec[500]#expr1 - #expr2\r\n"
      L"neq: #prec[30]#expr1 != #expr2\r\n"
      L"not: #prec[950]!#expr1\r\n"
      L"or: #prec[10]#exprs[||]\r\n"
      L"plus: #prec[500]#exprs[+]\r\n"
      L"power: #prec[H]pow(#expr1, #expr2)\r\n"
      L"quotient: #prec[1000(0)] (double)(((int)#expr2) == 0 ? #expr1 / 0.0 : (int)(#expr1) / (int)(#expr2))\r\n"
      L"rem: #prec[1000(0)] (double)(((int)#expr2) == 0 ? (#expr1) / 0.0 : (int)(#expr1) % (int)(#expr2))\r\n"
      L"root: #prec[1000(900)] pow(#expr1, 1.0 / #degree)\r\n"
      L"sec: #prec[900(0)]1.0 / cos(#expr1)\r\n"
      L"sech: #prec[900(0)]1.0 / cosh(#expr1)\r\n"
      L"sin: #prec[H] sin(#expr1)\r\n"
      L"sinh: #prec[H] sinh(#expr1)\r\n"
      L"tan: #prec[H] tan(#expr1)\r\n"
      L"tanh: #prec[H] tanh(#expr1)\r\n"
      L"times: #prec[900] #exprs[*]\r\n"
      L"unary_minus: #prec[950]- #expr1\r\n"
      L"units_conversion: #prec[500(900)]#expr1*#expr2 + #expr3\r\n"
      L"units_conversion_factor: #prec[900]#expr1*#expr2\r\n"
      L"units_conversion_offset: #prec[500]#expr1+#expr2\r\n"
      L"xor: #prec[25(30)] (#expr1 != 0) ^ (#expr2 != 0)\r\n"
      L"piecewise_first_case: #prec[1000(5)](#expr1 ? #expr2 : \r\n"
      L"piecewise_extra_case: #prec[1000(5)]#expr1 ? #expr2 : \r\n"
      L"piecewise_otherwise: #prec[1000(5)]#expr1)\r\n"
      L"piecewise_no_otherwise: #prec[1000(5)]0.0/0.0)\r\n"
      L"eulergamma: #prec[999]0.577215664901533\r\n"
      L"exponentiale: #prec[999]2.71828182845905\r\n"
      L"false: #prec[999]0.0\r\n"
      L"infinity: #prec[900]1.0/0.0\r\n"
      L"notanumber: #prec[999]0.0/0.0\r\n"
      L"pi: #prec[999] 3.14159265358979\r\n"
      L"true: #prec[999]1.0\r\n"));
  /* now can use the standard transformation?
  cg->transform(mt);
  */
  try
  {
    // annotate the source variables in the model with the code-names based on existing annotations
    RETURN_INTO_OBJREF(cvbs,iface::cellml_services::CeVASBootstrap,CreateCeVASBootstrap());
    RETURN_INTO_OBJREF(cevas,iface::cellml_services::CeVAS,cvbs->createCeVASForModel(pModel));
//    for (unsigned int i=0;i<cevas->length();i++)
//    {
//      RETURN_INTO_OBJREF(cvs,iface::cellml_services::ConnectedVariableSet,cevas->getVariableSet(i));
//      RETURN_INTO_OBJREF(sv,iface::cellml_api::CellMLVariable,cvs->sourceVariable());
//      RETURN_INTO_WSTRING(array,as->getStringAnnotation(sv,L"array"));
//      RETURN_INTO_WSTRING(index,as->getStringAnnotation(sv,L"array_index"));
//      if (!array.empty() && !index.empty())
//      {
//        std::wstring ename = array;
//        ename += L"[";
//        ename += index;
//        ename += L"]";
//        as->setStringAnnotation(sv,L"expression",ename.c_str());
//        if (array == L"OC_STATE")
//        {
//          ename = std::wstring(L"OC_RATE");
//          ename += L"[";
//          ename += index;
//          ename += L"]";
//          as->setStringAnnotation(sv,L"expression_d1",ename.c_str());
//        }
//      }
//    }
    cg->useCeVAS(cevas);
//    cg->useAnnoSet(as);
    RETURN_INTO_OBJREF(cci,iface::cellml_services::CodeInformation,cg->generateCode(pModel));
    wchar_t* m = cci->errorMessage();
    if (!wcscmp(m,L""))
    {
      iface::cellml_services::ModelConstraintLevel mcl = cci->constraintLevel();
      if (mcl == iface::cellml_services::UNDERCONSTRAINED)
      {
        qDebug() << "Model is underconstrained";
      }
      else if (mcl == iface::cellml_services::OVERCONSTRAINED)
      {
        qDebug() << "Model is overconstrained";
      }
      else if (mcl == iface::cellml_services::UNSUITABLY_CONSTRAINED)
      {
        qDebug() << "Model is unsuitably constrained";
      }
      else
      {
        qDebug() << "Model is correctly constrained";
        // create the code in the format we know how to handle
        code += L"#include <math.h>\n";
        code += L"#include <stdio.h>\n";
        /* required functions */
        code += L"extern double fabs(double x);\n";
        code += L"extern double acos(double x);\n";
        code += L"extern double acosh(double x);\n";
        code += L"extern double atan(double x);\n";
        code += L"extern double atanh(double x);\n";
        code += L"extern double asin(double x);\n";
        code += L"extern double asinh(double x);\n";
        code += L"extern double acos(double x);\n";
        code += L"extern double acosh(double x);\n";
        code += L"extern double asin(double x);\n";
        code += L"extern double asinh(double x);\n";
        code += L"extern double atan(double x);\n";
        code += L"extern double atanh(double x);\n";
        code += L"extern double ceil(double x);\n";
        code += L"extern double cos(double x);\n";
        code += L"extern double cosh(double x);\n";
        code += L"extern double tan(double x);\n";
        code += L"extern double tanh(double x);\n";
        code += L"extern double sin(double x);\n";
        code += L"extern double sinh(double x);\n";
        code += L"extern double exp(double x);\n";
        code += L"extern double floor(double x);\n";
        code += L"extern double pow(double x, double y);\n";
        code += L"extern double factorial(double x);\n";
        code += L"extern double log(double x);\n";
        code += L"extern double arbitrary_log(double x, double base);\n";
        code += L"extern double gcd_pair(double a, double b);\n";
        code += L"extern double lcm_pair(double a, double b);\n";
        code += L"extern double gcd_multi(unsigned int size, ...);\n";
        code += L"extern double lcm_multi(unsigned int size, ...);\n";
        code += L"extern double multi_min(unsigned int size, ...);\n";
        code += L"extern double multi_max(unsigned int size, ...);\n";
        code += L"extern void NR_MINIMISE(double(*func)"
          L"(double VOI, double *C, double *R, double *S, double *A),"
          L"double VOI, double *C, double *R, double *S, double *A, "
          L"double *V);\n";
        wchar_t* frag = cci->functionsString();
        code += frag;
        free(frag);

//        nBound = 1;
//        nRates = cci->rateIndexCount();
//        nAlgebraic = cci->algebraicIndexCount();
//        nConstants = cci->constantIndexCount();

        code += L"\n\nvoid OC_CellML_RHS_routine(double VOI, double* OC_STATE, double* OC_RATE, double* OC_WANTED, double* OC_KNOWN)\n{\n\n";
        code += L"double DUMMY_ASSIGNMENT;\n";
        code += L"double CONSTANTS[";
        code += QString::number(cci->constantIndexCount()).toStdWString();
        code += L"], ALGEBRAIC[";
        code += QString::number(cci->algebraicIndexCount()).toStdWString();
        code += L"];\n\n";

        // start the model code...
        /* https://svn.physiomeproject.org/svn/physiome/CellML_DOM_API/trunk/interfaces/CCGS.idl for full description */

        /* initConsts - all variables which aren't state variables but have
         *              an initial_value attribute, and any variables & rates
         *              which follow.
         */
        frag = cci->initConstsString();
        //code += L"void SetupFixedConstants(double* CONSTANTS,double* RATES,"
        //  L"double* STATES)\n{\n";
        code += frag;
        //code += L"}\n";
        free(frag);

        /* rates      - All rates which are not static.
         */
        frag = cci->ratesString();
        //code += L"void ComputeRates(double VOI,double* STATES,double* RATES,"
        //  L"double* CONSTANTS,double* ALGEBRAIC)\n{\n";
        code += frag;
        //code += L"}\n";
        free(frag);

        /* variables  - All variables not computed by initConsts or rates
         *  (i.e., these are not required for the integration of the model and
         *   thus only need to be called for output or presentation or similar
         *   purposes)
         */
        frag = cci->variablesString();
        //code += L"void EvaluateVariables(double VOI,double* CONSTANTS,"
        //  L"double* RATES, double* STATES, double* ALGEBRAIC)\n{\n";
        code += frag;
        //code += L"}\n";
        free(frag);

        // and now clear out initialisation of state variables and known variables.
//        clearCodeAssignments(code,L"OC_STATE",mStateCounter);
//        clearCodeAssignments(code,L"OC_KNOWN",mParameterCounter);

        // close the subroutine
        code += L"\n\n}//OC_CellML_RHS_routine()\n\n;";
      }
    }
    else
    {
      qDebug() << "Error generating code:" << m;
    }
    free(m);
  }
  catch (...)
  {
    qDebug() << L"Error generating the code information for model";
  }
  return code;
}

//==============================================================================






















std::wstring
MakeWideString(const char* input)
{
  wchar_t* tmp;
  size_t l = strlen(input);
  tmp = new wchar_t[l + 1];
  memset(tmp, 0, (l + 1) * sizeof(wchar_t));
  const char* mbr = input;
  mbsrtowcs(tmp, &mbr, l, NULL);
  std::wstring str(tmp);
  delete [] tmp;

  return str;
}

//void
//TargetDescriptionToSet(char* descr,
//                       std::set<std::pair<std::pair<std::wstring, std::wstring>, uint32_t> >&
//                       targs)
//{
//  int phase = 0;
//  char * comp = descr, * var = NULL, * number = NULL;
//  for (char c = *descr; c != 0; c = *++descr)
//  {
//    switch (phase)
//    {
//    case 0:
//      if (c == ',')
//      {
//        if (comp != descr)
//          printf("Warning: premature comma in command line variable list. Parse results will be wrong.\n");
//        return;
//      }
//      if (c == '/')
//      {
//        *descr = 0;
//        var = descr + 1;
//        phase = 1;
//      }
//      continue;
//    case 1:
//      if (c == ',')
//      {
//        printf("Warning: Comma after / but before : in command line variable list. Parse results will be wrong.\n");
//        return;
//      }
//      if (c == ':')
//      {
//        *descr = 0;
//        number = descr + 1;
//        phase = 2;
//      }
//      continue;
//    case 2:
//      if (c == ',')
//      {
//        targs.insert(std::pair<std::pair<std::wstring, std::wstring>,
//                               uint32_t>
//                     (std::pair<std::wstring, std::wstring>
//                      (
//                       MakeWideString(comp), MakeWideString(var)
//                      ),
//                      strtoul(number, NULL, 10)
//                     )
//                    );

//        comp = descr + 1;
//        phase = 0;
//      }
//      continue;
//    }
//  }
//  if (phase == 2)
//  {
//    targs.insert(std::pair<std::pair<std::wstring, std::wstring>,
//                 uint32_t>
//      (std::pair<std::wstring, std::wstring>
//       (
//        MakeWideString(comp), MakeWideString(var)
//        ),
//       strtoul(number, NULL, 10)
//       )
//                 );
//  }
//  else
//    printf("Warning: unexpected end of command line variable list in phase %u. Parse results will be wrong.\n",
//           phase);

//}

int CellmlModelRuntime::customGen(iface::cellml_api::Model *pModel)
{
//  if (argc < 5)
//  {
//    printf("Usage: CustomGen modelURL wanted known unwanted\n"
//           "Each of wanted, known, and unwanted is a comma separated list.\n"
//           "Use a single comma to specify an empty list.\n"
//           "Each entry in the list should be in the form:\n"
//           "  component/variable:degreeOfDerivative\n");
//    return -1;
//  }

//  std::wstring URL(MakeWideString(argv[1]));

  std::set<std::pair<std::pair<std::wstring, std::wstring>, uint32_t> > wanted, known, unwanted;

//  TargetDescriptionToSet(argv[2], wanted);
//  TargetDescriptionToSet(argv[3], known);
//  TargetDescriptionToSet(argv[4], unwanted);

  RETURN_INTO_OBJREF(cb, iface::cellml_api::CellMLBootstrap,
                     CreateCellMLBootstrap());
  RETURN_INTO_OBJREF(ml, iface::cellml_api::ModelLoader,
                     cb->modelLoader());
  // These assignments to NULL are only here in the test code to help look for
  // memory errors by freeing things early... in production code, they wouldn't
  // be necessary.
  cb = NULL;

//  ObjRef<iface::cellml_api::Model> mod;
//  try
//  {
//    mod = already_AddRefd<iface::cellml_api::Model>(ml->loadFromURL(URL.c_str()));
//  }
//  catch (...)
//  {
//    printf("Error loading model URL.\n");
//    return -1;
//  }

  RETURN_INTO_OBJREF(cgb, iface::cellml_services::CodeGeneratorBootstrap,
                     CreateCodeGeneratorBootstrap()
                    );
  RETURN_INTO_OBJREF(cg, iface::cellml_services::CodeGenerator,
                     cgb->createCodeGenerator());
  cgb = NULL;

  cg->stateVariableNamePattern(L"VARS[%]");
  RETURN_INTO_OBJREF(ccg, iface::cellml_services::CustomGenerator,
                     cg->createCustomGenerator(pModel));
  cg = NULL;
//  mod = NULL;
  RETURN_INTO_OBJREF(cti, iface::cellml_services::ComputationTargetIterator,
                     ccg->iterateTargets());
  while (true)
  {
    RETURN_INTO_OBJREF(ct, iface::cellml_services::ComputationTarget,
                       cti->nextComputationTarget());
    if (ct == NULL)
      break;

    RETURN_INTO_OBJREF(cv, iface::cellml_api::CellMLVariable, ct->variable());
    RETURN_INTO_WSTRING(compname, cv->componentName());
    RETURN_INTO_WSTRING(varname, cv->name());
    std::pair<std::pair<std::wstring, std::wstring>, uint32_t> p
      (std::pair<std::wstring, std::wstring>(compname, varname), ct->degree());
//    if (wanted.count(p))
if (ct->type() == iface::cellml_services::STATE_VARIABLE)
      ccg->requestComputation(ct);
//    else if (known.count(p))
else if (ct->type() == iface::cellml_services::VARIABLE_OF_INTEGRATION)
      ccg->markAsKnown(ct);
    else if (unwanted.count(p))
      ccg->markAsUnwanted(ct);
  }
  cti = NULL;

  RETURN_INTO_OBJREF(cci, iface::cellml_services::CustomCodeInformation,
                     ccg->generateCode());
  ccg = NULL;
  printf("Constraint level = ");
  switch (cci->constraintLevel())
  {
  case iface::cellml_services::UNDERCONSTRAINED:
    printf("UNDERCONSTRAINED\n");
    break;
  case iface::cellml_services::UNSUITABLY_CONSTRAINED:
    printf("UNSUITABLY_CONSTRAINED\n");
    break;
  case iface::cellml_services::OVERCONSTRAINED:
    printf("OVERCONSTRAINED\n");
    break;
  case iface::cellml_services::CORRECTLY_CONSTRAINED:
    printf("CORRECTLY_CONSTRAINED\n");
    break;
  default:
    printf("Unkown value\n");
  }
  printf("Index count: %u\n", cci->indexCount());
  cti = already_AddRefd<iface::cellml_services::ComputationTargetIterator>(cci->iterateTargets());
  while (true)
  {
    RETURN_INTO_OBJREF(ct, iface::cellml_services::ComputationTarget,
                       cti->nextComputationTarget());
    if (ct == NULL)
      break;

    RETURN_INTO_OBJREF(cv, iface::cellml_api::CellMLVariable, ct->variable());
    RETURN_INTO_WSTRING(compname, cv->componentName());
    RETURN_INTO_WSTRING(varname, cv->name());
    printf("* Computation target %S/%S:%u:\n", compname.c_str(), varname.c_str(),
           ct->degree());
    printf("  => Type = ");
    switch (ct->type())
    {
    case iface::cellml_services::VARIABLE_OF_INTEGRATION:
      printf("VARIABLE_OF_INTEGRATION - was marked as independent.\n");
      break;
    case iface::cellml_services::CONSTANT:
      printf("CONSTANT - this should not happen!\n");
      break;
    case iface::cellml_services::STATE_VARIABLE:
      printf("STATE_VARIABLE - was requested, and is available.\n");
      break;
    case iface::cellml_services::ALGEBRAIC:
      printf("ALGEBRAIC - is used as an intermediate.\n");
      break;
    case iface::cellml_services::FLOATING:
      printf("FLOATING - unused and not requested.\n");
      break;
    case iface::cellml_services::LOCALLY_BOUND:
      printf("LOCALLY_BOUND - locally bound in expressions only.\n");
      break;
    case iface::cellml_services::PSEUDOSTATE_VARIABLE:
      printf("PSEUDOSTATE_VARIABLE - target was requested, but could "
             "not be computed from the independent variables and model.\n");
      break;
    default:
      printf("Unknown type!\n");
    }
    RETURN_INTO_WSTRING(targname, ct->name());
    printf("  => Name = %S\n", targname.c_str());
    printf("  => Index = %u\n", ct->assignedIndex());
  }
  // To do: Print output from cci->iterateTargets();
  RETURN_INTO_WSTRING(functionsString, cci->functionsString());
  printf("Functions: %S\n", functionsString.c_str());
  RETURN_INTO_WSTRING(code, cci->generatedCode());
  printf("Code: %S\n", code.c_str());
}

























}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
