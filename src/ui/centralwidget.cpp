#include "centralwidget.h"
#include "tabwidget.h"

#include "ui_centralwidget.h"

#include <QFileInfo>
#include <QTextStream>

#include <qsciscintilla.h>
#include <qscilexerxml.h>

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget)
{
    // Set up the UI

    mUi->setupUi(this);

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
