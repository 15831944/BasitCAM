/*
*    Copyright (c) 2014 eryar All Rights Reserved.
*
*           File : occQt.cpp
*         Author : eryar@163.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : Qt main window for OpenCASCADE.
*/

#include "occQt.h"
#include "occView.h"
#include "gridDialog.h"
#include "gcodedialog.h"
#include "toolsdialog.h"

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QDebug>
#include <QComboBox>

#include "basicheaders.h"

#include "DXFHeaders/dl_dxf.h"
#include "DXFHeaders/dl_creationadapter.h"
#include "dxfreader.h"
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_BezierSurface.hxx>
#include <Handle_Geom_BezierSurface.hxx>
#include <AIS_MultipleConnectedInteractive.hxx>

occQt::occQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("BasitCAM");
    mOccView = new OccView(this);

    mContext = mOccView->getContext();

    setCentralWidget(mOccView);

    createUIElements();
    createActions();
    createMenus();
    createToolBars();
    createConnections();


    mModelingToolBar->hide();
    mPrimitiveToolBar->hide();





}

occQt::~occQt()
{

}
void occQt::createConnections()
{
    connect(mOccView, SIGNAL(MessageOfCoordinate(QString)), this, SLOT(ShowCoordinateOnStatusBar(QString)));
    connect(mToolsDialog, SIGNAL(ChangeToolList(const QStringListModel *)), mGCodeDialog, SLOT(ChangedTools(const QStringListModel*)));
}

