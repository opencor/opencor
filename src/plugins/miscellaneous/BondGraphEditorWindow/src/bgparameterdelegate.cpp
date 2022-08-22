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

#include <QHBoxLayout>
#include <QValidator>
#include <limits>

#include "bgparameterimportdialog.h"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnewline-eof"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
#endif
#include "bondgraph.hpp"

#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
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
#    pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace OpenCOR {
namespace BondGraphEditorWindow {

BrowseLineEdit::BrowseLineEdit(QWidget *parent, BGElementEditorScene *scene) :
    QFrame(parent, nullptr), m_scene(scene), m_parentName("")
{
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

QSize BrowseLineEdit::sizeHint() const
{
    QSize sizeLineEditor = line_editor->sizeHint(),
          sizeBrowseButton = browse_button->sizeHint();

    int w = sizeLineEditor.rwidth() + sizeBrowseButton.rwidth(),
        h = qMax(sizeLineEditor.rheight(), sizeBrowseButton.rheight());

    return QSize(w, h);
}

QSize BrowseLineEdit::minimumSizeHint() const
{
    QSize sizeLineEditor = line_editor->minimumSizeHint(),
          sizeBrowseButton = browse_button->minimumSizeHint();

    int w = sizeLineEditor.rwidth() + sizeBrowseButton.rwidth(),
        h = qMax(sizeLineEditor.rheight(), sizeBrowseButton.rheight());

    return QSize(w, h);
}

nlohmann::json BrowseLineEdit::json() const
{
    return m_json;
}

void BrowseLineEdit::setAsState(bool state)
{
    m_isState = state;
}

void BrowseLineEdit::setJson(nlohmann::json &str)
{
    try {
        // The input may contain reference information, for instance to check the
        // dimensions of the selected cellml variable to be consistent with the
        // chosen one this is stored in the record field, the value is stored in the
        // value field The value object it self may have another value object and
        // this layer needs to be removed
        if (str.contains("record")) {
            m_reference = str["record"];
            setAsState(m_reference["isState"]);
            if (!str["value"].is_object()) {
                m_json["value"] = str["value"];
            } else {
                m_json = str["value"];
            }
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
        Q_UNUSED(ex);
    }
}

void BrowseLineEdit::setParentName(QString parent_name)
{
    m_parentName = parent_name;
}

void BrowseLineEdit::editing_finished()
{
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

void BrowseLineEdit::browse()
{
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
                                     BGElementEditorScene *scene) :
    QItemDelegate(parent),
    m_scene(scene), m_ElementName(""),
    m_AllowDimensionChange(false)
{
}

QSize ParameterDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

void ParameterDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    // load pattern description stored in the StatusLine
    QString patternDescription = index.data(Qt::StatusTipRole).toString();

    QRegExp rxString("\\b(FileName)\\b");

    // if the type is Filename open parameter spec dialog
    if (rxString.indexIn(patternDescription) != -1) {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();

        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

        // print the text in the display
        drawDisplay(painter, myOption, myOption.rect, value);
        // if the line has the focus, print a rectangle
        drawFocus(painter, myOption, myOption.rect);
    } else {
        // for all other types use the standard delegate
        QItemDelegate::paint(painter, option, index);
    }
}

QWidget *ParameterDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &,
                                         const QModelIndex &index) const
{
    // load pattern description stored in the StatusLine
    QString patternDescription = index.data(Qt::StatusTipRole).toString();

    QRegExp rxString("\\b(Anything)\\b"), rxList("\\b(List)\\b"),
        rxFilename("\\b(FileName)\\b"), rxUnit("\\b(Unit)\\b"),
        rxInteger("\\b(Integer)\\b"), rxDouble("\\b(Double|Float|Floating)\\b"),
        rxSelection("\\b(Selection)\\b"), rxBool("\\b(Bool)\\b");

    // if the type is "Anything" choose a LineEditor
    if (rxString.indexIn(patternDescription) != -1) {
        QLineEdit *lineEditor = new QLineEdit(parent);
        connect(lineEditor, SIGNAL(editingFinished()), this,
                SLOT(commit_and_close_editor()));

        return lineEditor;
    } else if (rxList.indexIn(patternDescription) != -1) {
        // if the type is "List" of files , the use BrowseLineEditor
        if (rxFilename.indexIn(patternDescription) != -1) {
            BrowseLineEdit *parameterEditor = new BrowseLineEdit(parent, m_scene);
            parameterEditor->setParentName(m_ElementName);
            connect(parameterEditor, SIGNAL(editingFinished()), this,
                    SLOT(commit_and_close_editor()));

            return parameterEditor;
        }
        // if the type is "List" of something else choose a LineEditor
        else {
            QLineEdit *lineEditor = new QLineEdit(parent);

            connect(lineEditor, SIGNAL(editingFinished()), this,
                    SLOT(commit_and_close_editor()));

            return lineEditor;
        }
    }
    // if the type is "FileName" choose a BrowseLineEditor
    else if (rxFilename.indexIn(patternDescription) != -1) {
        BrowseLineEdit *parameterEditor = new BrowseLineEdit(parent, m_scene);
        parameterEditor->setParentName(m_ElementName);
        connect(parameterEditor, SIGNAL(editingFinished()), this,
                SLOT(commit_and_close_editor()));

        return parameterEditor;
    }
    // if the type is "UNIT" choose a LineEdit with unit validation
    else if (rxUnit.indexIn(patternDescription) != -1) {
        QLineEdit *lineEdit = new QLineEdit(parent);

        connect(lineEdit, SIGNAL(editingFinished()), this,
                SLOT(commit_and_close_editor()));

        return lineEdit;
    }
    // if the type is "Integer" choose a LineEditor with appropriate bounds
    else if (rxInteger.indexIn(patternDescription) != -1) {
        const QStringList defaultPattern =
            patternDescription.split(" ").filter("...");
        const QStringList defaultValues = defaultPattern[0].split("...");

        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setValidator(new QIntValidator(
            defaultValues[0].toInt(), defaultValues[1].toInt(), lineEdit));

        connect(lineEdit, SIGNAL(editingFinished()), this,
                SLOT(commit_and_close_editor()));

        return lineEdit;
    }
    // if the type is "Double" choose a LineEditor with appropriate bounds
    else if (rxDouble.indexIn(patternDescription) != -1) {
        const QStringList defaultPattern =
            patternDescription.split(" ").filter("...");
        QStringList defaultValues = defaultPattern[0].split("...");

        // Unfortunately conversion of MAX_DOUBLE to string and back fails
        // sometimes, therefore use MAX_DOUBLE/2 to make sure we are below.
        // In practice MAX_DOUBLE just means VERY large, it is normally not
        // important how large.
        const double maxDouble = std::numeric_limits<double>::max() / 2;
        defaultValues = defaultValues.replaceInStrings(
            "MAX_DOUBLE", QVariant(maxDouble).toString());

        const unsigned int numberOfDecimals = 14;

        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setValidator(new QDoubleValidator(
            defaultValues[0].toDouble(), defaultValues[1].toDouble(),
            numberOfDecimals, lineEdit));

        connect(lineEdit, SIGNAL(editingFinished()), this,
                SLOT(commit_and_close_editor()));

        return lineEdit;
    }
    // if the type is "Selection" choose a ComboBox
    else if (rxSelection.indexIn(patternDescription) != -1) {
        QComboBox *comboBox = new QComboBox(parent);

        // we assume, that pattern_description is of the form
        // "Type: [Selection item1|item2| ....|item ]".
        // Find the first space after the first '[',
        // which indicates the start of the first option
        int beginPattern = patternDescription.indexOf("[");
        beginPattern = patternDescription.indexOf(" ", beginPattern) + 1;

        // Find the last ']', which signals the end of the options
        const int endPattern = patternDescription.lastIndexOf("]");

        // Extract the options from the string
        QString pattern =
            patternDescription.mid(beginPattern, endPattern - beginPattern);

        // Remove trailing whitespaces
        while (pattern.endsWith(' '))
            pattern.chop(1);

        // Split the list
        const QStringList choices = pattern.split("|");
        comboBox->addItems(choices);

        comboBox->setEditable(false);

        connect(comboBox, SIGNAL(currentIndexChanged(int)), this,
                SLOT(commit_and_close_editor()));

        return comboBox;
    }
    // if the type is "Bool" choose a ComboBox
    else if (rxBool.indexIn(patternDescription) != -1) {
        QComboBox *comboBox = new QComboBox(parent);

        std::vector<std::string> choices;
        choices.push_back(std::string("true"));
        choices.push_back(std::string("false"));

        // add items to the combo box
        for (size_t i = 0; i < choices.size(); ++i)
            comboBox->addItem(tr(choices[i].c_str()), tr(choices[i].c_str()));

        comboBox->setEditable(false);

        connect(comboBox, SIGNAL(currentIndexChanged(int)), this,
                SLOT(commit_and_close_editor()));

        return comboBox;
    };

    // if it is not the column "parameter values", do nothing
    return nullptr;
}

void ParameterDelegate::setElementName(QString e_name)
{
    m_ElementName = e_name;
}

void ParameterDelegate::setDimensionAsMutable(bool mutate)
{
    m_AllowDimensionChange = mutate;
}

void ParameterDelegate::setEditorData(QWidget *editor,
                                      const QModelIndex &index) const
{
    if (BrowseLineEdit *parameterEditor =
            qobject_cast<BrowseLineEdit *>(editor)) {
        QString jsonS = index.data(Qt::UserRole).toString();

        nlohmann::json j;
        nlohmann::json j1;
        try {
            j = nlohmann::json::parse(jsonS.toStdString());
            // Parameter Editor expects an object with "value" key
            if (!j.is_object()) {
                j1["value"] = j;
                j = j1;
            }
        } catch (nlohmann::json::parse_error &ex) {
            Q_UNUSED(ex);
            j["value"] = jsonS.toStdString();
        }

        parameterEditor->setJson(j);
    } else if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
        QRegExp rx(index.data(Qt::DisplayRole).toString());

        // Preset ComboBox to the current selection
        for (int i = 0; i < comboBox->count(); ++i) {
            if (rx.exactMatch(comboBox->itemText(i))) {
                comboBox->setCurrentIndex(i);
            }
        }
    } else
        QItemDelegate::setEditorData(editor, index);
}

void ParameterDelegate::commit_and_close_editor()
{
    QWidget *editor = qobject_cast<QWidget *>(sender());
    Q_EMIT commitData(editor);
    Q_EMIT closeEditor(editor);
}

void ParameterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor)) {
        QString lineEditContent = lineEdit->text();
        if (lineEditContent.size() == 0)
            return;
        int position = 0;
        // If the editor has a validator, only accept the content if it contains
        // correct input. Otherwise bad cases
        // can happen, when some unfinished number violates the bounds, and
        // the user clicks elsewhere to lose the focus of the editor.
        if (lineEdit->validator()) {
            auto vres = lineEdit->validator()->validate(lineEditContent, position);
            if (vres == QValidator::Acceptable)
                model->setData(index, lineEditContent);
            return;
        }

        if (!m_AllowDimensionChange) {
            QString patternDescription = index.data(Qt::StatusTipRole).toString();
            QRegExp rxUnit("\\b(Unit)\\b");
            if (rxUnit.indexIn(patternDescription) != -1) {
                QString targetUnit = index.data(Qt::DisplayRole).toString();
                auto res = BG::checkUnits(lineEditContent.toStdString(),
                                          targetUnit.toStdString());
                if (res.size() != 0) {
                    auto msh = QString::fromStdString(res);
                    auto tres = BG::checkUnits(targetUnit.toStdString(), "");
                    auto tmsh = QString::fromStdString(tres);
                    // Cannot qMessage from non ui thread, so send a signal
                    Q_EMIT invalidUserInput(tr("Inconsistent or invalid units specified. "
                                               "Expected (Base SI) ")
                                            + targetUnit + " ( " + tmsh + " ) " + tr("given") + " " + lineEditContent + " ( " + msh + " ) \n Reverted to previous value!");
                    return;
                }
            }
        }

        model->setData(index, lineEditContent);
    } else if (BrowseLineEdit *parameterEditor =
                   qobject_cast<BrowseLineEdit *>(editor)) {
        auto value = parameterEditor->json();
        model->setData(index, QString::fromStdString(value.dump()), Qt::UserRole);
    } else if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
        QString value = comboBox->currentText();
        model->setData(index, value);
    }
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR
