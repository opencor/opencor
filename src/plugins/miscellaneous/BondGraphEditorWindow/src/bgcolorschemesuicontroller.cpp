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
#include "bgcolorschemesuicontroller.h"
#include <bgeditorscene.h>
/**
 * Following diagnostic pragmas have been added to integrate with opencor.
 * OpenCOR fails to build on these warnings (by design)
 */
#if defined(_MSC_VER) && !defined(__clang__)
#    pragma warning(push)
#    pragma warning(disable : 4244)
#    pragma warning(disable : 4267)
#    pragma warning(disable : 4996)
#elif defined(__GNUC__) && !defined(__clang__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#else
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wmissing-field-initializers"
#    pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGColorSchemesUIController::BGColorSchemesUIController(QObject *parent) :
    QObject(parent)
{
    {
        Scheme scheme { tr("All Blacks") };
        scheme.bgColor = QColor("#D1F6CA");
        scheme.gridColor = Qt::lightGray;
        scheme.nodeColor = Qt::black;
        scheme.nodeStrokeColor = Qt::white;
        scheme.nodeLabelColor = Qt::white;
        scheme.edgeColor = Qt::white;
        scheme.edgeLabelColor = Qt::white;

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Grayscale") };
        scheme.bgColor = Qt::white;
        scheme.gridColor = Qt::gray;
        scheme.nodeColor = Qt::lightGray;
        scheme.nodeStrokeColor = Qt::black;
        scheme.nodeLabelColor = Qt::black;
        scheme.edgeColor = Qt::darkGray;
        scheme.edgeLabelColor = Qt::gray;

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Inverse Grayscale") };
        scheme.bgColor = Qt::black;
        scheme.gridColor = Qt::darkGray;
        scheme.nodeColor = Qt::darkGray;
        scheme.nodeStrokeColor = Qt::white;
        scheme.nodeLabelColor = Qt::white;
        scheme.edgeColor = Qt::gray;
        scheme.edgeLabelColor = Qt::lightGray;

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Solarized Light") };
        scheme.bgColor = QColor("#fdf6e3");
        scheme.gridColor = QColor("#eee8d5");
        scheme.nodeColor = QColor("#e0dbcb");
        scheme.nodeStrokeColor = QColor("#073642");
        scheme.nodeLabelColor = QColor("#657b83");
        scheme.edgeColor = QColor("#556058");
        scheme.edgeLabelColor = QColor("#808000");

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Blue && Orange") };
        scheme.bgColor = QColor("#ffffff");
        scheme.gridColor = QColor("#eeeeee");
        scheme.nodeColor = QColor("#55aaff");
        scheme.nodeStrokeColor = QColor("#ffffff");
        scheme.nodeLabelColor = QColor("#444444");
        scheme.edgeColor = QColor("#ffaa00");
        scheme.edgeLabelColor = QColor("#55aa7f");

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Forest") };
        scheme.bgColor = QColor("#e3e6bb");
        scheme.gridColor = QColor("#eeeeee");
        scheme.nodeColor = QColor("#aaff7f");
        scheme.nodeStrokeColor = QColor("#8d4600");
        scheme.nodeLabelColor = QColor("#343400");
        scheme.edgeColor = QColor("#aaaa7f");
        scheme.edgeLabelColor = QColor("#55aa00");

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Sunny Spring") };
        scheme.bgColor = QColor("#f3ffe1");
        scheme.gridColor = QColor("#eeeeee");
        scheme.nodeColor = QColor("#b4ba00");
        scheme.nodeStrokeColor = QColor("#b4ba00");
        scheme.nodeLabelColor = QColor("#111111");
        scheme.edgeColor = QColor("#ba4400");
        scheme.edgeLabelColor = QColor("#267536");

        addScheme(scheme);
    }

    {
        Scheme scheme { tr("Night Sky") };
        scheme.bgColor = QColor("#000640");
        scheme.gridColor = QColor("#070f5a");
        scheme.nodeColor = QColor("#000000");
        scheme.nodeStrokeColor = QColor("#6f73c0");
        scheme.nodeLabelColor = QColor("#dcdcdc");
        scheme.edgeColor = QColor("#6f73c0");
        scheme.edgeLabelColor = QColor("#aad6ff");

        addScheme(scheme);
    }

    connect(&m_menu, SIGNAL(triggered(QAction *)), this,
            SLOT(onMenuTriggered(QAction *)));
}

void BGColorSchemesUIController::setScene(BGEditorScene *scene)
{
        m_scene = scene;
}

QMenu *BGColorSchemesUIController::getSchemesMenu()
{
    return &m_menu;
}


void BGColorSchemesUIController::onMenuTriggered(QAction *action)
{
    int index = action->data().toInt();
    applyScheme(m_schemes.at(index));
}

void BGColorSchemesUIController::addScheme(const Scheme &scheme)
{
    auto index = m_schemes.size();
    m_schemes << scheme;
    auto action = m_menu.addAction(scheme.name);
    action->setData(index);
}

void BGColorSchemesUIController::applyScheme(const Scheme &scheme)
{
    if (m_scene) {
        m_scene->setBackgroundBrush(scheme.bgColor);
        m_scene->setGridPen(scheme.gridColor);
        m_scene->setClassAttribute("element", "color", scheme.nodeColor);
        m_scene->setClassAttribute("element", "stroke.color",
                                   scheme.nodeStrokeColor);
        m_scene->setClassAttribute("element", "label.color", scheme.nodeLabelColor);
        m_scene->setClassAttribute("connection", "color", scheme.edgeColor);
        m_scene->setClassAttribute("connection", "label.color",
                                   scheme.edgeLabelColor);

        m_scene->addUndoState();

        Q_EMIT colorSchemeApplied(m_scene);
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
