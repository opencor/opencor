//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================
// Note: this exporter is adapted from David Nickerson's code which is an
//       updated version of Jonathan Cooper's CellML 1.1 to 1.0 converter. The
//       update was to make Jonathan's code work with version 1.12 of the CellML
//       API. In all cases, the original limitations apply...
//       Original code: http://www.cellml.org/tools/jonathan-cooper-s-cellml-1-1-to-1-0-converter/versionconverter-tar.bz2/view
//       Updated code: https://github.com/nickerso/flattenCellML
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include <QApplication>

//==============================================================================

#include "AnnoToolsBootstrap.hpp"
#include "CeVASBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellml10Exporter::CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                                       const QString &pFileName) :
    CellmlFileCellmlExporter(pModel, L"1.0"),
    mCopiedUnits(QSet<QPair<QString, QString> >()),
    mComponentNames(QSet<QString>())
{
    // Set the model's name and cmeta:id, if any

    mExportedModel->name(pModel->name());

    std::wstring cmetaId = pModel->cmetaId();

    if (cmetaId.length())
        mExportedModel->cmetaId(cmetaId);

    // Create an annotation set to manage annotations

    ObjRef<iface::cellml_services::AnnotationToolService> annotationToolService = CreateAnnotationToolService();

    mAnnotationSet = annotationToolService->createAnnotationSet();

    // Create a CellML Variable Association Service object so that we can find
    // relevant components

    ObjRef<iface::cellml_services::CeVASBootstrap> cevasBootstrap = CreateCeVASBootstrap();
    ObjRef<iface::cellml_services::CeVAS> cevas = cevasBootstrap->createCeVASForModel(pModel);

    std::wstring cevasError = cevas->modelError();

    if (cevasError.length()) {
        mErrorMessage = QObject::tr("a CeVAS object could not be created");

        return;
    }

    // Copy all (i.e. both local and imported) model-level units to our exported
    // model

    ObjRef<iface::cellml_api::UnitsSet> unitsSet = pModel->allUnits();

    copyUnitsSet(unitsSet, mExportedModel);

    // Annotate imported components since they may have been renamed

    annotateImportedComponents(mModel);

    // Copy all needed components to our exported model

    copyComponents(cevas);

    // Copy all groups to our exported model

    copyGroups(mModel);

    // Copy all connections to our exported model

    copyConnections(mModel);

    // Deal with 'initial_value="var_name"' occurrences

    propagateInitialValues();

    // Save our exported model

    mResult = saveModel(mExportedModel, pFileName);
}

//==============================================================================

void CellmlFileCellml10Exporter::copyUnitsSet(iface::cellml_api::UnitsSet *pUnitsSet,
                                              iface::cellml_api::CellMLElement *pElement)
{
    // Copy all the units to the model/component element, should there be some
    // Note: the element is in a different model to the units, so we can't just
    //       clone them...

    if (!pUnitsSet->length())
        return;

    ObjRef<iface::cellml_api::Model> model = pElement->modelElement();
    ObjRef<iface::cellml_api::UnitsIterator> unitsIterator = pUnitsSet->iterateUnits();

    forever {
        ObjRef<iface::cellml_api::Units> units = unitsIterator->nextUnits();

        if (!units)
            break;

        // Copy the units to the model/component element, but only if it isn't
        // already in this model

        ObjRef<iface::cellml_api::Model> unitsModel = units->modelElement();

        if (unitsModel != model) {
            // Only copy units which haven't already been copied

            std::wstring unitsName = units->name();
            QPair<QString, QString> unitsPair(QString::fromStdWString(unitsName),
                                              QString::fromStdWString(unitsModel->name()));

            if (!mCopiedUnits.contains(unitsPair)) {
                // Keep track of the units pair

                mCopiedUnits << unitsPair;

                // Create a new units

                ObjRef<iface::cellml_api::Units> newUnits = model->createUnits();

                newUnits->name(unitsName);

                bool newUnitsIsBaseUnits = units->isBaseUnits();

                if (newUnitsIsBaseUnits)
                    newUnits->isBaseUnits(newUnitsIsBaseUnits);

                // Copy the units' unit references to the new units

                ObjRef<iface::cellml_api::UnitSet> unitSet = units->unitCollection();

                if (unitSet->length()) {
                    ObjRef<iface::cellml_api::UnitIterator> unitIterator = unitSet->iterateUnits();

                    forever {
                        ObjRef<iface::cellml_api::Unit> unit = unitIterator->nextUnit();

                        if (!unit)
                            break;

                        // Create a new unit reference

                        ObjRef<iface::cellml_api::Unit> newUnit = model->createUnit();

                        newUnit->prefix(unit->prefix());
                        newUnit->multiplier(unit->multiplier());
                        newUnit->offset(unit->offset());
                        newUnit->exponent(unit->exponent());
                        newUnit->units(unit->units());

                        // Add the unit reference to the new units

                        newUnits->addElement(newUnit);
                    }
                }

                // Add the new units to the element

                pElement->addElement(newUnits);
            }
        }
    }
}

