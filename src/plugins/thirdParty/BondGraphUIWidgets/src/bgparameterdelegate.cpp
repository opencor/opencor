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
#include "bgparameterdelegate.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QValidator>
#include <limits>

#include "bgparameterimportdialog.h"
#include "bondgraph.hpp"
/**
 * Following diagnostic pragmas have been added to integrate with opencor.
 * OpenCOR fails to build on these warnings (by design)
 */

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(push)
    #pragma warning(disable: 4244)
    #pragma warning(disable: 4267)
    #pragma warning(disable: 4996)
#elif defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#endif



namespace OpenCOR {
namespace BondGraphUI {

BrowseLineEdit::BrowseLineEdit(QWidget *parent, BGElementEditorScene *scene)
    : QFrame(parent, 0), m_scene(scene), m_parentName("") {
  line_editor = new QLineEdit;
  connect(line_editor, SIGNAL(editingFinished()), this,
          SLOT(editing_finished()));

  browse_button = new QPushButton("&Browse...");
  connect(browse_button, SIGNAL(clicked()), this, SLOT(browse()));

  setFocusPolicy(Qt::StrongFocus);

  QHBoxLayout *layout = new QHBoxLayout;

  layout->setContentsMargins(1, 1, 1, 1);
  layout->addWidget(line_editor);
  layout->addWidget(browse_button);
  setLayout(layout);

  setAutoFillBackground(true);
  setBackgroundRole(QPalette::Highlight);
}

QSize BrowseLineEdit::sizeHint() const {
  QSize size_line_editor = line_editor->sizeHint(),
        size_browse_button = browse_button->sizeHint();

  int w = size_line_editor.rwidth() + size_browse_button.rwidth(),
      h = qMax(size_line_editor.rheight(), size_browse_button.rheight());

  return QSize(w, h);
}

QSize BrowseLineEdit::minimumSizeHint() const {
  QSize size_line_editor = line_editor->minimumSizeHint(),
        size_browse_button = browse_button->minimumSizeHint();

  int w = size_line_editor.rwidth() + size_browse_button.rwidth(),
      h = qMax(size_line_editor.rheight(), size_browse_button.rheight());

  return QSize(w, h);
}

nlohmann::json BrowseLineEdit::json() const { return m_json; }

void BrowseLineEdit::setAsState(bool state) { m_isState = state; }

void BrowseLineEdit::setJson(nlohmann::json &str) {
  try {
    // The input may contain reference information, for instance to check the
    // dimensions of the selected cellml variable to be consistent with the
    // chosen one this is stored in the record field, the value is stored in the
    // value field The value object it self may have another value object and
    // this layer needs to be removed
    if (str.contains("record")) {
      m_reference = str["record"];
      setAsState(m_reference["isState"]);
      if (!str["value"].is_object())
        m_json["value"] = str["value"];
      else
        m_json = str["value"];
    } else {
      m_reference.clear();
      m_json = str;
    }
    if (m_json["value"].is_number()) {
      line_editor->setText(QString::number(m_json["value"].get<double>()));
    } else {
      line_editor->setText(QString::fromStdString(m_json["value"]));
    }
  } catch (nlohmann::json::exception &ex) {
    qDebug() << Q_FUNC_INFO << "\nFailed to process record "
             << QString::fromStdString(str.dump());
  }
}

void BrowseLineEdit::setParentName(QString parentName) {
  m_parentName = parentName;
}

void BrowseLineEdit::editing_finished() {
  QString val = line_editor->text();
  bool numeric = false;
  double dv = val.toDouble(&numeric);
  if (numeric) {
    // Remove any other decorations
    nlohmann::json j;
    j["value"] = dv;
    m_json = j;
  } else {
    m_json["value"] = val.toStdString();
  }
  Q_EMIT editingFinished();
}

void BrowseLineEdit::browse() {
  nlohmann::json res;
  if (m_reference.contains("dimension")) {
    res = BGParameterImport::getParameterRecordWithDimensionalConsistency(
        m_json, m_reference["dimension"], m_parentName, m_isState, m_scene);
  } else
    res = BGParameterImport::getParameterRecord(m_json, m_parentName, m_isState,
                                                m_scene);
  // Update the json as well as what is displayed
  bool updated = false;
  blockSignals(true);
  if (res.size() > 0) {
    line_editor->setText(QString::fromStdString(res["value"]));
    m_json = res;
    updated = true;
  }
  blockSignals(false);
  if (updated)
    editing_finished();
}

ParameterDelegate::ParameterDelegate(QObject *parent,
                                     BGElementEditorScene *scene)
    : QItemDelegate(parent), m_scene(scene), m_ElementName(""),
      m_AllowDimensionChange(false) {}

QSize ParameterDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {
  return QItemDelegate::sizeHint(option, index);
}

void ParameterDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {

  // load pattern description stored in the StatusLine
  QString pattern_description = index.data(Qt::StatusTipRole).toString();

  QRegExp rx_string("\\b(FileName)\\b");

  // if the type is Filename open parameter spec dialog
  if (rx_string.indexIn(pattern_description) != -1) {
    QString value = index.model()->data(index, Qt::DisplayRole).toString();

    QStyleOptionViewItem my_option = option;
    my_option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    // print the text in the display
    drawDisplay(painter, my_option, my_option.rect, value);
    // if the line has the focus, print a rectangle
    drawFocus(painter, my_option, my_option.rect);
  } else {
    // for all other types use the standard delegate
    QItemDelegate::paint(painter, option, index);
  }
}

QWidget *ParameterDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &,
                                         const QModelIndex &index) const {
  // load pattern description stored in the StatusLine
  QString pattern_description = index.data(Qt::StatusTipRole).toString();

  QRegExp rx_string("\\b(Anything)\\b"), rx_list("\\b(List)\\b"),
      rx_filename("\\b(FileName)\\b"), rx_unit("\\b(Unit)\\b"),
      rx_integer("\\b(Integer)\\b"), rx_double("\\b(Double|Float|Floating)\\b"),
      rx_selection("\\b(Selection)\\b"), rx_bool("\\b(Bool)\\b");

  // if the type is "Anything" choose a LineEditor
  if (rx_string.indexIn(pattern_description) != -1) {
    QLineEdit *line_editor = new QLineEdit(parent);
    connect(line_editor, SIGNAL(editingFinished()), this,
            SLOT(commit_and_close_editor()));

    return line_editor;
  } else if (rx_list.indexIn(pattern_description) != -1) {
    // if the type is "List" of files , the use BrowseLineEditor
    if (rx_filename.indexIn(pattern_description) != -1) {
      BrowseLineEdit *parameter_editor = new BrowseLineEdit(parent, m_scene);
      parameter_editor->setParentName(m_ElementName);
      connect(parameter_editor, SIGNAL(editingFinished()), this,
              SLOT(commit_and_close_editor()));

      return parameter_editor;
    }
    // if the type is "List" of something else choose a LineEditor
    else {
      QLineEdit *line_editor = new QLineEdit(parent);

      connect(line_editor, SIGNAL(editingFinished()), this,
              SLOT(commit_and_close_editor()));

      return line_editor;
    }
  }
  // if the type is "FileName" choose a BrowseLineEditor
  else if (rx_filename.indexIn(pattern_description) != -1) {
    BrowseLineEdit *parameter_editor = new BrowseLineEdit(parent, m_scene);
    parameter_editor->setParentName(m_ElementName);
    connect(parameter_editor, SIGNAL(editingFinished()), this,
            SLOT(commit_and_close_editor()));

    return parameter_editor;
  }
  // if the type is "UNIT" choose a LineEdit with unit validation
  else if (rx_unit.indexIn(pattern_description) != -1) {
    QLineEdit *line_edit = new QLineEdit(parent);

    connect(line_edit, SIGNAL(editingFinished()), this,
            SLOT(commit_and_close_editor()));

    return line_edit;
  }
  // if the type is "Integer" choose a LineEditor with appropriate bounds
  else if (rx_integer.indexIn(pattern_description) != -1) {
    const QStringList default_pattern =
        pattern_description.split(" ").filter("...");
    const QStringList default_values = default_pattern[0].split("...");

    QLineEdit *line_edit = new QLineEdit(parent);
    line_edit->setValidator(new QIntValidator(
        default_values[0].toInt(), default_values[1].toInt(), line_edit));

    connect(line_edit, SIGNAL(editingFinished()), this,
            SLOT(commit_and_close_editor()));

    return line_edit;
  }
  // if the type is "Double" choose a LineEditor with appropriate bounds
  else if (rx_double.indexIn(pattern_description) != -1) {
    const QStringList default_pattern =
        pattern_description.split(" ").filter("...");
    QStringList default_values = default_pattern[0].split("...");

    // Unfortunately conversion of MAX_DOUBLE to string and back fails
    // sometimes, therefore use MAX_DOUBLE/2 to make sure we are below.
    // In practice MAX_DOUBLE just means VERY large, it is normally not
    // important how large.
    const double max_double = std::numeric_limits<double>::max() / 2;
    default_values = default_values.replaceInStrings(
        "MAX_DOUBLE", QVariant(max_double).toString());

    const unsigned int number_of_decimals = 14;

    QLineEdit *line_edit = new QLineEdit(parent);
    line_edit->setValidator(new QDoubleValidator(
        default_values[0].toDouble(), default_values[1].toDouble(),
        number_of_decimals, line_edit));

    connect(line_edit, SIGNAL(editingFinished()), this,
            SLOT(commit_and_close_editor()));

    return line_edit;
  }
  // if the type is "Selection" choose a ComboBox
  else if (rx_selection.indexIn(pattern_description) != -1) {
    QComboBox *combo_box = new QComboBox(parent);

    // we assume, that pattern_description is of the form
    // "Type: [Selection item1|item2| ....|item ]".
    // Find the first space after the first '[',
    // which indicates the start of the first option
    int begin_pattern = pattern_description.indexOf("[");
    begin_pattern = pattern_description.indexOf(" ", begin_pattern) + 1;

    // Find the last ']', which signals the end of the options
    const int end_pattern = pattern_description.lastIndexOf("]");

    // Extract the options from the string
    QString pattern =
        pattern_description.mid(begin_pattern, end_pattern - begin_pattern);

    // Remove trailing whitespaces
    while (pattern.endsWith(' '))
      pattern.chop(1);

    // Split the list
    const QStringList choices = pattern.split("|");
    combo_box->addItems(choices);

    combo_box->setEditable(false);

    connect(combo_box, SIGNAL(currentIndexChanged(int)), this,
            SLOT(commit_and_close_editor()));

    return combo_box;
  }
  // if the type is "Bool" choose a ComboBox
  else if (rx_bool.indexIn(pattern_description) != -1) {
    QComboBox *combo_box = new QComboBox(parent);

    std::vector<std::string> choices;
    choices.push_back(std::string("true"));
    choices.push_back(std::string("false"));

    // add items to the combo box
    for (size_t i = 0; i < choices.size(); ++i)
      combo_box->addItem(tr(choices[i].c_str()), tr(choices[i].c_str()));

    combo_box->setEditable(false);

    connect(combo_box, SIGNAL(currentIndexChanged(int)), this,
            SLOT(commit_and_close_editor()));

    return combo_box;
  };

  // if it is not the column "parameter values", do nothing
  return 0;
}

void ParameterDelegate::setElementName(QString eName) { m_ElementName = eName; }

void ParameterDelegate::setDimensionAsMutable(bool mutate) {
  m_AllowDimensionChange = mutate;
}

void ParameterDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const {

  if (BrowseLineEdit *parameter_editor =
          qobject_cast<BrowseLineEdit *>(editor)) {
    QString json_s = index.data(Qt::UserRole).toString();

    nlohmann::json j;
    nlohmann::json j1;
    try {
      j = nlohmann::json::parse(json_s.toStdString());
      // Parameter Editor expects an object with "value" key
      if (!j.is_object()) {
        j1["value"] = j;
        j = j1;
      }
    } catch (nlohmann::json::parse_error &ex) {
      Q_UNUSED(ex);
      j["value"] = json_s.toStdString();
    }

    parameter_editor->setJson(j);
  } else if (QComboBox *combo_box = qobject_cast<QComboBox *>(editor)) {
    QRegExp rx(index.data(Qt::DisplayRole).toString());

    // Preset ComboBox to the current selection
    for (int i = 0; i < combo_box->count(); ++i)
      if (rx.exactMatch(combo_box->itemText(i)))
        combo_box->setCurrentIndex(i);
  } else
    QItemDelegate::setEditorData(editor, index);
}

void ParameterDelegate::commit_and_close_editor() {
  QWidget *editor = qobject_cast<QWidget *>(sender());
  Q_EMIT commitData(editor);
  Q_EMIT closeEditor(editor);
}

void ParameterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const {
  if (QLineEdit *line_edit = qobject_cast<QLineEdit *>(editor)) {
    QString line_edit_content = line_edit->text();
    if (line_edit_content.size() == 0)
      return;
    int position = 0;
    // If the editor has a validator, only accept the content if it contains
    // correct input. Otherwise bad cases
    // can happen, when some unfinished number violates the bounds, and
    // the user clicks elsewhere to lose the focus of the editor.
    if (line_edit->validator()) {
      auto vres = line_edit->validator()->validate(line_edit_content, position);
      if (vres == QValidator::Acceptable)
        model->setData(index, line_edit_content);
      return;
    }

    if (!m_AllowDimensionChange) {
      QString pattern_description = index.data(Qt::StatusTipRole).toString();
      QRegExp rx_unit("\\b(Unit)\\b");
      if (rx_unit.indexIn(pattern_description) != -1) {
        QString targetUnit = index.data(Qt::DisplayRole).toString();
        auto res = BG::checkUnits(line_edit_content.toStdString(),
                                  targetUnit.toStdString());
        if (res.size() != 0) {
          auto msh = QString::fromStdString(res);
          auto tres = BG::checkUnits(targetUnit.toStdString(), "");
          auto tmsh = QString::fromStdString(tres);
          // Cannot qMessage from non ui thread, so send a signal
          Q_EMIT invalidUserInput(tr("Inconsistent or invalid units specified. "
                                     "Expected (Base SI) ") +
                                  targetUnit + " ( " + tmsh + " ) " +
                                  tr("given") + " " + line_edit_content +
                                  " ( " + msh +
                                  " ) \n Reverted to previous value!");
          return;
        }
      }
    }

    model->setData(index, line_edit_content);
  } else if (BrowseLineEdit *parameter_editor =
                 qobject_cast<BrowseLineEdit *>(editor)) {
    auto value = parameter_editor->json();
    model->setData(index, QString::fromStdString(value.dump()), Qt::UserRole);
  } else if (QComboBox *combo_box = qobject_cast<QComboBox *>(editor)) {
    QString value = combo_box->currentText();
    model->setData(index, value);
  }
}
} // namespace BondGraphUI
} // namespace OpenCOR