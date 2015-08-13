#ifndef GRIDDIALOG_H_
#define GRIDDIALOG_H_

#include <QDialog>

class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class OccView;

class GridDialog : public QDialog
{
    Q_OBJECT
public:
    GridDialog(OccView *occView, QWidget *parent = 0);

private slots:
    void okButton();
    void cancelButton();
    void isChecked(bool isChecked);

private:

    void initLayout();
    void initConnections();


    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    QCheckBox   *m_isCheck;
    QLineEdit   *m_xCount;
    QLineEdit   *m_yCount;
    QLineEdit   *m_xDistance;
    QLineEdit   *m_yDistance;
    QLabel      *m_xCountLabel;
    QLabel      *m_yCountLabel;
    QLabel      *m_xDistLabel;
    QLabel      *m_yDistLabel;

    OccView *m_occView;


};

#endif
