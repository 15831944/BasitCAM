#ifndef GCODEDIALOG_H
#define GCODEDIALOG_H

#include <QDialog>

namespace Ui {
class GCodeDialog;
}

class OccView;
class QStringListModel;

class GCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GCodeDialog(OccView *occView, QWidget *parent = 0);
    ~GCodeDialog();

public slots:
    void ChangedTools(const QStringListModel *tools);
private slots:
    void selectedObject();

    void on_GCodeWriteButton_clicked();

    void on_whereB_clicked();

private:
    Ui::GCodeDialog *ui;
    OccView *m_OccView;

};

#endif // GCODEDIALOG_H
