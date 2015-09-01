#include "genericalgos.h"
#include "basicheaders.h"

#include <QVector>
#include <limits>
#include <qdebug.h>



//Generic Algorithm
void ConvertTopoToCurve(const QVector <const TopoDS_Edge *> &edges, QVector<BRepAdaptor_Curve *> &curves)
{
    for (int i = 0; i < edges.size(); ++i) {
        curves.push_back(new BRepAdaptor_Curve(*edges[i]));
    }
}


//
int FindNearestItem(const QVector<BRepAdaptor_Curve *> &curves, const gp_Pnt &pnt)
{
    int resultIndex = -1;
    gp_Pnt sp, ep;
    double dif1, dif2, tempMinDif, minDif = std::numeric_limits<double>::lowest();

    for (int i = 0; i < curves.size(); ++i) {
        BRepAdaptor_Curve *c = curves[i];
        c->D0(0., sp);
        c->D0(c->LastParameter(), ep);
        dif1 = sp.Distance(pnt);
        dif2 = ep.Distance(pnt);
        if (dif1 < dif2)
            tempMinDif = dif1;
        else
            tempMinDif = dif2;

        if (tempMinDif < minDif) {
            minDif = tempMinDif;
            resultIndex = i;
        }
    }

   // result = curves[resultIndex];
    return resultIndex;
}


void SortingCurves(const QVector<BRepAdaptor_Curve *> &curves, const gp_Pnt &startP, QVector<BRepAdaptor_Curve *> &sorted)
{

    int index = FindNearestItem(curves, startP);
    if (index < 0)
        ;//throw Exception e;

    while (1) {

    }


   // sorted;




}
void MakeArcWithBulge(const gp_Pnt &p1, const gp_Pnt &p2, double bulge, gp_Pnt &p3)
{
    gp_Pnt midP((p1.XYZ() + p2.XYZ()) / 2.0);
    gp_Dir dir = gp::DZ();

    if (bulge < 0) {
        bulge *= -1;
        dir = gp_Dir(0, 0, -1);
    }
    double bulgeLen = bulge * p1.Distance(midP);

    gp_Vec vec1(p1, p2);
    vec1.Normalize();
    gp_Vec vec2(vec1 ^ dir);
    vec2.Normalize();
    vec2.Scale(bulgeLen);
    midP.Translate(vec2);
    p3 = midP;
    //return  GC_MakeArcOfCircle(p1, midP, p2);
}


TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight,
                    const Standard_Real myThickness, TopoDS_Shape &out)
{

    gp_Pnt aPnt1(-myWidth / 2., 0., 0.);
    gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt3(0, -myThickness / 2., 0);
    gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt5(myWidth / 2., 0, 0);



    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);

    TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    TopoDS_Edge e3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(e1, e2, e3);

    // Complete Profile
    gp_Trsf aTrsf;
    aTrsf.SetMirror(gp::OX());
    TopoDS_Shape aMirroredShape = BRepBuilderAPI_Transform(aWire, aTrsf);
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile= mkWire.Wire();

    // Body Prism the Profile
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
    gp_Vec aPrismVec(0, 0, myHeight);
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);


    //Create a Hollowed Solid
    TopoDS_Face faceToRemove;
    Standard_Real zMax = -1;

    TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE);
    for (; aFaceExplorer.More(); aFaceExplorer.Next()) {
        TopoDS_Face face = TopoDS::Face(aFaceExplorer.Current());
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(face);
        if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt pnt = aPlane->Location();
            Standard_Real aZ = pnt.Z();
            if (aZ > zMax) {
                zMax = aZ;
                faceToRemove = face;
            }
        }
    }

    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);

    myBody = BRepOffsetAPI_MakeThickSolid(myBody, facesToRemove, -myThickness / 50, 1.e-3);

    //Threading : Create Surface
    gp_Ax2 neckAx2(gp_Pnt(0, 0, myHeight), gp::DZ());
    Standard_Real myNeckRadius = myThickness / 4.;
    Standard_Real myNeckHeight = myHeight / 10.;

    Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);


    // Threading : Define 2D Curves
    gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
    gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
    gp_Ax2d anAx2d(aPnt, aDir);

    Standard_Real aMajor = 2. * M_PI;
    Standard_Real aMinor = myNeckHeight / 10;


    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);


    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);

    // Threading : Build Edges and Wires
    TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);



    // Create Threading
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);

    TopoDS_Shape myThreading = aTool.Shape();
    out = myThreading;

    // Building the Resulting Compound
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound (aRes);
    aBuilder.Add (aRes, myBody);
    aBuilder.Add (aRes, myThreading);

    return aRes;


}
int GetMatchTwoPntsPair(const gp_Pnt& b1, const gp_Pnt& e1, const gp_Pnt& b2, const gp_Pnt& e2,
                                       double& minDis, double& otherDis)
{
    double distance[4];
    int index=-1, i;

    distance[0]=b1.SquareDistance(b2);
    distance[1]=b1.SquareDistance(e2);
    distance[2]=e1.SquareDistance(b2);
    distance[3]=e1.SquareDistance(e2);

    minDis = std::numeric_limits<double>::max();
    for(i=0;i<4;++i)
    {
        if(distance[i]<minDis)
        {
            minDis = distance[i];
            index = i;
        }
    }
    otherDis = distance[3-index];

    minDis = std::sqrt(minDis);
    otherDis = std::sqrt(otherDis);
    return index;
}




