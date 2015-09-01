#ifndef GENERICALGOS_H
#define GENERICALGOS_H

#include <QVector>
#include <TopoDS_Edge.hxx>
#include <NCollection_Vector.hxx>
#include <Geom_TrimmedCurve.hxx>
//class NCollection_Vector;
class BRepAdaptor_Curve;
class gp_Pnt;

//namespace GenAlgo {




void ConvertTopoToCurve(const QVector <const TopoDS_Edge *> &edges, QVector <BRepAdaptor_Curve *> &curves);

int FindNearestItem(const QVector<BRepAdaptor_Curve *> &curves, const gp_Pnt &pnt);

void SortingCurves(const QVector<BRepAdaptor_Curve *> &curves, const gp_Pnt &startP, QVector<BRepAdaptor_Curve *> &sorted);

bool SortEdgesForConstructWire(const NCollection_Vector<TopoDS_Edge>& oldedges,  NCollection_Vector<TopoDS_Edge>& newedges, double tol, bool *pClosed, double* pMaxGap);

void MakeArcWithBulge(const gp_Pnt &p1, const gp_Pnt &p2, double bulge, gp_Pnt &p3);

//}







#endif // GENERICALGOS_H
