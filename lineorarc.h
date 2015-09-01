#ifndef LINEORARC_H
#define LINEORARC_H

#include <gp_Pnt.hxx>
#include <Geom_TrimmedCurve.hxx>

class LineOrArc
{
public:
    LineOrArc(const gp_Pnt &p1, const gp_Pnt &p2, double bulge = 0);

    bool IsLine();
    Handle_Geom_TrimmedCurve GetArc() const;
    Handle(Geom_TrimmedCurve) GetLine() const;

private:
    double m_bulge;
    gp_Pnt m_p1;
    gp_Pnt m_p2;

};



#endif // LINEORARC_H
