#include "tpoperation.h"
#include "tool.h"


#include <QString>

TPOperation::TPOperation(const QString &opName)
{
    m_opName = opName;
}



void TPOperation::SetOperation(const Handle(TopTools_HSequenceOfShape) &operation)
{
    m_operation = operation;
}

void TPOperation::SetTool(const Tool &t)
{
    m_tool = t;
}
void TPOperation::SetSafeZ(double safeZ)
{
    m_safeZ = safeZ;
}
void TPOperation::SetPlungeZ(double plungeZ)
{
    m_plungeZ = plungeZ;
}

QString TPOperation::GetToolName() const
{
    return m_tool.Name();
}
double TPOperation::GetToolSpeed() const
{
    return m_tool.SpindleSpeed();
}
double TPOperation::GetToolFeedRate() const
{
    return m_tool.Feedrate();
}
double TPOperation::GetSafeZ() const
{
    return m_safeZ;
}
double TPOperation::GetPlungeZ() const
{
    return m_plungeZ;
}
