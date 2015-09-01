#include "tool.h"

#include <qstringlist.h>

Tool::Tool()
{

}

Tool::Tool(QString name, double diameter, double spindle, double feedrate)
    :   m_name(name),
        m_dia(diameter),
        m_spindleSpeed(spindle),
        m_feedrate(feedrate)
{

}

QString Tool::Name() const
{
    return m_name;
}

double Tool::SpindleSpeed() const
{
    return m_spindleSpeed;
}
double Tool::Feedrate() const
{
    return m_feedrate;
}

double Tool::Diameter() const
{
    return m_dia;
}

QStringList Tool::StringList() const
{
    QStringList slist;
    slist << m_name << QString::number(m_dia) << QString::number(m_spindleSpeed) << QString::number(m_feedrate);
    return slist;
}
Tool *Tool::MakeTool(QStringList slist)
{
    if (slist.count() != 4)
        return NULL;

    return new Tool(slist.at(0), slist.at(1).toDouble(), slist.at(2).toDouble(), slist.at(3).toDouble());

}
