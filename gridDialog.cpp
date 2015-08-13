#include "gridDialog.h"
#include  "OccView.h"

#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QIntValidator>
#include <qdebug.h>

GridDialog::GridDialog(OccView *occView, QWidget *parent)
            : m_occView(occView), QDialog(parent)
{
    m_okButton = new QPushButton("Ok", this);
    m_cancelButton = new QPushButton("Cancel", this);
    m_isCheck = new QCheckBox("Hide/Show", this);

    m_xCount = new QLineEdit(this);
    m_xCount->setValidator(new QIntValidator(this));
    m_xCount->setText("10");

    m_yCount = new QLineEdit(this);
    m_yCount->setValidator(new QIntValidator(this));
    m_yCount->setText("10");

    m_xDistance  = new QLineEdit(this);
    m_xDistance->setValidator(new QIntValidator(this));
    m_xDistance->setText("100");

    m_yDistance = new QLineEdit(this);
    m_yDistance->setValidator(new QIntValidator(this));
    m_yDistance->setText("100");

    m_xCountLabel = new QLabel("X Count :", this);
    m_yCountLabel = new QLabel("Y Count :", this);
    m_xDistLabel = new QLabel("X Size :", this);
    m_yDistLabel = new QLabel("Y Size :", this);





    initLayout();
    initConnections();

    m_occView->initGrid(0.0, 0.0, 10.0, 10.0, 100.0, 100.0);
    m_isCheck->setChecked(true);
    isChecked(true);


}

void GridDialog::initLayout()
{

    QHBoxLayout *lay1 = new QHBoxLayout;
    lay1->addWidget(m_xCountLabel);
    lay1->addWidget(m_xCount);
    lay1->addWidget(m_yCountLabel);
    lay1->addWidget(m_yCount);

    QHBoxLayout *lay2 = new QHBoxLayout;
    lay2->addWidget(m_xDistLabel);
    lay2->addWidget(m_xDistance);
    lay2->addWidget(m_yDistLabel);
    lay2->addWidget(m_yDistance);


    QHBoxLayout *buttonsLay = new QHBoxLayout;
    buttonsLay->addWidget(m_okButton);
    buttonsLay->addWidget(m_cancelButton);
    buttonsLay->addWidget(m_isCheck);

    QVBoxLayout *vLay = new QVBoxLayout;
    vLay->addLayout(lay1);
    vLay->addLayout(lay2);
    vLay->addLayout(buttonsLay);

    setLayout(vLay);
}

void GridDialog::initConnections()
{
    connect(m_okButton, SIGNAL(clicked(bool)), this, SLOT(okButton()));
    connect(m_cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelButton()));
    connect(m_isCheck, SIGNAL(clicked(bool)), this, SLOT(isChecked(bool)));
}

void GridDialog::okButton()
{

    //isChanged
    double xStep = m_xCount->text().toInt();
    double yStep = m_yCount->text().toInt();
    double xSize = m_xDistance->text().toInt();
    double ySize = m_yDistance->text().toInt();

    qDebug() << xStep;
    qDebug() << yStep;
    qDebug() << xSize;
    qDebug() << ySize;

    m_occView->initGrid(0.0, 0.0, xStep, yStep, xSize, ySize);
    accept();
}

void GridDialog::cancelButton()
{
    hide();
}
void GridDialog::isChecked(bool isChecked)
{
    if (isChecked)
        m_occView->showGrid();
    else
        m_occView->hideGrid();
}
