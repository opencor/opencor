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
    Widget(pParent),
    mUi(new Ui::RawCellmlViewWidget),
    mEditors(QMap<QString, Core::BorderedWidget *>()),
    mEditor(0),
    mViewerHeight(0),
    mEditorHeight(0)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create our vertical splitter

    mVerticalSplitter = new QSplitter(Qt::Vertical, this);

    connect(mVerticalSplitter, SIGNAL(splitterMoved(int,int)),
            this, SLOT(verticalSplitterMoved()));

    // Create a bordered viewer (the border is only here for aesthetical
    // reasons)

    mViewer = new Core::BorderedWidget(new Viewer::ViewerWidget(this),
                                       false, false, true, false);

    // Populate our vertical splitter and add it to our raw CellML view widget

    mVerticalSplitter->addWidget(mViewer);

    mUi->verticalLayout->addWidget(mVerticalSplitter);
}

//==============================================================================

RawCellmlViewWidget::~RawCellmlViewWidget()
{
    // Delete the UI

    delete mUi;
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

    mViewerHeight = pSettings->value(SettingsViewerHeight,
                                     0.19*qApp->desktop()->screenGeometry().height()).toInt();
    mEditorHeight = pSettings->value(SettingsEditorHeight,
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
    // Note #2: we rely on mViewerHeight and mEditorHeight rather than directly
    //          calling the height() method of the viewer and of the editor,
    //          respectively since it may happen that the user exits OpenCOR
    //          without ever having switched to the raw CellML view, in which
    //          case we couldn't retrieve the viewer and editor's height which
    //          in turn would result in OpenCOR crashing, so...

    pSettings->setValue(SettingsViewerHeight, mViewerHeight);
    pSettings->setValue(SettingsEditorHeight, mEditorHeight);
}

//==============================================================================

void RawCellmlViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the size of our viewer and current editor, and hide the editor

    bool needInitialSizes = !mEditor;

    int viewerHeight = 0;
    int editorHeight = 0;

    if (mEditor) {
        // An editor is currently available, so retrieve the size of both our
        // viewer and the current editor

        viewerHeight = mViewer->height();
        editorHeight = mEditor->height();

        // Hide the current editor

        mEditor->hide();
    }

    // Retrieve the editor associated with the file name, if any

    mEditor = mEditors.value(pFileName);

    if (!mEditor) {
        // No editor exists for the file name, so create and set up a Scintilla
        // editor with an XML lexer associated to it

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

        mEditor = new Core::BorderedWidget(new QScintillaSupport::QScintilla(this, fileContents, fileIsWritable,
                                                                             new QsciLexerXML(this)),
                                           true, false, false, false);

        // Keep track of the editor and add it to our vertical splitter

        mEditors.insert(pFileName, mEditor);

        mVerticalSplitter->addWidget(mEditor);
    }

    // Make sure that the 'new' editor is visible

    mEditor->show();

    // Set the raw CellML view widget's focus proxy to the 'new' editor

    setFocusProxy(mEditor->widget());

    // Adjust our vertical splitter's sizes

    if (needInitialSizes) {
        // We need to initialise our vertical splitter's sizes, so...

        mVerticalSplitter->setSizes(QList<int>() << mViewerHeight
                                                 << mEditorHeight);
    } else {
        // Our vertical splitter's sizes have already been initialised, so set
        // its sizes so that our 'new' editor gets its size set to that of the
        // 'old' editor

        QList<int> newSizes = QList<int>() << viewerHeight;

        for (int i = 1, iMax = mVerticalSplitter->count(); i < iMax; ++i)
            if (static_cast<Core::BorderedWidget *>(mVerticalSplitter->widget(i)) == mEditor)
                // This is the editor we are after, so...

                newSizes << editorHeight;
            else
                // Not the editor we are after, so...

                newSizes << 0;

        mVerticalSplitter->setSizes(newSizes);
    }
}

//==============================================================================

void RawCellmlViewWidget::verticalSplitterMoved()
{
    // The splitter has moved, so keep track of the viewer and editor's new
    // height

    mViewerHeight = mViewer->height();
    mEditorHeight = mEditor->height();
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
