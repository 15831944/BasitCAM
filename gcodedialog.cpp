#include "gcodedialog.h"
#include "ui_gcodedialog.h"
#include "OccQt.h"
#include "OccView.h"
#include "basicheaders.h"
#include "gcodewriter.h"
#include "toolsdialog.h"
#include "tool.h"
#include "genericalgos.h"
#include "tpoperation.h"

#include <qdebug.h>
#include <qvector.h>
#include <QStringListModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QMap>
#include <QAbstractItemView>


GCodeDialog::GCodeDialog(occQt *app, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GCodeDialog),
    m_app(app),
    m_isClickedStartP(false)
{
    ui->setupUi(this);
    m_operationModel = new QStringListModel(this);
    ui->LVContourList->setModel(m_operationModel);
    ui->LVContourList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

GCodeDialog::~GCodeDialog()
{
    delete ui;
}



/*void GCodeDialog::selectedObject()
{
    if (isHidden())
        return;
    if (m_OccView->getContext()->HasSelectedShape()) {
        qDebug() << "seçildi ";
        TopoDS_Shape sh = m_OccView->getContext()->SelectedShape();
        qDebug() << sh.ShapeType();

    }
    else
    {
        qDebug() << "seçilemedi";
        return;
    }


    this->show();
}
*/
void GCodeDialog::on_GCodeWriteButton_clicked()
{


    QString fileName = ui->lePath->text();
    QString postName = ui->LEPostProcessor->text();
    if (fileName.isEmpty() || postName.isEmpty()) {
        QMessageBox::critical(this, "Wrong", "Empty File Path or Post");
        return;
    }

    QModelIndex modelIndex = ui->LVContourList->currentIndex();
    if (!modelIndex.isValid()) {
        QMessageBox::critical(this, "Wrong", "Any Selected Operation in List");
        return;
    }
    QString opName = m_operationModel->stringList().at(modelIndex.row());//yanlış

    QMap<QString, TPOperation *>::iterator iter = m_operations.find(opName);
    if (iter == m_operations.end()) {
        QMessageBox::critical(this, "Wrong", "Not Found Operation");
        return;
    }

    if (*iter == NULL) {
        QMessageBox::critical(this, "Wrong", "Unknow error(on_GCodeWriteButton_clicked)");
        return;
    }


    GCodeWriter writer(fileName, postName);
    writer.SetTPOperation(*iter);
    writer.Write();

    // open g code file
    QProcess *process = new QProcess(this);
    process->start("notepad.exe", QStringList() << fileName);
}

void GCodeDialog::ChangedTools(const QStringListModel *tools)
{
    ui->toolCBox->setModel((QStringListModel *)tools);
}

void GCodeDialog::on_whereB_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, "", "", "*.nc");
    str += ".nc";
    ui->lePath->setText(str);

}

void GCodeDialog::on_BStartPoint_clicked()
{
    hide();
    m_isClickedStartP = true;
    connect(m_app->GetView(), SIGNAL(MouseLeftPressed()), this, SLOT(show()));
}

void GCodeDialog::on_BPostProcessor_clicked()
{
    QString str = QFileDialog::getOpenFileName(this, "*.js");
    ui->LEPostProcessor->setText(str);
}


void GCodeDialog::SelectedContour(QVector <TopoDS_Edge> &path, Handle(TopTools_HSequenceOfShape) &wires)
{
    Handle_AIS_InteractiveContext myContext = m_app->GetView()->getContext();
    qDebug() << myContext->NbSelected();

    //std::vector <const TopoDS_Edge > path2;
    for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
    {
        Handle_AIS_InteractiveObject picked;
        picked= myContext->SelectedInteractive();
        Handle(AIS_Shape) aShape=Handle(AIS_Shape)::DownCast(picked);
        TopoDS_Shape sh = aShape->Shape();

        if (aShape->Shape().ShapeType() == TopAbs_EDGE){
            TopoDS_Edge &edge = (TopoDS_Edge &)aShape->Shape();
            path.push_back(edge);           //sıralamayı unutma

         }
    }

    Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape();
    for (int i = 0; i < path.size(); ++i)
        edges->Append(path[i]);

    double tol = 0.01;//Precision::Confusion();
    ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, tol, Standard_False, wires);


}

