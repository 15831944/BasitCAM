#ifndef GCODEDIALOG_H
#define GCODEDIALOG_H

#include <QDialog>
#include <QVector>
#include <QMap>

namespace Ui {
class GCodeDialog;
}

#include <TopoDS_Shape.hxx>
#include <NCollection_Vector.hxx>
#include <TopoDS_Edge.hxx>
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <gp_Pnt.hxx>

class occQt;
class QStringListModel;
class TopoDS_Wire;
class TopTools_HSequenceOfShape;
class TPOperation;

class GCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GCodeDialog(occQt *app, QWidget *parent = 0);
    ~GCodeDialog();

public slots:
    void ChangedTools(const QStringListModel *tools);
    virtual void show();
private slots:
    //void selectedObject();

    void on_GCodeWriteButton_clicked();

    void on_whereB_clicked();

    void on_BStartPoint_clicked();

    void on_BPostProcessor_clicked();

    void on_BAddContour_clicked();

private:
    Ui::GCodeDialog *ui;
    occQt *m_app;
    QStringListModel *m_operationModel;
    QMap<QString, TPOperation *> m_operations;

    void SelectedContour(QVector<TopoDS_Edge> &path, Handle(TopTools_HSequenceOfShape) &wires);

    TopoDS_Shape Offset(const TopoDS_Wire &wire, double offsetVal) const;
    gp_Pnt m_startP;
    bool m_isClickedStartP;
};

#endif // GCODEDIALOG_H
