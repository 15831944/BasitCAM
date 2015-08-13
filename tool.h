#ifndef TOOL_H
#define TOOL_H

#include <qstring.h>
#include <QStringList>
class Tool
{
public:
    Tool(QString name, double diameter, double spindle, double feedrate);

    QString Name() const;
    double Diameter()const;
    double SpindleSpeed() const;
    double Feedrate() const;
    QStringList StringList() const;

    static Tool *MakeTool(QStringList slist);

private:
    QString m_name;
    double m_dia;
    double m_spindleSpeed;
    double m_feedrate;

};

#endif // TOOL_H
