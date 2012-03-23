//==============================================================================
// Raw CellML view widget
//==============================================================================

#include "qscintilla.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
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
    mEditors(QMap<QString, QScintillaSupport::QScintilla *>()),
    mEditor(0),
    mViewerInitialHeight(0),
    mEditorInitialHeight(0)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create our vertical splitter

    mVerticalSplitter = new QSplitter(Qt::Vertical, this);

    // Create a viewer

    mViewer = new Viewer::ViewerWidget(pParent);

    // Populate our vertical splitter and add it to our raw CellML view widget

    mVerticalSplitter->addWidget(mViewer);

    mUi->verticalLayout->addWidget(mVerticalSplitter);
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
    // Note #2: because the editor's default height is much bigger than our raw
    //          CellML view widget's height, the viewer's default height will
    //          effectively be less than 19% of the desktop's height, but that
    //          doesn't matter at all...

    mViewerInitialHeight = pSettings->value(SettingsViewerHeight,
                                            0.19*qApp->desktop()->screenGeometry().height()).toInt();
    mEditorInitialHeight = pSettings->value(SettingsEditorHeight,
                                            qApp->desktop()->screenGeometry().height()).toInt();
}

//==============================================================================

void RawCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the viewer's and editor's height
    // Note: we must also keep track of the editor's height because when loading
    //       our raw CellML view widget's settings (see above), the widget
    //       doesn't yet have a 'proper' height, so we couldn't simply assume
    //       that the editor's initial height is widget's height minus the
    //       viewer's initial height, so...

    pSettings->setValue(SettingsViewerHeight, mViewer->height());
    pSettings->setValue(SettingsEditorHeight, mEditor->height());
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

        mEditor = new QScintillaSupport::QScintilla(fileContents, fileIsWritable,
                                                    new QsciLexerXML(parent()),
                                                    qobject_cast<QWidget *>(parent()));

        // Keep track of the editor and add it to our vertical splitter

        mEditors.insert(pFileName, mEditor);

        mVerticalSplitter->addWidget(mEditor);
    }

    // Make sure that the 'new' editor is visible

    mEditor->show();

    // Adjust our vertical splitter's sizes

    if (needInitialSizes) {
        // We need to initialise our vertical splitter's sizes, so...

        mVerticalSplitter->setSizes(QList<int>() << mViewerInitialHeight
                                                 << mEditorInitialHeight);
    } else {
        // Our vertical splitter's sizes have already been initialised, so set
        // its sizes so that our 'new' editor gets its size set to that of the
        // 'old' editor

        QList<int> newSizes = QList<int>() << viewerHeight;

        for (int i = 1, iMax = mVerticalSplitter->count(); i < iMax; ++i)
            if (dynamic_cast<QScintillaSupport::QScintilla *>(mVerticalSplitter->widget(i)) == mEditor)
                // This the editor we are after, so...

                newSizes << editorHeight;
            else
                // Not the editor we are after, so...

                newSizes << 0;

        mVerticalSplitter->setSizes(newSizes);
    }
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
