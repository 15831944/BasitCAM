#ifndef GCODEWRITER_H
#define GCODEWRITER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <gp_Pnt.hxx>
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <TopTools_HSequenceOfShape.hxx>

class TopoDS_Edge;
class QTextStream;
class TPOperation;

class GCodeWriter : public QObject
{
    Q_OBJECT
public:
    GCodeWriter(const QString &fileName, const QString &postPath);
    void SetTPOperation(TPOperation *operation);
    void Write();

public slots:
    void WriteFromScript(const QString &str);
    QString GetToolName() const;
    double GetFeedRate() const;
    double GetSpeed() const;
    double GetSafeZ() const;
    double GetPlungeZ() const;
private:
    void Ordering(const QVector <const TopoDS_Edge *> &path, const gp_Pnt &startP) const;
    QString m_fileName;
    QString m_postPath;
    QTextStream m_stream;

    TPOperation *m_operation;
};

#endif // GCODEWRITER_H
