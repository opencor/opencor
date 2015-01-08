/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Lexer for the pretty CellML format
//==============================================================================

#include "prettycellmlviewlexer.h"
#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellmlViewLexer::PrettyCellmlViewLexer(QObject *pParent) :
    QsciLexerCustom(pParent)
{
    // Some initialisations

    mKeywords = QStringList() << "as" << "comp" << "def" << "model" << "unit";
}

//==============================================================================

const char * PrettyCellmlViewLexer::language() const
{
    // Return the language for our lexer

    return "Pretty CellML";
}

//==============================================================================

QString PrettyCellmlViewLexer::description(int pStyle) const
{
    // Return the given style's description

    switch (pStyle) {
    case Default:
        return QObject::tr("Default");
    case Comment:
        return QObject::tr("Comment");
    case Keyword:
        return QObject::tr("Keyword");
    }

    return QString();
}

//==============================================================================

QColor PrettyCellmlViewLexer::defaultColor(int pStyle) const
{
    // Return the given style's default colour

    switch (pStyle) {
    case Default:
        return QColor(0x80, 0x80, 0x80);
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    case Keyword:
        return QColor(0x00, 0x00, 0x7f);
    }

    return QsciLexerCustom::defaultColor(pStyle);
}

//==============================================================================

void PrettyCellmlViewLexer::styleText(int pStart, int pEnd)
{
    // Make sure that we have an editor

    if (!editor())
        return;

    // Retrieve the test to style

    char *data = new char[pEnd-pStart+1];

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, pStart, pEnd, data);

    QString text = QString(data);

    delete[] data;

    if (text.isEmpty())
        return;

    startStyling(pStart, 0x1f);

    QStringList lines = text.split(qobject_cast<QScintillaSupport::QScintillaWidget *>(editor())->eolString());

    foreach (QString line, lines) {
        int lineLength = line.length();

        line = line.trimmed();

        if (line.startsWith("//"))
            setStyling(lineLength, Comment);
        else
            setStyling(lineLength, Default);
    }
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
