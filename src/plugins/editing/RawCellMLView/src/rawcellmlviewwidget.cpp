//==============================================================================
// Raw CellML view widget
//==============================================================================

#include "borderedwidget.h"
#include "coreutils.h"
#include "qscintilla.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QSettings>
#include <QSplitter>
#include <QTextStream>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

RawCellmlViewWidget::RawCellmlViewWidget(QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mGui(new Ui::RawCellmlViewWidget),
    mBorderedEditor(0),
    mBorderedEditors(QMap<QString, Core::BorderedWidget *>()),
    mBorderedViewerHeight(0),
    mBorderedEditorHeight(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Keep track of our sizes when moving the splitter

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterMoved()));

    // Create our viewer

    mViewer = new Viewer::ViewerWidget(this);
    mBorderedViewer = new Core::BorderedWidget(mViewer,
                                               false, false, true, false);

    // Add the bordered viewer to ourselves

    addWidget(mBorderedViewer);
}

//==============================================================================

RawCellmlViewWidget::~RawCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

static const QString SettingsViewerHeight = "ViewerHeight";
static const QString SettingsEditorHeight = "EditorHeight";

//==============================================================================

void RawCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the viewer's and editor's height
    // Note #1: the viewer's default height is 19% of the desktop's height while
    //          that of the editor is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our raw CellML view widget, the viewer's default height will
    //          effectively be less than 19% of the desktop's height, but that
    //          doesn't matter at all...

    mBorderedViewerHeight = pSettings->value(SettingsViewerHeight,
                                             0.19*qApp->desktop()->screenGeometry().height()).toInt();
    mBorderedEditorHeight = pSettings->value(SettingsEditorHeight,
                                             qApp->desktop()->screenGeometry().height()).toInt();
}

//==============================================================================

void RawCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the viewer's and editor's height
    // Note #1: we must also keep track of the editor's height because when
    //          loading our settings (see above), the widget doesn't yet have a
    //          'proper' height, so we couldn't simply assume that the editor's
    //          initial height is this widget's height minus the viewer's
    //          initial height, so...
    // Note #2: we rely on mBorderedViewerHeight and mBorderedEditorHeight
    //          rather than directly calling the height() method of the viewer
    //          and of the editor, respectively since it may happen that the
    //          user exits OpenCOR without ever having switched to the raw
    //          CellML view, in which case we couldn't retrieve the viewer and
    //          editor's height which in turn would result in OpenCOR crashing,
    //          so...

    pSettings->setValue(SettingsViewerHeight, mBorderedViewerHeight);
    pSettings->setValue(SettingsEditorHeight, mBorderedEditorHeight);
}

//==============================================================================

void RawCellmlViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the size of our viewer and current editor, and hide the editor

    bool needInitialSizes = !mBorderedEditor;

    int borderedViewerHeight = 0;
    int borderedEditorHeight = 0;

    if (mBorderedEditor) {
        // An editor is currently available, so retrieve the size of both our
        // viewer and the current editor

        borderedViewerHeight = mBorderedViewer->height();
        borderedEditorHeight = mBorderedEditor->height();

        // Hide the current editor

        mBorderedEditor->hide();
    }

    // Retrieve the editor associated with the file name, if any

    mBorderedEditor = mBorderedEditors.value(pFileName);

    if (!mBorderedEditor) {
        // No editor exists for the file name, so create and set up a Scintilla
        // editor with an XML lexer associated with it

        QFile file(pFileName);
        QString fileContents = QString();
        bool fileIsWritable = false;

        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            // We could open the file, so retrieve its contents and whether it
            // can be written to

            fileContents = QTextStream(&file).readAll();
            fileIsWritable = !(QFileInfo(pFileName).isWritable());

            // We are done with the file, so close it

            file.close();
        }

        mBorderedEditor = new Core::BorderedWidget(new QScintillaSupport::QScintilla(fileContents,
                                                                                     fileIsWritable,
                                                                                     new QsciLexerXML(this),
                                                                                     parentWidget()),
                                                   true, false, false, false);

        // Keep track of our bordered editor and add it to ourselves

        mBorderedEditors.insert(pFileName, mBorderedEditor);

        addWidget(mBorderedEditor);
    }

    // Make sure that 'new' bordered editor is visible

    mBorderedEditor->show();

    // Set the raw CellML view widget's focus proxy to the 'new' editor

    setFocusProxy(mBorderedEditor->widget());

    // Adjust our sizes

    if (needInitialSizes) {
        // We need to initialise our sizes, so...

        setSizes(QList<int>() << mBorderedViewerHeight
                              << mBorderedEditorHeight);
    } else {
        // Our sizes have already been initialised, so set our sizes so that
        // our 'new' editor gets its size set to that of the 'old' editor

        QList<int> newSizes = QList<int>() << borderedViewerHeight;

        for (int i = 1, iMax = count(); i < iMax; ++i)
            if (static_cast<Core::BorderedWidget *>(widget(i)) == mBorderedEditor)
                // This is the editor we are after, so...

                newSizes << borderedEditorHeight;
            else
                // Not the editor we are after, so...

                newSizes << 0;

        setSizes(newSizes);
    }
}

//==============================================================================

bool RawCellmlViewWidget::isManaged(const QString &pFileName) const
{
    // Return whether the given file name is managed

    return mBorderedEditors.value(pFileName);
}

//==============================================================================

void RawCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the bordered editor, should there be one for the given file name

    Core::BorderedWidget *borderedEditor  = mBorderedEditors.value(pFileName);

    if (borderedEditor) {
        // There is a bordered editor for the given file name, so delete it and
        // remove it from our list

        delete borderedEditor;

        mBorderedEditors.remove(pFileName);

        // Reset our memory of who is the current bordered editor

        mBorderedEditor = 0;
    }
}

//==============================================================================

void RawCellmlViewWidget::splitterMoved()
{
    // The splitter has moved, so keep track of the viewer and editor's new
    // height

    mBorderedViewerHeight = mBorderedViewer->height();
    mBorderedEditorHeight = mBorderedEditor->height();
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