void GCodeDialog::on_BAddContour_clicked()
{
    int toolIndex = ui->toolCBox->currentIndex();
    if (toolIndex < 0) {
        QMessageBox::critical(this, "Wrong", "Any Selected Tool");
        return;
    }
    const Tool *t1 = m_app->GetToolsDialog()->GetTool(toolIndex);
    if (t1 == NULL) {
        QMessageBox::critical(this, "Wrong", "Tool Exception");
        return;
    }




    QString opName = ui->LEOperation->text();
    if (m_operationModel->stringList().contains(opName)) {
        QMessageBox::critical(this, "Wrong", "Change to Operation Name");
        return;
    }

    QVector <TopoDS_Edge> path;
    Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape();
    SelectedContour(path, wires);

    if (path.isEmpty()) {
        QMessageBox::critical(this, "Wrong", "Any Selected Curve");
        return;
    }


    bool isCW = ui->RbCW->isChecked();
    qDebug() << "11";

    for (int i = 1; i <= wires->Length(); ++i) {
        TopoDS_Wire wr = TopoDS::Wire(wires->Value(i));

        TopoDS_Shape resultOffseted;
        if (ui->outsideRB->isChecked()) {
            resultOffseted = Offset(wr, t1->Diameter() * 0.5);
        }
        else if (ui->insideRB->isChecked()) {
            resultOffseted = Offset(wr, t1->Diameter() * -0.5);
        }
        else {
            resultOffseted = wr;
        }

        m_app->draw(resultOffseted, Standard_True, Quantity_NOC_YELLOW);
    }


    TPOperation *tpOp = new TPOperation(opName);
    tpOp->SetOperation(wires);
    tpOp->SetTool(*t1);
    tpOp->SetSafeZ(ui->LeSafeZ->text().toDouble());
    tpOp->SetPlungeZ(ui->LePlungeZ->text().toDouble());
    m_operations.insert(opName, tpOp);

    qDebug() << "22";

    //Offset varsa yapılacak
    //ekrana çizilecek takım yolu

    m_operationModel->setStringList(m_operationModel->stringList() << opName);
    ui->LEOperation->setText(opName + "1");
}


TopoDS_Shape GCodeDialog::Offset(const TopoDS_Wire &wire, double offsetVal) const
{

    GeomAbs_JoinType join = GeomAbs_Intersection;
    bool isOpen = true;

    if (wire.Closed()) {
        join = GeomAbs_Arc;
        isOpen = false;
        qDebug() << "close";
    }


    gp_Dir aDir = gp::DZ();
    if (offsetVal > 0)
      aDir.SetZ(-1.0);

    TopoDS_Face workingPlane = BRepBuilderAPI_MakeFace(gp_Pln(gp::Origin(), aDir));
    BRepOffsetAPI_MakeOffset offset(workingPlane, join, isOpen);
    try
    {
    //offset.Init(GeomAbs_Intersection);
    offset.AddWire(wire);
    offset.Perform(offsetVal);
    }
    catch(std::exception &e) {
        qDebug() << QString(e.what());
    }
    catch(...) {
        qDebug() << "catch";

    }

    return offset.Shape();
}
void GCodeDialog::show()
{
    if (m_isClickedStartP) {
        disconnect(m_app->GetView(), SIGNAL(MouseLeftPressed()), this, SLOT(show()));
        m_startP = m_app->GetView()->GetLeftClickMousePos();
        m_isClickedStartP = false;
        qDebug () << m_startP.X() << m_startP.Y() << m_startP.Z();
    }
    ((QWidget *)this)->show();
}
