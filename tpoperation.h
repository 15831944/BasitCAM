#ifndef TOOLPATHCALC_H
#define TOOLPATHCALC_H

#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <QString>
#include <tool.h>

class TPOperation
{
public:
    TPOperation(const QString &opName);
    void SetOperation(const Handle(TopTools_HSequenceOfShape) &operation);
    void SetTool(const Tool &t);
    void SetSafeZ(double safeZ);
    void SetPlungeZ(double plungeZ);

    QString GetToolName() const;
    double GetToolSpeed() const;
    double GetToolFeedRate() const;
    double GetSafeZ() const;
    double GetPlungeZ() const;

private:

    Tool m_tool;
    Handle(TopTools_HSequenceOfShape) m_operation;
    QString m_opName;
    double m_safeZ;
    double m_plungeZ;
};

#endif // TOOLPATHCALC_H