void occQt::createActions( void )
{
    mExitAction = new QAction(tr("Exit"), this);
    mExitAction->setShortcut(tr("Ctrl+Q"));
    mExitAction->setIcon(QIcon(":/Resources/close.png"));
    mExitAction->setStatusTip(tr("Exit the application"));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

    mOpenAction = new QAction(tr("Open"), this);
    mOpenAction->setShortcut(tr("Ctrl+O"));
    mOpenAction->setStatusTip(tr("Open File"));
    connect(mOpenAction, SIGNAL(triggered(bool)), this, SLOT(open()));

    mSaveAction = new QAction(tr("Save"), this);
    mSaveAction->setShortcut(tr("Ctrl+S"));
    mSaveAction->setStatusTip(tr("Save File"));
    connect(mSaveAction, SIGNAL(triggered(bool)), this, SLOT(save()));

    mOpenIgesAction = new QAction(tr("Open Iges"), this);
    mOpenIgesAction->setStatusTip(tr("Open Iges File"));
    connect(mOpenIgesAction, SIGNAL(triggered(bool)), this, SLOT(openIgesFile()));

    mOpenDxfAction = new QAction(tr("Open Dxf"), this);
    mOpenDxfAction->setStatusTip(tr("Open Dxf File"));
    connect(mOpenDxfAction, SIGNAL(triggered(bool)), this, SLOT(openDxfFile()));

    mViewZoomAction = new QAction(tr("Zoom"), this);
    mViewZoomAction->setIcon(QIcon(":/Resources/Zoom.png"));
    mViewZoomAction->setStatusTip(tr("Zoom the view"));
    connect(mViewZoomAction, SIGNAL(triggered()), mOccView, SLOT(zoom()));

    mViewPanAction = new QAction(tr("Pan"), this);
    mViewPanAction->setIcon(QIcon(":/Resources/Pan.png"));
    mViewPanAction->setStatusTip(tr("Pan the view"));
    connect(mViewPanAction, SIGNAL(triggered()), mOccView, SLOT(pan()));

    mViewRotateAction = new QAction(tr("Rotate"), this);
    mViewRotateAction->setIcon(QIcon(":/Resources/Rotate.png"));
    mViewRotateAction->setStatusTip(tr("Rotate the view"));
    connect(mViewRotateAction, SIGNAL(triggered()), mOccView, SLOT(rotate()));

    mViewResetAction = new QAction(tr("Reset"), this);
    mViewResetAction->setIcon(QIcon(":/Resources/Home.png"));
    mViewResetAction->setStatusTip(tr("Reset the view"));
    connect(mViewResetAction, SIGNAL(triggered()), mOccView, SLOT(reset()));

    mViewFitallAction = new QAction(tr("Fit All"), this);
    mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
    mViewFitallAction->setStatusTip(tr("Fit all "));
    connect(mViewFitallAction, SIGNAL(triggered()), mOccView, SLOT(fitAll()));

    mMakeBoxAction = new QAction(tr("Box"), this);
    mMakeBoxAction->setIcon(QIcon(":/Resources/box.png"));
    mMakeBoxAction->setStatusTip(tr("Make a box"));
    connect(mMakeBoxAction, SIGNAL(triggered()), this, SLOT(makeBox()));

    mMakeConeAction = new QAction(tr("Cone"), this);
    mMakeConeAction->setIcon(QIcon(":/Resources/cone.png"));
    mMakeConeAction->setStatusTip(tr("Make a cone"));
    connect(mMakeConeAction, SIGNAL(triggered()), this, SLOT(makeCone()));

    mMakeSphereAction = new QAction(tr("Sphere"), this);
    mMakeSphereAction->setStatusTip(tr("Make a sphere"));
    mMakeSphereAction->setIcon(QIcon(":/Resources/sphere.png"));
    connect(mMakeSphereAction, SIGNAL(triggered()), this, SLOT(makeSphere()));

    mMakeCylinderAction = new QAction(tr("Cylinder"), this);
    mMakeCylinderAction->setStatusTip(tr("Make a cylinder"));
    mMakeCylinderAction->setIcon(QIcon(":/Resources/cylinder.png"));
    connect(mMakeCylinderAction, SIGNAL(triggered()), this, SLOT(makeCylinder()));

    mMakeTorusAction = new QAction(tr("Torus"), this);
    mMakeTorusAction->setStatusTip(tr("Make a torus"));
    mMakeTorusAction->setIcon(QIcon(":/Resources/torus.png"));
    connect(mMakeTorusAction, SIGNAL(triggered()), this, SLOT(makeTorus()));

    mFilletAction = new QAction(tr("Fillet"), this);
    mFilletAction->setIcon(QIcon(":/Resources/fillet.png"));
    mFilletAction->setStatusTip(tr("Test Fillet algorithm"));
    connect(mFilletAction, SIGNAL(triggered()), this, SLOT(makeFillet()));

    mChamferAction = new QAction(tr("Chamfer"), this);
    mChamferAction->setIcon(QIcon(":/Resources/chamfer.png"));
    mChamferAction->setStatusTip(tr("Test chamfer algorithm"));
    connect(mChamferAction, SIGNAL(triggered()), this, SLOT(makeChamfer()));

    mExtrudeAction = new QAction(tr("Extrude"), this);
    mExtrudeAction->setIcon(QIcon(":/Resources/extrude.png"));
    mExtrudeAction->setStatusTip(tr("Test extrude algorithm"));
    connect(mExtrudeAction, SIGNAL(triggered()), this, SLOT(makeExtrude()));

    mRevolveAction = new QAction(tr("Revolve"), this);
    mRevolveAction->setIcon(QIcon(":/Resources/revolve.png"));
    mRevolveAction->setStatusTip(tr("Test revol algorithm"));
    connect(mRevolveAction, SIGNAL(triggered()), this, SLOT(makeRevol()));

    mLoftAction = new QAction(tr("Loft"), this);
    mLoftAction->setIcon(QIcon(":/Resources/loft.png"));
    mLoftAction->setStatusTip(tr("Test loft algorithm"));
    connect(mLoftAction, SIGNAL(triggered()), this, SLOT(makeLoft()));

    mCutAction = new QAction(tr("Cut"), this);
    mCutAction->setIcon(QIcon(":/Resources/cut.png"));
    mCutAction->setStatusTip(tr("Boolean operation cut"));
    connect(mCutAction, SIGNAL(triggered()), this, SLOT(testCut()));

    mFuseAction = new QAction(tr("Fuse"), this);
    mFuseAction->setIcon(QIcon(":/Resources/fuse.png"));
    mFuseAction->setStatusTip(tr("Boolean operation fuse"));
    connect(mFuseAction, SIGNAL(triggered()), this, SLOT(testFuse()));

    mCommonAction = new QAction(tr("Common"), this);
    mCommonAction->setIcon(QIcon(":/Resources/common.png"));
    mCommonAction->setStatusTip(tr("Boolean operation common"));
    connect(mCommonAction, SIGNAL(triggered()), this, SLOT(testCommon()));

    mAboutAction = new QAction(tr("About"), this);
    mAboutAction->setStatusTip(tr("About the application"));
    mAboutAction->setIcon(QIcon(":/Resources/lamp.png"));
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));


    mGridDialogAction = new QAction(tr("Grid"), this);
    mGridDialogAction->setStatusTip(tr("Grid Dialog"));
    mGridDialogAction->setIcon(QIcon(":/Resources/grid.png"));
    connect(mGridDialogAction, SIGNAL(triggered()), mGridDialog, SLOT(open()));

    mGCodeDialogAction = new QAction(tr("G Code"), this);
    mGCodeDialogAction->setStatusTip(tr("G Code Dialog"));
    mGCodeDialogAction->setIcon(QIcon(":/Resources/gcode.png"));
    connect(mGCodeDialogAction, SIGNAL(triggered()), mGCodeDialog, SLOT(open()));

    mToolsDialogAction = new QAction(tr("Tools"), this);
    mToolsDialogAction->setStatusTip(tr("Tools Dialog"));
    mToolsDialogAction->setIcon(QIcon(":/Resources/tools.png"));
    connect(mToolsDialogAction, SIGNAL(triggered()), mToolsDialog, SLOT(open()));
}

