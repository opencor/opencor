#include "centralwidget.h"
#include "fileorganiserwidget.h"
#include "tabwidget.h"

#include "ui_centralwidget.h"

#include <QDragEnterEvent>
#include <QFileInfo>
#include <QTextStream>
#include <QUrl>

#include <qsciscintilla.h>
#include <qscilexerxml.h>

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create and add our tab widget

    mTabWidget = new TabWidget(this);

    mUi->verticalLayout->addWidget(mTabWidget);
}

CentralWidget::~CentralWidget()
{
    // Delete the UI

    delete mUi;
}

void CentralWidget::openFile(const QString &pFileName)
{
    // Create an editor for the file

    QsciScintilla *scintilla = new QsciScintilla(this);
    QsciLexerXML *lexer      = new QsciLexerXML;

    scintilla->setLexer(lexer);
    scintilla->setFolding(QsciScintilla::BoxedTreeFoldStyle);

    // Set the contents of the file to the editor

    QFile file(pFileName);

    file.open(QIODevice::ReadOnly);

    scintilla->setText(QTextStream(&file).readAll());
    scintilla->setReadOnly(!(QFile::permissions(pFileName) & QFile::WriteUser));

    file.close();

    // Create a new tab and have the editor as its contents

    mTabWidget->setCurrentIndex(mTabWidget->addTab(scintilla,
                                                   QFileInfo(pFileName).baseName()));
}

void CentralWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URIs or items from our file organiser

    if (   (   (pEvent->mimeData()->hasFormat("text/uri-list"))
            || (pEvent->mimeData()->hasFormat(FileOrganiserMimeType)))
        && (!pEvent->mimeData()->urls().isEmpty()))
        // Note: we test the list of URL in case we are trying to drop one or
        //       several folders (and no file) from the file organiser

        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

void CentralWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

void CentralWidget::dropEvent(QDropEvent *pEvent)
{
    // Retrieve the name of the various files that have been dropped

    QList<QUrl> urlList = pEvent->mimeData()->urls();
    QStringList fileNames;

    for (int i = 0; i < urlList.count(); ++i)
    {
        QString fileName = urlList.at(i).toLocalFile();
        QFileInfo fileInfo = fileName;

        if (fileInfo.isFile()) {
            if (fileInfo.isSymLink()) {
                // We are dropping a symbolic link, so retrieve its target and
                // check that it exists, and if it does then add it

                fileName = fileInfo.symLinkTarget();

                if (QFileInfo(fileName).exists())
                    fileNames.append(fileName);
            } else {
                // We are dropping a file, so we can just add it

                fileNames.append(fileName);
            }
        }
    }

    // fileNames may contain duplicates (in case we dropped some symbolic
    // links), so remove them

    fileNames.removeDuplicates();

    // Let people know that some files have been dropped on us

    emit filesDropped(fileNames);

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}
