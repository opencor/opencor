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
// Simulation Experiment view preferences widget
//==============================================================================

#pragma once

//==============================================================================

#include "graphpanelwidget.h"
#include "preferencesinterface.h"

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_symbol.h"
#include "qwtend.h"

//==============================================================================

namespace Ui {
    class SimulationExperimentViewPreferencesWidget;
} // namespace Ui

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
    class PropertyEditorWidget;
    class TabBarWidget;
} // namespace Core

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

static const auto PluginName = QStringLiteral("SimulationExperimentView");

//==============================================================================

static const auto SettingsPreferencesGraphPanelBackgroundColor = QStringLiteral("GraphPanelBackgroundColor");
static const auto SettingsPreferencesGraphPanelForegroundColor = QStringLiteral("GraphPanelForegroundColor");
static const auto SettingsPreferencesGraphPanelLegend          = QStringLiteral("GraphPanelLegend");

//==============================================================================

static const QColor SettingsPreferencesGraphPanelBackgroundColorDefault = GraphPanelWidget::DefaultGraphPanelBackgroundColor;
static const QColor SettingsPreferencesGraphPanelForegroundColorDefault = GraphPanelWidget::DefaultGraphPanelForegroundColor;
static const bool SettingsPreferencesGraphPanelLegendDefault            = GraphPanelWidget::DefaultGraphPanelLegend;

//==============================================================================

static const auto SettingsPreferencesGraphLineStyle       = QStringLiteral("GraphLineStyle");
static const auto SettingsPreferencesGraphLineWidth       = QStringLiteral("GraphLineWidth");
static const auto SettingsPreferencesGraphSymbolStyle     = QStringLiteral("GraphSymbolStyle");
static const auto SettingsPreferencesGraphSymbolSize      = QStringLiteral("GraphSymbolSize");
static const auto SettingsPreferencesGraphSymbolFilled    = QStringLiteral("GraphSymbolFilled");
static const auto SettingsPreferencesGraphSymbolFillColor = QStringLiteral("GraphSymbolFillColor");

//==============================================================================

static const Qt::PenStyle SettingsPreferencesGraphLineStyleDefault       = GraphPanelWidget::DefaultGraphLineStyle;
static const int SettingsPreferencesGraphLineWidthDefault                = GraphPanelWidget::DefaultGraphLineWidth;
static const QwtSymbol::Style SettingsPreferencesGraphSymbolStyleDefault = GraphPanelWidget::DefaultGraphSymbolStyle;
static const int SettingsPreferencesGraphSymbolSizeDefault               = GraphPanelWidget::DefaultGraphSymbolSize;
static const bool SettingsPreferencesGraphSymbolFilledDefault            = GraphPanelWidget::DefaultGraphSymbolFilled;
static const QColor SettingsPreferencesGraphSymbolFillColorDefault       = GraphPanelWidget::DefaultGraphSymbolFillColor;

//==============================================================================

class SimulationExperimentViewPreferencesWidget : public Preferences::PreferencesWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewPreferencesWidget(QWidget *pParent);
    ~SimulationExperimentViewPreferencesWidget() override;

    bool preferencesChanged() const override;

    void resetPreferences() override;
    void savePreferences() override;

private:
    Ui::SimulationExperimentViewPreferencesWidget *mGui;

    Core::TabBarWidget *mCategoryTabs;

    Core::PropertyEditorWidget *mGraphPanelProperties;
    Core::PropertyEditorWidget *mGraphProperties;

    QMap<int, Core::BorderedWidget *> mPropertyEditors;

    QColor mGraphPanelBackgroundColor;
    QColor mGraphPanelForegroundColor;
    bool mGraphPanelLegend;

    Qt::PenStyle mGraphLineStyle;
    int mGraphLineWidth;
    QwtSymbol::Style mGraphSymbolStyle;
    int mGraphSymbolSize;
    bool mGraphSymbolFilled;
    QColor mGraphSymbolFillColor;

private slots:
    void updateGui();

    void headerSectionResized(int pIndex, int pOldSize, int pNewSize);
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