void occQt::createMenus( void )
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mOpenAction);
    mFileMenu->addAction(mSaveAction);
    mFileMenu->addAction(mOpenIgesAction);
    mFileMenu->addAction(mOpenDxfAction);
    mFileMenu->addAction(mExitAction);

    mEditMenu = menuBar()->addMenu(tr("&Edit"));
    mEditMenu->addAction(mGridDialogAction);

    mViewMenu = menuBar()->addMenu(tr("&View"));
    mViewMenu->addAction(mViewZoomAction);
    mViewMenu->addAction(mViewPanAction);
    mViewMenu->addAction(mViewRotateAction);
    mViewMenu->addSeparator();
    mViewMenu->addAction(mViewResetAction);
    mViewMenu->addAction(mViewFitallAction);

//    mPrimitiveMenu = menuBar()->addMenu(tr("&Primitive"));
//    mPrimitiveMenu->addAction(mMakeBoxAction);
//    mPrimitiveMenu->addAction(mMakeConeAction);
//    mPrimitiveMenu->addAction(mMakeSphereAction);
//    mPrimitiveMenu->addAction(mMakeCylinderAction);
//    mPrimitiveMenu->addAction(mMakeTorusAction);

//    mModelingMenu = menuBar()->addMenu(tr("&Modeling"));
//    mModelingMenu->addAction(mFilletAction);
//    mModelingMenu->addAction(mChamferAction);
//    mModelingMenu->addAction(mExtrudeAction);
//    mModelingMenu->addAction(mRevolveAction);
//    mModelingMenu->addAction(mLoftAction);
//    mModelingMenu->addSeparator();
//    mModelingMenu->addAction(mCutAction);
//    mModelingMenu->addAction(mFuseAction);
//    mModelingMenu->addAction(mCommonAction);

    mOperationMenu = menuBar()->addMenu(tr("&Operations"));
    mOperationMenu->addAction(mGCodeDialogAction);
    mOperationMenu->addAction(mToolsDialogAction);

    mHelpMenu = menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mAboutAction);
}

