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
    case Parameters:
        return QObject::tr("Parameters");
    }

    return QString();
}

//==============================================================================

QColor PrettyCellmlViewLexer::defaultColor(int pStyle) const
{
    // Return the given style's default colour

    switch (pStyle) {
    case Default:
        return QColor(0x00, 0x00, 0x00);
    case Comment:
        return QColor(0x00, 0x7f, 0x00);
    case Keyword:
        return QColor(0x7f, 0x7f, 0x00);
    case Parameters:
        return QColor(0x00, 0x00, 0x7f);
    }

    return QsciLexerCustom::defaultColor(pStyle);
}

//==============================================================================

QFont PrettyCellmlViewLexer::defaultFont(int pStyle) const
{
    // Return the given style's default colour

    QFont res = QsciLexer::defaultFont(pStyle);

    switch (pStyle) {
    case Parameters:
        res.setItalic(true);

        break;
    }

    return res;
}

//==============================================================================

void PrettyCellmlViewLexer::styleText(int pStart, int pEnd)
{
    // Make sure that we have an editor

    if (!editor())
        return;

    // Retrieve the text to style

    char *data = new char[pEnd-pStart+1];

    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, pStart, pEnd, data);

    QString text = QString(data);

    delete[] data;

    if (text.isEmpty())
        return;

    // Retrieve the various lines that make up the text

    QScintillaSupport::QScintillaWidget *currentEditor = qobject_cast<QScintillaSupport::QScintillaWidget *>(editor());
    int eolStringSize = currentEditor->eolString().size();

    QStringList lines = text.split(currentEditor->eolString());

    // Style our various lines

    int lineShift = 0;

    foreach (QString line, lines) {
        // Check whether our line contains a comment

        int commentPosition = line.indexOf("//");
        int lineLength = line.length();
        int commentLength = 0;

        if (commentPosition != -1) {
            // We have a comment, so style it and remove it from our line

            commentLength = line.length()-commentPosition;

            startStyling(pStart+lineShift+commentPosition, 0x1f);
            setStyling(commentLength, Comment);

            line.chop(commentLength);

            lineLength -= commentLength;
        }

        // Style the rest of our line using default styling

        startStyling(pStart+lineShift, 0x1f);
        setStyling(line.length(), Default);

        // Check whether the rest of our line contains some keywords and, if so,
        // style them

//---GRY--- TO BE DONE...

        // Check whether some of our line contains some parameters and, if so,
        // style them

        int istart = -1;

        for (int i = 0, imax = line.length(); i < imax; ++i) {
            if ((line[i] == '{') && (istart == -1)) {
                istart = i;
            } else if ((line[i] == '}') && (istart != -1)) {
                startStyling(pStart+lineShift+istart, 0x1f);
                setStyling(i-istart+1, Parameters);

                istart = -1;
            }
        }

        lineShift += lineLength+commentLength+eolStringSize;
    }
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
