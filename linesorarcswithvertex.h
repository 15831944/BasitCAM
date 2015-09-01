#ifndef LINESORARCSWITHVERTEX_H
#define LINESORARCSWITHVERTEX_H

#include <QVector>
#include <gp_Pnt.hxx>
#include <TopoDS_Wire.hxx>

class LinesOrArcsWithVertex
{
public:
    LinesOrArcsWithVertex();

    void AddPoint(const gp_Pnt &p, double bulge = 0);
    bool IsEmpty() const;
    TopoDS_Wire GetWireShape();
    void SetClose(bool isClose);
private:
    QVector<gp_Pnt> m_points;
    QVector<double> m_bulges;
    bool m_isClosed;
};

#endif // LINESORARCSWITHVERTEX_H