void occQt::createToolBars( void )
{
    mNavigateToolBar = addToolBar(tr("&Navigate"));
    mNavigateToolBar->addAction(mViewZoomAction);
    mNavigateToolBar->addAction(mViewPanAction);
    mNavigateToolBar->addAction(mViewRotateAction);
    mNavigateToolBar->addWidget(mViewPlaneComboBox);

    mViewToolBar = addToolBar(tr("&View"));
    mViewToolBar->addAction(mViewResetAction);
    mViewToolBar->addAction(mViewFitallAction);

    mPrimitiveToolBar = addToolBar(tr("&Primitive"));
    mPrimitiveToolBar->addAction(mMakeBoxAction);
    mPrimitiveToolBar->addAction(mMakeConeAction);
    mPrimitiveToolBar->addAction(mMakeSphereAction);
    mPrimitiveToolBar->addAction(mMakeCylinderAction);
    mPrimitiveToolBar->addAction(mMakeTorusAction);

    mModelingToolBar = addToolBar(tr("&Modeling"));
    mModelingToolBar->addAction(mFilletAction);
    mModelingToolBar->addAction(mChamferAction);
    mModelingToolBar->addAction(mExtrudeAction);
    mModelingToolBar->addAction(mRevolveAction);
    mModelingToolBar->addAction(mLoftAction);
    mModelingToolBar->addSeparator();
    mModelingToolBar->addAction(mCutAction);
    mModelingToolBar->addAction(mFuseAction);
    mModelingToolBar->addAction(mCommonAction);

    mEditToolBar = addToolBar(tr("&Edit"));
    mEditToolBar->addAction(mGridDialogAction);

    mOperationToolBar = addToolBar(tr("&Operations"));
    mOperationToolBar->addAction(mGCodeDialogAction);
    mOperationToolBar->addAction(mToolsDialogAction);


    mHelpToolBar = addToolBar(tr("Help"));
    mHelpToolBar->addAction(mAboutAction);

}

void occQt::createUIElements()
{
    mGridDialog = new GridDialog(mOccView, this);
    mToolsDialog = new ToolsDialog(this);
    mGCodeDialog = new GCodeDialog(mOccView, this);
    mGCodeDialog->ChangedTools(mToolsDialog->GetModelList());

    mViewPlaneComboBox = new QComboBox(this);

    mViewPlaneComboBox->addItem("XY Plane");
    mViewPlaneComboBox->addItem("XZ Plane");
    mViewPlaneComboBox->addItem("YZ Plane");
    connect(mViewPlaneComboBox, SIGNAL(currentIndexChanged(int)), mOccView, SLOT(changeViewingPlane(int)));
    mOccView->changeViewingPlane(0);
}

void occQt::about()
{
    QMessageBox::about(this, tr("About BasitCAM"),
        tr("<h2>BasitCAM 1.0</h2>"
        "<p>Copyright &copy; 2015 suleymanturkoglu_@hotmail.com"
        "<p>BasitCAM is a demo applicaton about Qt and OpenCASCADE."));
}

void occQt::makeBox()
{
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();
    Handle_AIS_Shape anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);

    mContext->Display(anAisBox);
}

void occQt::makeCone()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
    Handle_AIS_Shape anAisReducer = new AIS_Shape(aTopoReducer);

    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
    Handle_AIS_Shape anAisCone = new AIS_Shape(aTopoCone);

    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    mContext->Display(anAisReducer);
    mContext->Display(anAisCone);
}

void occQt::makeSphere()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0));

    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();
    Handle_AIS_Shape anAisSphere = new AIS_Shape(aTopoSphere);

    anAisSphere->SetColor(Quantity_NOC_BLUE1);

    mContext->Display(anAisSphere);
}