//==============================================================================

iface::cellml_api::CellMLComponent * CellmlFileCellml10Exporter::findRealComponent(iface::cellml_api::CellMLComponent *pComponent)
{
    // Find the 'real' component for the given component
    // Note #1: we use CeVAS's algorithm for this, i.e. if the component is an
    //          ImportComponent, then we return the template CellMLComponent
    //          that it is based on in an imported model, otherwise we return
    //          the given component. If no 'real' component exists, then we
    //          return 0...
    // Note #2: we require that a component is not imported more than once in
    //          the same import element, as then multiple components share one
    //          real component...

    pComponent->add_ref();

    forever {
        ObjRef<iface::cellml_api::ImportComponent> importComponent = QueryInterface(pComponent);

        if (!importComponent)
            // We are dealing with a non-import component

            break;

        pComponent->release_ref();

        ObjRef<iface::cellml_api::CellMLElement> importComponentParentElement = importComponent->parentElement();
        ObjRef<iface::cellml_api::CellMLImport> import = QueryInterface(importComponentParentElement);
        ObjRef<iface::cellml_api::Model> model = import->importedModel();
        ObjRef<iface::cellml_api::CellMLComponentSet> components = model->modelComponents();

        pComponent = components->getComponent(importComponent->componentRef());

        if (!pComponent)
            // No 'real' component exists

            break;
    }

    return pComponent;
}

//==============================================================================

void CellmlFileCellml10Exporter::annotateImportedComponents(iface::cellml_api::Model *pModel)
{
    // Recursively annotate all imported components with the name they are given
    // by the importing model

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();

    if (imports->length()) {
        // The model has some imports, so go through them

        ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

        forever {
            ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();

            if (!import)
                break;

            // We have an import, so recursively annotate the components
            // imported by the given model
            // Note: depth first processing means that imported components
            //       renamed twice end up with the right name...

            ObjRef<iface::cellml_api::Model> importedModel = import->importedModel();

            annotateImportedComponents(importedModel);

            // Annotate the imported components

            ObjRef<iface::cellml_api::ImportComponentSet> importComponents = import->components();

            if (importComponents->length()) {
                // The import includes some imported components, so go through
                // them and annotate them with the name they are given by the
                // importing model

                ObjRef<iface::cellml_api::ImportComponentIterator> importComponentsIterator = importComponents->iterateImportComponents();

                forever {
                    ObjRef<iface::cellml_api::ImportComponent> importComponent = importComponentsIterator->nextImportComponent();

                    if (!importComponent)
                        break;

                    // We have an imported component, so annotate it

                    ObjRef<iface::cellml_api::CellMLComponent> realComponent = findRealComponent(importComponent);

                    mAnnotationSet->setStringAnnotation(realComponent, L"renamed", importComponent->name());
                }
            }
        }
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::ensureComponentNameUniqueness(std::wstring &pComponentName)
{
    // Make sure that the component name is unique within our exported model
    // Note: if the given component name is already used, then we append the
    //       string "_n" to it, where n is the least natural number giving an
    //       unused name...

    QString componentName = QString::fromStdWString(pComponentName);
    QString suffix = QString();
    unsigned n = 0;

    while (mComponentNames.contains(componentName+suffix))
        // The current component name is already used, so generate a new suffix

        suffix = QString("_%1").arg(++n);

    // We have got a valid suffix for our component name, so update the given
    // component name and keep track of it

    componentName += suffix;

    mComponentNames << componentName;

    pComponentName = componentName.toStdWString();
}

//==============================================================================

iface::dom::Element * CellmlFileCellml10Exporter::copyDomElement(iface::dom::Element *pDomElement)
{
    Q_UNUSED(pDomElement);

    // Create and return a (manual) deep copy of the given DOM element
    // Note #1: if the DOM element is in the MathML namespace, then it will be
    //          converted to an iface::cellml_api::MathMLElement instance prior
    //          to being returned...
    // Note #2: it appears that when adding a child element or text node, which
    //          we are subsequently going to release_ref() ourselves, then we
    //          must also release_ref() the parent, or its refcount never
    //          reaches zero...

    std::wstring namespaceUri = pDomElement->namespaceURI();

    // Create a blank DOM element

    iface::dom::Element *res = mExportedModel->createExtensionElement(namespaceUri,
                                                                      pDomElement->nodeName());

    // Copy the DOM element attributes

    ObjRef<iface::dom::Document> document = res->ownerDocument();
    ObjRef<iface::dom::NamedNodeMap> attributes = pDomElement->attributes();

    for (uint32_t i = 0, iMax = attributes->length(); i < iMax; ++i) {
        ObjRef<iface::dom::Attr> attr = QueryInterface(attributes->item(i));
        iface::dom::Attr *newAttr = document->createAttributeNS(CELLML_1_0_NAMESPACE, attr->name());

        newAttr->value(attr->value());

        ObjRef<iface::dom::Attr> dummyAttr = res->setAttributeNodeNS(newAttr);

        Q_UNUSED(dummyAttr);
    }

    // Copy the child element and text nodes

    ObjRef<iface::dom::NodeList> childNodes = pDomElement->childNodes();

    for (uint32_t i = 0, iMax = childNodes->length(); i < iMax; ++i) {
        ObjRef<iface::dom::Node> childNode = childNodes->item(i);

        switch (childNode->nodeType()) {
        case iface::dom::Node::ELEMENT_NODE: {
            ObjRef<iface::dom::Element> elementNode = QueryInterface(childNode);
            ObjRef<iface::dom::Element> newElementNode = copyDomElement(elementNode);

            res->appendChild(newElementNode)->release_ref();

            break;
        }
        case iface::dom::Node::TEXT_NODE: {
            ObjRef<iface::dom::Text> newTextNode = document->createTextNode(childNode->nodeValue());

            res->appendChild(newTextNode)->release_ref();

            break;
        }
        default:
            // Not something we care about, so do nothing...

            ;
        }
    }

    // Cast the DOM element to iface::cellml_api::MathMLElement, if it is in the
    // MathML namespace

    if (namespaceUri == MATHML_NAMESPACE) {
        ObjRef<iface::mathml_dom::MathMLElement> realRes = QueryInterface(res);

        res = realRes;
    }

    // We are all done, so return our copied DOM element

    return res;
}

//==============================================================================

void CellmlFileCellml10Exporter::copyExtensionElements(iface::cellml_api::CellMLElement *pFromElement,
                                                       iface::cellml_api::CellMLElement *pToElement)
{
    // Copy any extension elements

    ObjRef<iface::cellml_api::ExtensionElementList> elements = pFromElement->extensionElements();

    for (uint32_t i = 0, iMax = elements->length(); i < iMax; ++i) {
        ObjRef<iface::dom::Element> element = elements->getAt(i);

        // Check that it's not a MathML element

        if (element->namespaceURI() == MATHML_NAMESPACE)
            continue;

        ObjRef<iface::dom::Element> newElement = copyDomElement(element);

        pToElement->appendExtensionElement(newElement);
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyComponent(iface::cellml_api::CellMLComponent *pComponent)
{
    // Copy the given component into our exported model
    // Note: we create a new component, and manually copy the contents of the
    //       given component into it...

    // Make sure that the component hasn't already been copied

    ObjRef<iface::cellml_api::CellMLComponent> newComponent = QueryInterface(mAnnotationSet->getObjectAnnotation(pComponent, L"copy"));

    if (newComponent)
        return;

    // Create a new component and make sure that the original component knows
    // about its existence

    newComponent = mExportedModel->createComponent();

    mAnnotationSet->setObjectAnnotation(pComponent, L"copy", newComponent);

    // Check whether the component has been renamed

    std::wstring newComponentName = pComponent->name();
    std::wstring componentRenamedName = mAnnotationSet->getStringAnnotation(pComponent, L"renamed");

    if (componentRenamedName.length())
        // The component was imported, so it may have been renamed

        newComponentName = componentRenamedName;

    // Make sure that the name of our new component is unique within our
    // exported model

    ensureComponentNameUniqueness(newComponentName);

    // Set both the name and cmeta:id, if any, of our new component

    newComponent->name(newComponentName);

    std::wstring newComponentCmetaId = pComponent->cmetaId();

    if (newComponentCmetaId.length())
        newComponent->cmetaId(newComponentCmetaId);

    // Copy the units to our new component

    ObjRef<iface::cellml_api::UnitsSet> componentUnitsSet = pComponent->units();

    copyUnitsSet(componentUnitsSet, newComponent);

    // Copy the variables to our new component, if any

    ObjRef<iface::cellml_api::CellMLVariableSet> componentVariables = pComponent->variables();

    if (componentVariables->length()) {
        ObjRef<iface::cellml_api::CellMLVariableIterator> componentVariablesIterator = componentVariables->iterateVariables();

        forever {
            ObjRef<iface::cellml_api::CellMLVariable> componentVariable = componentVariablesIterator->nextVariable();

            if (!componentVariable)
                break;

            // We have a variable to copy, so create a new variable and copy its
            // attributes from the original variable

            ObjRef<iface::cellml_api::CellMLVariable> newComponentVariable = mExportedModel->createCellMLVariable();

            newComponentVariable->name(componentVariable->name());

            std::wstring componentVariableCmetaId = componentVariable->cmetaId();

            if (componentVariableCmetaId.length())
                newComponentVariable->cmetaId(componentVariableCmetaId);

            newComponentVariable->initialValue(componentVariable->initialValue());
            newComponentVariable->unitsName(componentVariable->unitsName());
            newComponentVariable->publicInterface(componentVariable->publicInterface());
            newComponentVariable->privateInterface(componentVariable->privateInterface());

            // Copy our new variable to our new component

            newComponent->addElement(newComponentVariable);
        }
    }

    // Copy the math nodes to our new component, if any

    ObjRef<iface::cellml_api::MathList> componentMath = pComponent->math();

    if (componentMath->length()) {
        ObjRef<iface::cellml_api::MathMLElementIterator> componentMathIterator = componentMath->iterate();

        forever {
            ObjRef<iface::mathml_dom::MathMLElement> componentMathNode = componentMathIterator->next();

            if (!componentMathNode)
                break;

            // We have a math node, so copy a clone of it into our new component

            iface::cellml_api::MathMLElement newComponentMathNode = dynamic_cast<iface::cellml_api::MathMLElement>(copyDomElement(componentMathNode));

            newComponent->addMath(newComponentMathNode);

            newComponentMathNode->release_ref();
            // Note: since we want the new component to be its owner...
        }
    }

    // Copy the extension elements to our new component

    copyExtensionElements(pComponent, newComponent);

    // Add the new component to our exported model

    mExportedModel->addElement(newComponent);
}

//==============================================================================

void CellmlFileCellml10Exporter::copyComponents(iface::cellml_services::CeVAS *pCevas)
{
    // Copy the relevant components into our exported model, using the given
    // CeVAS to find components to copy

    ObjRef<iface::cellml_api::CellMLComponentIterator> componentsIterator = pCevas->iterateRelevantComponents();

    forever {
        ObjRef<iface::cellml_api::CellMLComponent> component = componentsIterator->nextComponent();

        if (!component)
            break;

        // We have a component, so copy it

        copyComponent(component);
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyGroup(iface::cellml_api::Model *pModel,
                                           iface::cellml_api::ComponentRefSet *pComponentReferences,
                                           iface::cellml_api::ComponentRef *pToComponentReference)
{
    if (!pComponentReferences->length())
        return;

    // Iterate this level of the tree

    ObjRef<iface::cellml_api::CellMLComponentSet> components = pModel->modelComponents();
    ObjRef<iface::cellml_api::ComponentRefIterator> componentReferencesIterator = pComponentReferences->iterateComponentRefs();

    forever {
        ObjRef<iface::cellml_api::ComponentRef> componentReference = componentReferencesIterator->nextComponentRef();

        if (!componentReference)
            break;

        // We have a component reference, so find the referenced component

        ObjRef<iface::cellml_api::CellMLComponent> referencedComponent = components->getComponent(componentReference->componentName());

        if (!referencedComponent)
            continue;

        // Find the real component object

        ObjRef<iface::cellml_api::CellMLComponent> realComponent = findRealComponent(referencedComponent);

        // Has the real component been copied?

        ObjRef<iface::cellml_api::CellMLComponent> copiedComponent = QueryInterface(mAnnotationSet->getObjectAnnotation(realComponent, L"copy"));

        if (!copiedComponent)
            continue;

        // Create a component reference for the copied component

        ObjRef<iface::cellml_api::ComponentRef> newComponentReference = mExportedModel->createComponentRef();

        newComponentReference->componentName(copiedComponent->name());

        if (pToComponentReference) {
            // Add our new component reference to the given group

            pToComponentReference->addElement(newComponentReference);
        } else {
            // Create a new group

            ObjRef<iface::cellml_api::Group> group = mExportedModel->createGroup();

            mExportedModel->addElement(group);

            ObjRef<iface::cellml_api::RelationshipRef> relationshipReference = mExportedModel->createRelationshipRef();

            relationshipReference->setRelationshipName(L"", L"encapsulation");

            group->addElement(relationshipReference);

            // Add our new component reference as the root

            group->addElement(newComponentReference);
        }

        // Copy any children of our component reference

        ObjRef<iface::cellml_api::ComponentRefSet> componentReferences = componentReference->componentRefs();

        copyGroup(pModel, componentReferences, newComponentReference);
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyGroups(iface::cellml_api::Model *pModel)
{
    // Iterate only groups defining the encapsulation hierarchy and copy them
    // into our exported model, if any

    ObjRef<iface::cellml_api::GroupSet> groups = pModel->groups();

    if (groups->length()) {
        ObjRef<iface::cellml_api::GroupIterator> groupsIterator = groups->iterateGroups();

        forever {
            ObjRef<iface::cellml_api::Group> group = groupsIterator->nextGroup();

            if (!group)
                break;

            // Copy the encapsulation group into our exported model

            ObjRef<iface::cellml_api::ComponentRefSet> groupComponentReferences = group->componentRefs();

            copyGroup(pModel, groupComponentReferences);
        }
    }

    // Recursively process imported models, if any

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();

    if (imports->length()) {
        ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

        forever {
            ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();

            if (!import)
                break;

            // Process the imported model

            ObjRef<iface::cellml_api::Model> importedModel = import->importedModel();

            copyGroups(importedModel);
        }
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyConnection(iface::cellml_api::Connection *pConnection)
{
    // Copy a connection, possibly involving imported components, into our
    // exported model
    // Note: the connection will only be copied if both components involved have
    //       been copied across first, and hence have a "copy" annotation...

    ObjRef<iface::cellml_api::MapComponents> componentMapping = pConnection->componentMapping();
    ObjRef<iface::cellml_api::CellMLComponent> firstComponent = componentMapping->firstComponent();
    ObjRef<iface::cellml_api::CellMLComponent> secondComponent = componentMapping->secondComponent();

    // Check that we have copied the components involved, and get the copies

    ObjRef<iface::cellml_api::CellMLComponent> newFirstComponent = QueryInterface(mAnnotationSet->getObjectAnnotation(firstComponent, L"copy"));

    if (!newFirstComponent)
        return;

    ObjRef<iface::cellml_api::CellMLComponent> newSecondComponent = QueryInterface(mAnnotationSet->getObjectAnnotation(secondComponent, L"copy"));

    if (!newSecondComponent)
        return;

    // Create a new connection

    ObjRef<iface::cellml_api::Connection> newConnection = mExportedModel->createConnection();

    mExportedModel->addElement(newConnection);

    // Create a new component mapping

    ObjRef<iface::cellml_api::MapComponents> newComponentMapping = newConnection->componentMapping();

    newComponentMapping->firstComponentName(newFirstComponent->name());
    newComponentMapping->secondComponentName(newSecondComponent->name());

    // Add the variable mappings

    ObjRef<iface::cellml_api::MapVariablesSet> variableMappings = pConnection->variableMappings();

    if (variableMappings->length()) {
        ObjRef<iface::cellml_api::MapVariablesIterator> variableMappingsIterator = variableMappings->iterateMapVariables();

        forever {
            ObjRef<iface::cellml_api::MapVariables> variableMapping = variableMappingsIterator->nextMapVariables();

            if (!variableMapping)
                break;

            ObjRef<iface::cellml_api::MapVariables> newVariableMapping = mExportedModel->createMapVariables();

            newVariableMapping->firstVariableName(variableMapping->firstVariableName());
            newVariableMapping->secondVariableName(variableMapping->secondVariableName());

            newConnection->addElement(newVariableMapping);
        }
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::copyConnections(iface::cellml_api::Model *pModel)
{
    // Copy local connections into our exported model, if any

    ObjRef<iface::cellml_api::ConnectionSet> connections = pModel->connections();

    if (connections->length()) {
        ObjRef<iface::cellml_api::ConnectionIterator> connectionsIterator = connections->iterateConnections();

        forever {
            ObjRef<iface::cellml_api::Connection> connection = connectionsIterator->nextConnection();

            if (!connection)
                break;

            // Copy the local connection into our exported model

            copyConnection(connection);
        }
    }

    // Recursively process imported models, if any

    ObjRef<iface::cellml_api::CellMLImportSet> imports = pModel->imports();

    if (imports->length()) {
        ObjRef<iface::cellml_api::CellMLImportIterator> importsIterator = imports->iterateImports();

        forever {
            ObjRef<iface::cellml_api::CellMLImport> import = importsIterator->nextImport();

            if (!import)
                break;

            // Process the imported model

            ObjRef<iface::cellml_api::Model> importedModel = import->importedModel();

            copyConnections(importedModel);
        }
    }
}

//==============================================================================

void CellmlFileCellml10Exporter::propagateInitialValues()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
