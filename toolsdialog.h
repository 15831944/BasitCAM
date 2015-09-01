#ifndef TOOLSDIALOG_H
#define TOOLSDIALOG_H

#include <QDialog>

namespace Ui {
class ToolsDialog;
}

class QStringListModel;
class Tool;

class ToolsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolsDialog(QWidget *parent = 0);
    ~ToolsDialog();

    const QStringListModel *GetModelList() const;
    const Tool *GetTool(int index) const;

signals:
    void ChangeToolList(const QStringListModel *tools);
private slots:
    void on_addTool_clicked();

    void on_removeTool_clicked();

    void on_toolList_clicked(const QModelIndex &index);

private:
    Ui::ToolsDialog *ui;
    QVector <Tool *> m_tools;
    QStringListModel *m_model;

    void WriteSettings() const;
    void ReadSettings();
    QString m_settingsFile;
};

#endif // TOOLSDIALOG_H