// bu fonksiyonu test olarak kullanıyorum öğrenme amaçlı
void occQt::makeCylinder()
{




    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 30.0, 0.0));

    TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0).Shape();
    Handle_AIS_Shape anAisCylinder = new AIS_Shape(aTopoCylinder);

    anAisCylinder->SetColor(Quantity_NOC_RED);

    anAxis.SetLocation(gp_Pnt(8.0, 30.0, 0.0));
    TopoDS_Shape aTopoPie = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0, M_PI_2 * 3.0).Shape();
    Handle_AIS_Shape anAisPie = new AIS_Shape(aTopoPie);

    anAisPie->SetColor(Quantity_NOC_TAN);

  // mContext->Display(anAisCylinder);
  // mContext->Display(anAisPie);

   gp_Trsf location;
    location.SetTranslation(gp_Vec(200.0,0.0,0.0));
   /* Handle_AIS_ConnectedInteractive r = new AIS_ConnectedInteractive;
    r->Connect (anAisCylinder, location);

    mContext->Display(r);
*/
    Handle_AIS_MultipleConnectedInteractive anAssebly = new AIS_MultipleConnectedInteractive;
    anAssebly->Connect(anAisCylinder);
    anAssebly->Connect(anAisPie, location);
    mContext->Display(anAssebly);
}
/*
static Handle_AIS_InteractiveObject Transform ( const Handle_AIS_InteractiveObject& theObject,
            const ModelPrs_InteractiveObjectFactoryHelper::ObjectCollectionType& theAncestors)
{
    if (theObject.IsNull())
       return theObject; //AIS_ConnectedInteractive not always checks that it is not null

    TopLoc_Location aLoc;
    ModelPrs_InteractiveObjectFactoryHelper::CombineTransformations (theAncestors, aLoc);
    if (aLoc.IsIdentity())
        return theObject;
    Handle_AIS_ConnectedInteractive r = new AIS_ConnectedInteractive;
    r->Connect (theObject, aLoc);
    return r;
}
    */
/*
// For AIS_MultipleConnected:

void RepresentationVisitor::Append (const Handle_AIS_InteractiveObject& theObject)
{
    if (myResult.IsNull())
        myResult = theObject;
    else {
        if (myResult->IsKind (STANDARD_TYPE (AIS_MultipleConnectedInteractive))) {
            __CADEX_DEFINE_HANDLE_BY_STATIC_CAST(AIS_MultipleConnectedInteractive, aM, myResult);
            aM->Connect (theObject);
        }
        else {
            Handle_AIS_MultipleConnectedInteractive aM = new AIS_MultipleConnectedInteractive;
            aM->Connect (myResult);
            aM->Connect (theObject);
            myResult = aM;
        }
    }
}
*/
void occQt::makeTorus()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));

    TopoDS_Shape aTopoTorus = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
    Handle_AIS_Shape anAisTorus = new AIS_Shape(aTopoTorus);

    anAisTorus->SetColor(Quantity_NOC_YELLOW);

    anAxis.SetLocation(gp_Pnt(8.0, 40.0, 0.0));
    TopoDS_Shape aTopoElbow = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0, M_PI_2).Shape();
    Handle_AIS_Shape anAisElbow = new AIS_Shape(aTopoElbow);

    anAisElbow->SetColor(Quantity_NOC_THISTLE);

    mContext->Display(anAisTorus);
    mContext->Display(anAisElbow);
}

void occQt::makeFillet()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeFillet MF(aTopoBox);

    // Add all the edges to fillet.
    for (TopExp_Explorer ex(aTopoBox, TopAbs_EDGE); ex.More(); ex.Next())
    {
        MF.Add(1.0, TopoDS::Edge(ex.Current()));
    }

    Handle_AIS_Shape anAisShape = new AIS_Shape(MF.Shape());
    anAisShape->SetColor(Quantity_NOC_VIOLET);

    mContext->Display(anAisShape);
}

