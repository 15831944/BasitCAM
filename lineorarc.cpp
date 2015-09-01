#include "lineorarc.h"
#include "basicheaders.h"

LineOrArc::LineOrArc(const gp_Pnt &p1, const gp_Pnt &p2, double bulge)
{
    m_p1 = p1;
    m_p2 = p2;
    m_bulge = bulge;

}


bool LineOrArc::IsLine()
{
    return m_bulge == 0;

}

Handle(Geom_TrimmedCurve) LineOrArc::GetArc() const
{
    gp_Pnt midP((m_p1.XYZ() + m_p2.XYZ()) / 2.0);
    gp_Dir dir = gp::DZ();
    double tempBulge = m_bulge;
    if (tempBulge < 0) {
        tempBulge = m_bulge * -1;
        dir = gp_Dir(0, 0, -1);
    }

    gp_Vec vec1(m_p1, m_p2);
    vec1.Normalize();
    gp_Vec vec2(vec1 ^ gp::DZ());
    vec2.Scale(tempBulge);
    midP.Translate(vec2);

    return GC_MakeArcOfCircle(m_p1, midP, m_p2);
}
Handle(Geom_TrimmedCurve) LineOrArc::GetLine() const
{
    return GC_MakeSegment(m_p1, m_p2);
}

