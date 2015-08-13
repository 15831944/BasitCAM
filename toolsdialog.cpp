#include "toolsdialog.h"
#include "ui_toolsdialog.h"
#include "tool.h"

#include <QStringListModel>
#include <QMessageBox>
#include <qsettings.h>
#include <QVariant>
#include <QDebug>
#include <QAbstractItemView>

ToolsDialog::ToolsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToolsDialog)
{
    ui->setupUi(this);


    ui->speed->setValidator(new QDoubleValidator(0., 999999., 10, this));
    ui->feedrate->setValidator(new QDoubleValidator(0, 999999., 10, this));
    ui->diameter->setValidator(new QDoubleValidator(0, 999999., 10, this));

    m_settingsFile = QApplication::applicationDirPath().left(1) + ":/toolSettings.ini";

    m_model = new QStringListModel(this);
    ui->toolList->setModel(m_model);
    ui->toolList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ReadSettings();

}

ToolsDialog::~ToolsDialog()
{
    WriteSettings();
    delete ui;
}

void ToolsDialog::on_addTool_clicked()
{
    QString name = ui->toolName->text();
    QString dia = ui->diameter->text();
    QString speed = ui->speed->text();
    QString fr = ui->feedrate->text();

    if (name.isEmpty() || dia.isEmpty() || speed.isEmpty() || fr.isEmpty()) {
        QMessageBox::critical(this, "Wrong", "Empty");
        return;
    }

    if (m_model->stringList().contains(name)) {
        QMessageBox::critical(this, "Wrong", "Change to name");
        return;
    }
    Tool *t = new Tool(name, ui->diameter->text().toDouble()
                       , ui->speed->text().toDouble()
                       , ui->feedrate->text().toDouble());
    m_tools.push_back(t);
    m_model->setStringList(m_model->stringList() << name);
    emit ChangeToolList(m_model);
}

void ToolsDialog::on_removeTool_clicked()
{
    QModelIndex index = ui->toolList->currentIndex();
    if (!index.isValid())
        return;

    m_tools.remove(index.row());
    m_model->removeRow(index.row());
    emit ChangeToolList(m_model);
}

void ToolsDialog::on_toolList_clicked(const QModelIndex &index)
{
    Tool *t = m_tools.at(index.row());
    ui->toolName->setText(t->Name());
    ui->diameter->setText(QString::number(t->Diameter()));
    ui->speed->setText(QString::number(t->SpindleSpeed()));
    ui->feedrate->setText(QString::number(t->Feedrate()));
}

void ToolsDialog::ReadSettings()
{
    //QSettings s(m_settingsFile, QSettings::NativeFormat);
    QSettings s(m_settingsFile);
    int size = s.value("toolsSize").toInt();

    QStringList toolsName;
    for (int i = 0; i < size; ++i) {
        QStringList slist = s.value("tools" + QString::number(i)).toStringList();
        m_tools.push_back(Tool::MakeTool(slist));

        toolsName << slist.at(0);
    }
    m_model->setStringList(toolsName);
}

void ToolsDialog::WriteSettings() const
{
    //QSettings s(m_settingsFile, QSettings::NativeFormat);
    QSettings s(m_settingsFile);
    s.setValue("toolsSize", m_tools.size());
    for (int i = 0; i < m_tools.size(); ++i) {
        s.setValue(("tools" + QString::number(i)), m_tools.at(i)->StringList());
    }

}
const QStringListModel *ToolsDialog::GetModelList() const
{
    return m_model;
}