void occQt::makeChamfer()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(8.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeChamfer MC(aTopoBox);
    TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;

    TopExp::MapShapesAndAncestors(aTopoBox, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);

    for (Standard_Integer i = 1; i <= aEdgeFaceMap.Extent(); ++i)
    {
        TopoDS_Edge anEdge = TopoDS::Edge(aEdgeFaceMap.FindKey(i));
        TopoDS_Face aFace = TopoDS::Face(aEdgeFaceMap.FindFromIndex(i).First());

        MC.Add(0.6, 0.6, anEdge, aFace);
    }

    Handle_AIS_Shape anAisShape = new AIS_Shape(MC.Shape());
    anAisShape->SetColor(Quantity_NOC_TOMATO);

    mContext->Display(anAisShape);
}

void occQt::makeExtrude()
{
    // prism a vertex result is an edge.
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(0.0, 60.0, 0.0));
    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, 5.0));
    Handle_AIS_Shape anAisPrismVertex = new AIS_Shape(aPrismVertex);

    // prism an edge result is a face.
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(5.0, 60.0, 0.0), gp_Pnt(10.0, 60.0, 0.0));
    TopoDS_Shape aPrismEdge = BRepPrimAPI_MakePrism(anEdge, gp_Vec(0.0, 0.0, 5.0));
    Handle_AIS_Shape anAisPrismEdge = new AIS_Shape(aPrismEdge);

    // prism a wire result is a shell.
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(16.0, 60.0, 0.0));

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(anAxis, 3.0));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aPrismCircle = BRepPrimAPI_MakePrism(aCircleWire, gp_Vec(0.0, 0.0, 5.0));
    Handle_AIS_Shape anAisPrismCircle = new AIS_Shape(aPrismCircle);

    // prism a face or a shell result is a solid.
    anAxis.SetLocation(gp_Pnt(24.0, 60.0, 0.0));
    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(anAxis, 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aPrismEllipse = BRepPrimAPI_MakePrism(aEllipseFace, gp_Vec(0.0, 0.0, 5.0));
    Handle_AIS_Shape anAisPrismEllipse = new AIS_Shape(aPrismEllipse);

    anAisPrismVertex->SetColor(Quantity_NOC_PAPAYAWHIP);
    anAisPrismEdge->SetColor(Quantity_NOC_PEACHPUFF);
    anAisPrismCircle->SetColor(Quantity_NOC_PERU);
    anAisPrismEllipse->SetColor(Quantity_NOC_PINK);

    mContext->Display(anAisPrismVertex);
    mContext->Display(anAisPrismEdge);
    mContext->Display(anAisPrismCircle);
    mContext->Display(anAisPrismEllipse);
}

