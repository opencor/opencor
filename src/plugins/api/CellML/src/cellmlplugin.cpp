#include "cellmlplugin.h"
#include "coreutils.h"

#include "cellml-api-cxx-support.hpp"
#include "CellMLBootstrap.hpp"
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceCIS.hxx"
#include "CISBootstrap.hpp"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QUrl>

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use the <a href=\"http://www.cellml.org/tools/api/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour utiliser l'<a href=\"http://www.cellml.org/tools/api/\">API CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::Api,
                      false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

class TestProgressObserver : public iface::cellml_services::IntegrationProgressObserver
{
public:
    TestProgressObserver(iface::cellml_services::CellMLCompiledModel *pCompiledModel) :
        mCompiledModel(pCompiledModel),
        mCodeInformation(pCompiledModel->codeInformation()),
        mRefCount(1),
        mFinished(false)
    {
        // Determine the header for the results

        iface::cellml_services::ComputationTargetIterator *cti = mCodeInformation->iterateTargets();

        bool first = true;

        QString header = QString();

        while (true) {
            iface::cellml_services::ComputationTarget* ct = cti->nextComputationTarget();

            if (!ct)
                break;

            if (     ((ct->type() == iface::cellml_services::STATE_VARIABLE)
                   || (ct->type() == iface::cellml_services::ALGEBRAIC)
                   || (ct->type() == iface::cellml_services::VARIABLE_OF_INTEGRATION))
                && (ct->degree() == 0)) {
                if (!first)
                    header += ",";
                else
                    first = false;

                header += QString::fromWCharArray(ct->variable()->name());
            }
        }

        // Output the header to our results file

        mResultsFile.setFileName(QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");

        mResultsFile.remove();   // Just in case...

        mResultsFile.open( QIODevice::WriteOnly);

        writeResults(header);
    }

    virtual void add_ref() throw(std::exception &)
    {
        ++mRefCount;
    }

    virtual void release_ref() throw(std::exception &)
    {
        --mRefCount;

        if (!mRefCount)
            delete this;
    }

    virtual char * objid() throw (std::exception &)
    {
        return strdup("singletonTestProgressObserver");
    }

    virtual void * query_interface(const char *pIface) throw (std::exception &)
    {
        if (!strcmp(pIface, "xpcom::IObject"))
            return static_cast<iface::XPCOM::IObject *>(this);
        else if (!strcmp(pIface, "cellml_services::IntegrationProgressObserver"))
            return static_cast<iface::cellml_services::IntegrationProgressObserver *> (this);

        return 0;
    }

    virtual void computedConstants(uint32_t, double *) throw (std::exception &)
    {
        // Do nothing...
    }

    virtual void results(uint32_t pNbOfValues, double *pValues) throw (std::exception&)
    {
        uint32_t rateIndexCount = mCodeInformation->rateIndexCount();
        uint32_t recSize =  2*rateIndexCount
                           +mCodeInformation->algebraicIndexCount()
                           +1;

        if (!recSize)
            return;

        for (uint32_t i = 0; i < pNbOfValues; i += recSize)
        {
            bool first = true;

            iface::cellml_services::ComputationTargetIterator *targetsIter = mCodeInformation->iterateTargets();

            QString values = QString();

            while (true)
            {
                iface::cellml_services::ComputationTarget *target = targetsIter->nextComputationTarget();

                if (!target)
                    break;

                if (target->degree())
                    continue;

                uint32_t varOff = 0;

                switch (target->type()) {
                case iface::cellml_services::STATE_VARIABLE:
                    varOff = 1+target->assignedIndex();

                    break;
                case iface::cellml_services::VARIABLE_OF_INTEGRATION:
                    varOff = 0;

                    break;
                case iface::cellml_services::ALGEBRAIC:
                    varOff = 1+2*rateIndexCount+target->assignedIndex();

                    break;
                default:
                    continue;
                }

                if (!first)
                    values += ",";
                else
                    first = false;

                values += QString::number(pValues[i+varOff]);
            }

            writeResults(values);
        }
    }

    virtual void done() throw (std::exception &)
    {
        mFinished = true;
    }

    virtual void failed(const char *pErrorMsg) throw (std::exception &)
    {
        qDebug("CIS error: Model integration failed (%s)", pErrorMsg);

        mFinished = true;
    }

    bool finished()
    {
        return mFinished;
    }

    void writeResults(const QString &pResults)
    {
        mResultsFile.write((pResults+"\n").toLatin1().constData());
        mResultsFile.flush();
    }

private:
    iface::cellml_services::CellMLCompiledModel *mCompiledModel;
    iface::cellml_services::CodeInformation *mCodeInformation;

    uint32_t mRefCount;
    bool mFinished;

    QFile mResultsFile;
};

void CellMLPlugin::initialize()
{
    // Fetch a bootstrap object

    iface::cellml_api::CellMLBootstrap *cbs = CreateCellMLBootstrap();

    // Retrieve the model loader

    iface::cellml_api::DOMModelLoader *ml = cbs->modelLoader();

    // Load our test CellML model
    // Note: we do this within a try...catch statement since we might get an
    //       exception...

    QString testCellmlModelFileName = QDir::tempPath()+QDir::separator()+"test_cellml_model.cellml";

    Core::saveResourceAs(":test_cellml_model", testCellmlModelFileName);

    iface::cellml_api::Model *model;

    try {
        model = ml->loadFromURL(QUrl::fromLocalFile(testCellmlModelFileName).toString().toStdWString().c_str());
    } catch (iface::cellml_api::CellMLException &) {
        qDebug("Model loader error: %s", QString::fromWCharArray(ml->lastErrorMessage()).toLatin1().constData());

        usePrecomputedTestCellmlResults();

        return;
    }

    // Run the model

    iface::cellml_services::CellMLIntegrationService* cis = CreateIntegrationService();
    iface::cellml_services::ODESolverCompiledModel* compiledModel = 0;

    try
    {
        compiledModel = cis->compileModelODE(model);
    } catch (iface::cellml_api::CellMLException &) {
        qDebug("CIS error: %s", QString::fromWCharArray(cis->lastError()).toLatin1().constData());

        usePrecomputedTestCellmlResults();

        return;
    } catch (...) {
        qDebug("CIS error: Unexpected exception calling compileModel.");

        usePrecomputedTestCellmlResults();

        return;
    }

    iface::cellml_services::ODESolverRun *odeSolverRun = cis->createODEIntegrationRun(compiledModel);

    TestProgressObserver* progressObserver = new TestProgressObserver(compiledModel);

    odeSolverRun->setProgressObserver(progressObserver);

    odeSolverRun->stepType(iface::cellml_services::BDF_IMPLICIT_1_5_SOLVE);
    odeSolverRun->setResultRange(0, 50, 1000);

    odeSolverRun->start();

    while (!progressObserver->finished())
        SleeperThread::msleep(1);

    QFile::remove(testCellmlModelFileName);
}

QList<FileType> CellMLPlugin::fileTypes() const
{
    // Return the CellML file type that the CellML plugin supports

    return QList<FileType>() << FileType(qobject_cast<FileInterface *>(this),
                                         CellmlMimeType, "cellml");
}

QString CellMLPlugin::fileTypeDescription(const QString &mMimeType) const
{
    // Return the description for the requested Mime type, that is as long as it
    // is for the CellML Mime type

    if (!mMimeType.compare(CellmlMimeType))
        return tr("CellML File");
    else
        // Not a Mime type that we can recognise, so...

        return FileInterface::fileTypeDescription(mMimeType);
}

void CellMLPlugin::usePrecomputedTestCellmlResults()
{
    // Use the pre-computed test CellML results
    // Note: this is done as a result of not being able to use the CellML API
    //       binaries (most likely because we couldn't compile our test CellML
    //       file)

    Core::saveResourceAs(":test_cellml_results",
                         QDir::tempPath()+QDir::separator()+"test_cellml_results.csv");
}

} }
