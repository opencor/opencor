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
#pragma once
#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QObject>
#include <QPushButton>

#include "thirdparty/json.hpp"

#include <bgelementeditorscene.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

/**
 * The BrowseLineEdit class provides a special line editor for the parameterGUI.
 * While editing file- or directory names it is much more easier to have a
 * file-dialog and just click on existing files or directories. This editor
 * provides a simple QLineEditor and a browse-button which opens a file- or a
 * directory dialog. Clicking on existing files or directories copies the path
 * to the line editor. Depending on the <tt>BrowseType</tt> given in the
 * constructor the browse button opens a <tt>file</tt> or a <tt>directory</tt>
 * dialog.
 */
class BrowseLineEdit : public QFrame
{
    Q_OBJECT

public:
    BrowseLineEdit(QWidget *parent = nullptr,
                   BGElementEditorScene *scene = nullptr);

    /**
   * Reimplemented from the QWidget class.
   * Returns the size of the editor.
   */
    QSize sizeHint() const override;

    /**
   * Reimplemented from the QWidget class.
   */
    QSize minimumSizeHint() const override;

    /**
   * Returns the json.
   */
    nlohmann::json json() const;

    /**
   * @brief Set the name of the object who's state/parameter is being modified
   *
   * @param parentName
   */
    void setParentName(QString parentName);

    void setAsState(bool state);

public Q_SLOTS:

    void setJson(nlohmann::json &str);

Q_SIGNALS:

    void editingFinished();

private Q_SLOTS:

    void editing_finished();

    /**
   * This function opens a parameter import dialog
   */
    void browse();

private:
    /**
   * The line editor.
   */
    QLineEdit *line_editor;

    /**
   * The browse button.
   */
    QPushButton *browse_button;

    // Scene related to the delegate
    BGElementEditorScene *m_scene;

    // Parent Element name
    QString m_parentName;

    // True if the entry is state - no symbolic assignments
    bool m_isState;

    /*
   * JSON struct based on import dialog spec
   */
    nlohmann::json m_json;
    /*
   * Any JSON data used as reference
   */
    nlohmann::json m_reference;
};

/**
 * The ParameterDelegate class implements special delegates for the QTreeWidget
 * class used in the parameterGUI. The QTreeWidget class provides some different
 * standard delegates for editing parameters shown in the tree structure. The
 * ParameterDelegate class provides special editors for the different types of
 * parameters defined in the ParameterHandler class. For all parameter types
 * based on strings as &quot;Anything&quot;, &quot;MultipleSelection&quot;
 * &quot;Map&quot; and &quot;List&quot; a simple line editor will be shown up.
 * In the case of integer and double type parameters the editor is a spin box
 * and for &quot;Selection&quot; type parameters a combo box will be shown up.
 * For parameters of type &quot;FileName&quot; and &quot;DirectoryName&quot; the
 * delegate shows a @ref BrowseLineEdit editor. The column of the tree structure
 * with the parameter values has to be set in the constructor.
 *
 */
class ParameterDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ParameterDelegate(QObject *parent = nullptr,
                      BGElementEditorScene *scene = nullptr);

    /**
   * @brief Set the Element Name of the object that is being modified
   *
   * @param eName
   */
    void setElementName(QString eName);

    /**
   * @brief Allow the SI units to be changed when mutate is True
   *
   * @param mutate
   */
    void setDimensionAsMutable(bool mutate);

    /**
   * This function creates the appropriate editor for the parameter
   * based on the <tt>index</tt>.
   */
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    /**
   * Reimplemented from QItemDelegate.
   */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    /**
   * Reimplemented from QItemDelegate.
   */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    /**
   * Reimplemented from QItemDelegate.
   */
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /**
   * Reimplemented from QItemDelegate.
   */
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
Q_SIGNALS:
    void invalidUserInput(QString err) const;

private Q_SLOTS:
    /**
   * Reimplemented from QItemDelegate.
   */
    void commit_and_close_editor();

private:
    BGElementEditorScene *m_scene;
    QString m_ElementName;
    bool m_AllowDimensionChange;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR

