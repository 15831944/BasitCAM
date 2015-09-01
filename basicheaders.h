#ifndef BASICHEADERS
#define BASICHEADERS


#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <TColgp_Array2OfPnt.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepFill_PipeShell.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <AIS_ConnectedInteractive.hxx>
#include <gp_Trsf.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepFill_OffsetWire.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAdaptor_Curve2d.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLib.hxx>
#include <BRepCheck_Analyzer.hxx>

#include <AIS_Shape.hxx>

#include <IGESControl_Reader.hxx>
#include <ShapeSchema.hxx>
#include <FSD_File.hxx>
#include <PTColStd_TransientPersistentMap.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <PTopoDS_HShape.hxx>
#include <MgtBRep.hxx>
#include <gp_Pnt.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Lin.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom2d_Ellipse.hxx>
#include <GCE2d_MakeSegment.hxx>

#include <Handle_ShapeFix_Shape.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>

#include <NCollection_Vector.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <Handle_TopTools_HSequenceOfShape.hxx>
#define PI 3.1415926535897932385
#define DEGTORAD(x) (((x)*PI)/180.)
#define RADTODEG(x) (((x) * 180.) / PI)

#endif // BASICHEADERS