void occQt::makeRevol()
{
    gp_Ax1 anAxis;

    // revol a vertex result is an edge.
    anAxis.SetLocation(gp_Pnt(0.0, 70.0, 0.0));
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(2.0, 70.0, 0.0));
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis);
    Handle_AIS_Shape anAisRevolVertex = new AIS_Shape(aRevolVertex);

    // revol an edge result is a face.
    anAxis.SetLocation(gp_Pnt(8.0, 70.0, 0.0));
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(6.0, 70.0, 0.0), gp_Pnt(6.0, 70.0, 5.0));
    TopoDS_Shape aRevolEdge = BRepPrimAPI_MakeRevol(anEdge, anAxis);
    Handle_AIS_Shape anAisRevolEdge = new AIS_Shape(aRevolEdge);

    // revol a wire result is a shell.
    anAxis.SetLocation(gp_Pnt(20.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(15.0, 70.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aRevolCircle = BRepPrimAPI_MakeRevol(aCircleWire, anAxis, M_PI_2);
    Handle_AIS_Shape anAisRevolCircle = new AIS_Shape(aRevolCircle);

    // revol a face result is a solid.
    anAxis.SetLocation(gp_Pnt(30.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(gp_Ax2(gp_Pnt(25.0, 70.0, 0.0), gp::DZ()), 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aRevolEllipse = BRepPrimAPI_MakeRevol(aEllipseFace, anAxis, M_PI_4);
    Handle_AIS_Shape anAisRevolEllipse = new AIS_Shape(aRevolEllipse);

    anAisRevolVertex->SetColor(Quantity_NOC_LIMEGREEN);
    anAisRevolEdge->SetColor(Quantity_NOC_LINEN);
    anAisRevolCircle->SetColor(Quantity_NOC_MAGENTA1);
    anAisRevolEllipse->SetColor(Quantity_NOC_MAROON);

    mContext->Display(anAisRevolVertex);
    mContext->Display(anAisRevolEdge);
    mContext->Display(anAisRevolCircle);
    mContext->Display(anAisRevolEllipse);
}

void occQt::makeLoft()
{
    // bottom wire.
    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(0.0, 80.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);

    // top wire.
    BRepBuilderAPI_MakePolygon aPolygon;
    aPolygon.Add(gp_Pnt(-3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 83.0, 6.0));
    aPolygon.Add(gp_Pnt(-3.0, 83.0, 6.0));
    aPolygon.Close();

    BRepOffsetAPI_ThruSections aShellGenerator;
    BRepOffsetAPI_ThruSections aSolidGenerator(true);

    aShellGenerator.AddWire(aCircleWire);
    aShellGenerator.AddWire(aPolygon.Wire());

    aSolidGenerator.AddWire(aCircleWire);
    aSolidGenerator.AddWire(aPolygon.Wire());

    // translate the solid.
    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aSolidGenerator.Shape(), aTrsf);

    Handle_AIS_Shape anAisShell = new AIS_Shape(aShellGenerator.Shape());
    Handle_AIS_Shape anAisSolid = new AIS_Shape(aTransform.Shape());

    anAisShell->SetColor(Quantity_NOC_OLIVEDRAB);
    anAisSolid->SetColor(Quantity_NOC_PEACHPUFF);

    mContext->Display(anAisShell);
    mContext->Display(anAisSolid);
}

void occQt::testCut()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 90.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCuttedShape1 = BRepAlgoAPI_Cut(aTopoBox, aTopoSphere);
    TopoDS_Shape aCuttedShape2 = BRepAlgoAPI_Cut(aTopoSphere, aTopoBox);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform1(aCuttedShape1, aTrsf);

    aTrsf.SetTranslation(gp_Vec(16.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform2(aCuttedShape2, aTrsf);

    Handle_AIS_Shape anAisBox = new AIS_Shape(aTopoBox);
    Handle_AIS_Shape anAisSphere = new AIS_Shape(aTopoSphere);
    Handle_AIS_Shape anAisCuttedShape1 = new AIS_Shape(aTransform1.Shape());
    Handle_AIS_Shape anAisCuttedShape2 = new AIS_Shape(aTransform2.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCuttedShape1->SetColor(Quantity_NOC_TAN);
    anAisCuttedShape2->SetColor(Quantity_NOC_SALMON);

    mContext->Display(anAisBox);
    mContext->Display(anAisSphere);
    mContext->Display(anAisCuttedShape1);
    mContext->Display(anAisCuttedShape2);
}

void occQt::testFuse()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 100.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aFusedShape = BRepAlgoAPI_Fuse(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aFusedShape, aTrsf);

    Handle_AIS_Shape anAisBox = new AIS_Shape(aTopoBox);
    Handle_AIS_Shape anAisSphere = new AIS_Shape(aTopoSphere);
    Handle_AIS_Shape anAisFusedShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisFusedShape->SetColor(Quantity_NOC_ROSYBROWN);

    mContext->Display(anAisBox);
    mContext->Display(anAisSphere);
    mContext->Display(anAisFusedShape);
}

void occQt::testCommon()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 110.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCommonShape = BRepAlgoAPI_Common(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aCommonShape, aTrsf);

    Handle_AIS_Shape anAisBox = new AIS_Shape(aTopoBox);
    Handle_AIS_Shape anAisSphere = new AIS_Shape(aTopoSphere);
    Handle_AIS_Shape anAisCommonShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCommonShape->SetColor(Quantity_NOC_ROYALBLUE);

    mContext->Display(anAisBox);
    mContext->Display(anAisSphere);
    mContext->Display(anAisCommonShape);
}

void occQt::openIgesFile()
{
    QString str = QFileDialog::getOpenFileName(this, "Open Iges File", "", "*.igs");
    if (str.isEmpty())
        return;
    qDebug() << str;

    IGESControl_Reader reader;
    reader.ReadFile(str.toStdString().c_str());
    reader.PrintCheckLoad(false, IFSelect_GeneralInfo );


    Standard_Integer nIgesFaces,nTransFaces;

    //loads file MyFile.igs
    Handle(TColStd_HSequenceOfTransient) myList =  reader.GiveList("iges-faces");
    //selects all IGES faces in the file and puts them into a list  called //MyList,
   // nIgesFaces = myList.Length();
    nTransFaces = reader.TransferList(myList);
    //translates MyList,
    qDebug() <<"   Transferred:"<<nTransFaces<<endl;
    TopoDS_Shape sh = reader.OneShape();
    //and obtains the results in an OCCT shape.

    Handle_AIS_Shape aisShape = new AIS_Shape(sh);
   // aisShape->SetColor(Quantity_NOC_STEELBLUE);
    mContext->Display(aisShape);
}

void occQt::openDxfFile()
{
    QString str = QFileDialog::getOpenFileName(this, "Open Iges File", "", "*.dxf");
    if (str.isEmpty())
        return;
    DXFReader *creationClas = new DXFReader(this);
    DL_Dxf *dxf = new DL_Dxf();
    if (!dxf->in(str.toStdString().c_str(), creationClas)) {
        qDebug() << "could not be opened";
        return;
    }
    delete dxf;
    delete creationClas;
}

void occQt::open()
{
    QString str = QFileDialog::getOpenFileName(this, "Open TRK File", "", "*.trk");
    if (str.isEmpty())
        return;

    FSD_File f;
    f.Open(str.toStdString().c_str(), Storage_VSRead);

    Handle(ShapeSchema) s = new ShapeSchema;

    Handle(Storage_Data)  d = s->Read(f);

    Handle(Storage_HSeqOfRoot)  roots = d->Roots();

    Handle(Standard_Persistent)  p;

    Handle(PTopoDS_HShape) aPShape;
    aPShape =  Handle(PTopoDS_HShape)::DownCast(p);

    TopoDS_Shape aShape;
    PTColStd_PersistentTransientMap aMap;
    qDebug() << "14";
    MgtBRep::Translate (aPShape, aMap,  aShape, MgtBRep_WithoutTriangle);
qDebug() << "15";


    Handle_AIS_Shape aisShape = new AIS_Shape(aShape);
   // aisShape->SetColor(Quantity_NOC_STEELBLUE);
    mContext->Display(aisShape);
}

void occQt::save()
{
    QString str = QFileDialog::getSaveFileName(this, "Save File", "", "*.trk");
    if (str.isEmpty())
        return;

    str += ".trk";

    Handle(ShapeSchema) s = new ShapeSchema;

    TopoDS_Shape  aShape;
    PTColStd_TransientPersistentMap  aMap;
    Handle(PTopoDS_HShape)  aPShape = MgtBRep::Translate(aShape, aMap, MgtBRep_WithoutTriangle);

    Handle(Storage_Data) d = new Storage_Data;
    d ->AddRoot("ObjectName", aPShape);

    FSD_File f;

    f.Open(str.toStdString().c_str(), Storage_VSWrite);
    s->Write(f,d);
}

void occQt::draw(const TopoDS_Shape &sh) const
{
    Handle_AIS_Shape hSh = new AIS_Shape(sh);
    mContext->Display(hSh);




}

 void occQt::ShowCoordinateOnStatusBar(const QString &message)
 {
     statusBar()->showMessage(message);
 }
