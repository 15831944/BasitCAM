#include "gcodewriter.h"
#include "basicheaders.h"
#include "tpoperation.h"

#include <QtScript>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

GCodeWriter::GCodeWriter(const QString &fileName, const QString &postPath)
            : QObject()
            , m_fileName(fileName)
            , m_postPath(postPath)
{

}

void GCodeWriter::SetTPOperation(TPOperation *operation)
{
    m_operation = operation;
}


void printPoint(const gp_Pnt &p1, const gp_Pnt &p2)
{
    qDebug() << p1.X() << p1.Y();
    qDebug() << p2.X() << p2.Y();
}


void GCodeWriter::Ordering(const QVector <const TopoDS_Edge *> &path, const gp_Pnt &startP) const
{
    QVector <const TopoDS_Edge *> newPath;
    gp_Pnt p1, p2;

    //for (int i = 0; i < path.size(); ++i) {
        for (int k = 0; k < path.size(); ++k) {
            BRepAdaptor_Curve curv(*path[k]);
            if (curv.GetType() == GeomAbs_Line) {
                qDebug() << "line";
                curv.D0(0.0, p1);
                curv.D0(curv.LastParameter(), p2);
            }
            else if(curv.GetType() == GeomAbs_Circle) {
                qDebug() << "circle";
                curv.D0(0.0, p1);
                curv.D0(curv.LastParameter(), p2);

            }
            else {
                qDebug() << curv.GetType();
            }

            printPoint(p1, p2);
        }
    //}
}

void GCodeWriter::Write()
{

    // open output stream
    QFile file(m_fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        //throw
        qDebug() << "GCodeWriter::Write cannot file";
    }

    m_stream.setDevice(&file);

    // open post stream and engine
    QScriptEngine engine;
    QFile jsFile(m_postPath);

    if(!jsFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Wrong", "Cannot open post processor file");
        return;
    }

    QTextStream in(&jsFile);
    QScriptValue result = engine.evaluate(in.readAll());


    if (engine.hasUncaughtException()) {
        int line = engine.uncaughtExceptionLineNumber();
        QString err = result.toString();
        QString msg = QString("Error at line %1: %2").arg(line).arg(err);
        QMessageBox::critical(0, "Error", msg);
    }


    QScriptValue constructor = engine.evaluate("PostProcessor");
    QScriptValue obj = engine.newQObject(this);

    constructor.construct(QScriptValueList() << obj);

    QScriptValue foo = engine.evaluate("foo");
    foo.construct(QScriptValueList() << obj << 3 );



    //WriteOperation();


    file.flush();
    file.close();
    jsFile.close();


}



void GCodeWriter::WriteFromScript(const QString &str)
{
    m_stream << str;
}

QString GCodeWriter::GetToolName() const
{
    return m_operation->GetToolName();
}
double GCodeWriter::GetFeedRate() const
{
    return m_operation->GetToolFeedRate();
}
double GCodeWriter::GetSpeed() const
{
    return m_operation->GetToolSpeed();
}
double GCodeWriter::GetSafeZ() const
{
    return m_operation->GetSafeZ();
}
double GCodeWriter::GetPlungeZ() const
{
    return m_operation->GetPlungeZ();
}
