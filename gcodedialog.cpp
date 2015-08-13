#include "gcodedialog.h"
#include "ui_gcodedialog.h"
#include "OccView.h"
#include "basicheaders.h"

#include <qdebug.h>
#include <qvector.h>
#include <QStringListModel>
#include <QFileDialog>

GCodeDialog::GCodeDialog(OccView *occView, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GCodeDialog),
    m_OccView(occView)
{
    ui->setupUi(this);
    ui->onRB->setChecked(true);

}

GCodeDialog::~GCodeDialog()
{
    delete ui;
}



void GCodeDialog::selectedObject()
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

void GCodeDialog::on_GCodeWriteButton_clicked()
{

    Handle_AIS_InteractiveContext myContext = m_OccView->getContext();
    qDebug() << myContext->NbSelected();

    QVector <const TopoDS_Edge *> path;
    std::vector <const TopoDS_Edge > path2;
    for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
    {
        Handle_AIS_InteractiveObject picked;
        picked= myContext->SelectedInteractive();
        Handle(AIS_Shape) aShape=Handle(AIS_Shape)::DownCast(picked);
        TopoDS_Shape sh = aShape->Shape();

        if (aShape->Shape().ShapeType() == TopAbs_EDGE){
            const TopoDS_Edge &edge = (const TopoDS_Edge &)aShape->Shape();
            path.push_back(&edge);           //sıralamayı unutma
         }
    }
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
