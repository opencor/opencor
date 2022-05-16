/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// MathML Viewer widget
//==============================================================================

#pragma once

//==============================================================================

#include "mathmlviewerwidgetglobal.h"
#include "widget.h"

//==============================================================================

#include <QDomNode>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_mml_document.h"
#include "qwtend.h"

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace MathMLViewerWidget {

//==============================================================================

class MATHMLVIEWERWIDGET_EXPORT MathmlViewerWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit MathmlViewerWidget(QWidget *pParent);

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void retranslateUi() override;

    void updateSettings(MathmlViewerWidget *pMathmlViewerWidget);

    QString contents() const;
    void setContents(const QString &pContents);

    bool error() const;
    void setError(bool pError);

    bool optimizeFontSize() const;
    void setOptimizeFontSize(bool pOptimizeFontSize);

    bool subscripts() const;
    void setSubscripts(bool pSubscripts);

    bool greekSymbols() const;
    void setGreekSymbols(bool pGreekSymbols);

    bool digitGrouping() const;
    void setDigitGrouping(bool pDigitGrouping);

protected:
    void contextMenuEvent(QContextMenuEvent *pEvent) override;
    void paintEvent(QPaintEvent *pEvent) override;

private:
    QMap<QString, QString> mGreekSymbols = {
                                               { "ALPHA", "Α" },
                                               { "BETA", "Β" },
                                               { "GAMMA", "Γ" },
                                               { "DELTA", "Δ" },
                                               { "EPSILON", "Ε" },
                                               { "ZETA", "Ζ" },
                                               { "ETA", "Η" },
                                               { "THETA", "Θ" },
                                               { "IOTA", "Ι" },
                                               { "KAPPA", "Κ" },
                                               { "LAMBDA", "Λ" },
                                               { "MU", "Μ" },
                                               { "NU", "Ν" },
                                               { "XI", "Ξ" },
                                               { "OMICRON", "Ο" },
                                               { "PI", "Π" },
                                               { "RHO", "Ρ" },
                                               { "SIGMA", "Σ" },
                                               { "TAU", "Τ" },
                                               { "UPSILON", "Υ" },
                                               { "PHI", "Φ" },
                                               { "CHI", "Χ" },
                                               { "PSI", "Ψ" },
                                               { "OMEGA", "Ω" },

                                               { "alpha", "α" },
                                               { "beta", "β" },
                                               { "gamma", "γ" },
                                               { "delta", "δ" },
                                               { "epsilon", "ε" },
                                               { "zeta", "ζ" },
                                               { "eta", "η" },
                                               { "theta", "θ" },
                                               { "iota", "ι" },
                                               { "kappa", "κ" },
                                               { "lambda", "λ" },
                                               { "mu", "μ" },
                                               { "nu", "ν" },
                                               { "xi", "ξ" },
                                               { "omicron", "ο" },
                                               { "pi", "π" },
                                               { "rho", "ρ" },
                                               { "sigma", "σ" },
                                               { "tau", "τ" },
                                               { "upsilon", "υ" },
                                               { "phi", "φ" },
                                               { "chi", "χ" },
                                               { "psi", "ψ" },
                                               { "omega", "ω" }
                                           };

    QwtMathMLDocument mMathmlDocument;

    double mOneOverMathmlDocumentWidth = 0.0;
    double mOneOverMathmlDocumentHeight = 0.0;

    QString mContents;
    bool mError = false;

    QMenu *mContextMenu;

    QAction *mOptimizeFontSizeAction;
    QAction *mSubscriptsAction;
    QAction *mGreekSymbolsAction;
    QAction *mDigitGroupingAction;
    QAction *mCopyToClipboardAction;

    QAction * newAction();

    QString greekSymbol(const QString &pValue) const;

    QDomElement newMiNode(const QDomNode &pDomNode,
                          const QString &pValue) const;

    void processNode(const QDomNode &pDomNode) const;
    QString processedContents() const;

private slots:
    void updateMathmlViewerWidget();

    void copyToClipboard();
};

//==============================================================================

} // namespace MathMLViewerWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