bool SortEdgesForConstructWire(const NCollection_Vector<TopoDS_Edge>& oldedges,  NCollection_Vector<TopoDS_Edge>& newedges, double tol, bool *pClosed, double* pMaxGap)
{
    int i, n, minID, id, id2;
    NCollection_Vector<int> bTaken,  matchedIDs, sortedIDs;
    NCollection_Vector<gp_Pnt> pnts;
    TopExp_Explorer ex;
    std::vector<TopoDS_Edge> edges;
    gp_Pnt pnt, endPnts[2];
    bool bSucc;
    double minDis, otherDis, minminDis;
    Standard_Boolean bDeg;
    TopoDS_Vertex v1, v2;
    TopoDS_Edge degEdge;

    newedges.Clear();
    if(pMaxGap)
        *pMaxGap = 0.0;
    n = oldedges.Length();
    if(n==0)
        return false;

    for(i=0; i<n; ++i)
    {
        TopExp::Vertices(oldedges(i), v1, v2);
        pnts.Append(BRep_Tool::Pnt(v1));
        pnts.Append(BRep_Tool::Pnt(v2));
        bTaken.Append(0);
    }

    endPnts[0] = pnts(0);
    endPnts[1] = pnts(1);
    bTaken(0)=1;
    edges.push_back(oldedges(0));

    while(1)
    {
        bSucc = false;
        minID = -1;
        minminDis = std::numeric_limits<double>::max();
        for(i=0; i<n; ++i)
        {
            if(bTaken(i)!=0)
                continue;
            id = GetMatchTwoPntsPair(pnts(2*i), pnts(2*i+1), endPnts[0], endPnts[1], minDis, otherDis);
            if(minDis<minminDis)
            {
                id2 = id;
                minID = i;
                minminDis = minDis;
            }
        }
        if(minID!=-1 && minminDis<tol)
        {
            bTaken(minID) = 1;
            bSucc = true;
            if(id2==0)
            {
                edges.insert(edges.begin(), oldedges(minID));
                endPnts[0] = pnts(2*minID+1);
            }
            else if(id2==1)
            {
                edges.push_back(oldedges(minID));
                endPnts[1] = pnts(2*minID+1);
            }
            else if(id2==2)
            {
                edges.insert(edges.begin(), oldedges(minID));
                endPnts[0] = pnts(2*minID);
            }
            else if(id2==3)
            {
                edges.push_back(oldedges(minID));
                endPnts[1] = pnts(2*minID);
            }
            if(pMaxGap)
            {
                if(*pMaxGap<minminDis)
                    *pMaxGap = minminDis;
            }
        }
        if(!bSucc)
            break;
    }

    if(edges.size()!=oldedges.Length())
        return false;

    if(endPnts[1].Distance(endPnts[0])<tol) //close
    {
        if(pClosed)
            *pClosed = true;
        int startID=-1;
        for(i=0; i<edges.size(); ++i)
        {
            if(oldedges(0).IsSame(edges[i]))
            {
                startID = i;
                break;
            }
        }
        if(startID==-1)
        {
            for(i=0; i<edges.size(); ++i)
                newedges.Append(edges[i]);
        }
        else
        {
            for(i=startID; i<edges.size(); ++i)
                newedges.Append(edges[i]);
            for(i=0; i<startID; ++i)
                newedges.Append(edges[i]);
        }
    }
    else
    {
        if(pClosed)
            *pClosed = false;

        for(i=0; i<edges.size(); ++i)
            newedges.Append(edges[i]);
    }

    return true;
}





