#include "linesorarcswithvertex.h"
#include "lineorarc.h"
#include "basicheaders.h"
#include "genericalgos.h"
#include <qdebug.h>

LinesOrArcsWithVertex::LinesOrArcsWithVertex()
        : m_isClosed(false)
{

}



void LinesOrArcsWithVertex::AddPoint(const gp_Pnt &p, double bulge)
{
    if (!m_points.isEmpty())
        if (p.IsEqual(m_points.back(), Precision::Confusion()))
            return;

    m_points.push_back(p);
    m_bulges.push_back(bulge);
}


TopoDS_Wire LinesOrArcsWithVertex::GetWireShape()
{
    TopoDS_Wire wire;
    BRepBuilderAPI_MakeWire mkWire;
    int size = m_points.size();

    for (int i = 0; i < size - 1; ++i) {
        gp_Pnt p1 = m_points[i];
        gp_Pnt p2 = m_points[i + 1];

        BRepBuilderAPI_MakeEdge mkEdge;
        if (m_bulges[i] == 0) { // add line

            mkEdge = BRepBuilderAPI_MakeEdge(GC_MakeSegment(p1, p2));
        }
        else {      //   add arc
            //mkEdge = BRepBuilderAPI_MakeEdge(GC_MakeSegment(p1, p2));
            gp_Pnt p3;
            MakeArcWithBulge(p1, p2, m_bulges[i], p3);
            mkEdge = BRepBuilderAPI_MakeEdge(GC_MakeArcOfCircle(p1, p3, p2));
        }

            mkWire.Add(mkEdge);

    }
    if (m_isClosed) {
        if (!m_points.isEmpty()) {
            gp_Pnt p1 = m_points.back();
            gp_Pnt p2 = m_points.first();
            double bulge = m_bulges.back();
            if (bulge == 0) {
                mkWire.Add(BRepBuilderAPI_MakeEdge(GC_MakeSegment(p1, p2)));
            }
            else {
                gp_Pnt p3;
                MakeArcWithBulge(p1, p2, bulge, p3);
                mkWire.Add(BRepBuilderAPI_MakeEdge(GC_MakeArcOfCircle(p1, p3, p2)));
            }

        }

    }

    return mkWire;
}
bool LinesOrArcsWithVertex::IsEmpty() const
{
    return m_points.isEmpty();
}
void LinesOrArcsWithVertex::SetClose(bool isClose)
{
    m_isClosed = isClose;
}
