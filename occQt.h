/*
*    Copyright (c) 2013 to current year. All Rights Reserved.
*
*           File : occQt.cpp
*         Author : eryar@163.com
*           Date : 2014-11-29 10:18
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : OpenCASCADE in Qt.
*
*/

#ifndef OCCQT_H
#define OCCQT_H

#include "ui_occQt.h"

#include <AIS_InteractiveContext.hxx>
#include <QVector>

class OccView;
class QComboBox;

class GridDialog;
class GCodeDialog;
class ToolsDialog;
class TopoDS_Edge;

//! Qt main window which include OpenCASCADE for its central widget.
class occQt : public QMainWindow
{
    Q_OBJECT

public:
    //! constructor/destructor.
    occQt(QWidget *parent = 0);
    ~occQt();

    const OccView *GetView() const;
    const ToolsDialog *GetToolsDialog() const;
protected:
    //! create all the actions.
    void createActions(void);

    //! create all the menus.
    void createMenus(void);

    //! create the toolbar.
    void createToolBars(void);

    void createUIElements();

    // initialize the OpenCASCADE modeler.
    void InitializeModeler(void);

private slots:
    //! show about box.
    void about(void);

    //! make box test.
    void makeBox(void);

    //! make cone test.
    void makeCone(void);

    //! make sphere test.
    void makeSphere(void);

    //! make cylinder test.
    void makeCylinder(void);

    //! make torus test.
    void makeTorus(void);

    //! fillet test.
    void makeFillet(void);

    //! chamfer test.
    void makeChamfer(void);

    //! test extrude algorithm.
    void makeExtrude(void);

    //! test revol algorithm.
    void makeRevol(void);

    //! test loft algorithm.
    void makeLoft(void);

    //! test boolean operation cut.
    void testCut(void);

    //! test boolean operation fuse.
    void testFuse(void);

    //! test boolean operation common.
    void testCommon(void);

    void openIgesFile();
    void openDxfFile();
    void save();
    void open();
    void openRecentFile();



private:
    Ui::occQtClass ui;

private:
    //! the exit action.
    QAction* mExitAction;
    // the open iges file
    QAction *mOpenIgesAction;
    QAction *mOpenDxfAction;
    QAction *mSaveAction;
    QAction *mOpenAction;

    //! the actions for the view: pan, reset, fitall.
    QAction* mViewZoomAction;
    QAction* mViewPanAction;
    QAction* mViewRotateAction;
    QAction* mViewResetAction;
    QAction* mViewFitallAction;


    //! the actions to test the OpenCASCADE modeling algorithms.
    QAction* mMakeBoxAction;
    QAction* mMakeConeAction;
    QAction* mMakeSphereAction;
    QAction* mMakeCylinderAction;
    QAction* mMakeTorusAction;

    //! make a fillet box.
    QAction* mFilletAction;
    QAction* mChamferAction;
    QAction* mExtrudeAction;
    QAction* mRevolveAction;
    QAction* mLoftAction;

    //! boolean operations.
    QAction* mCutAction;
    QAction* mFuseAction;
    QAction* mCommonAction;

    //! show the about info action.
    QAction* mAboutAction;

    QAction *mGridDialogAction;
    QAction *mGCodeDialogAction;
    QAction *mToolsDialogAction;

    enum { MaxRecentFiles = 5 };
    QAction *m_recentFileActs[MaxRecentFiles];

    //! the menus of the application.
    QMenu* mFileMenu;
    QMenu *mEditMenu;
    QMenu* mViewMenu;
    QMenu* mPrimitiveMenu;
    QMenu* mModelingMenu;
    QMenu* mOperationMenu;
    QMenu* mHelpMenu;
    //QMenu *m_recentFilesMenu;

    //! the toolbars of the application.
    QToolBar* mViewToolBar;
    QToolBar* mEditToolBar;
    QToolBar* mNavigateToolBar;
    QToolBar* mPrimitiveToolBar;
    QToolBar* mModelingToolBar;
    QToolBar* mOperationToolBar;
    QToolBar* mHelpToolBar;

    QComboBox *mViewPlaneComboBox;
    // wrapped the widget for occ.
    OccView* mOccView;

    //! the interactive context.
    Handle_AIS_InteractiveContext mContext;

public:

  void draw(QVector <TopoDS_Shape *> path) const;
  void draw(const TopoDS_Shape &sh, Standard_Boolean updateViewer = Standard_True, Quantity_NameOfColor clr = Quantity_NOC_BLUE1) const;

private:
  GridDialog *mGridDialog;
  GCodeDialog *mGCodeDialog;
  ToolsDialog *mToolsDialog;

public slots:
   void ShowCoordinateOnStatusBar(const QString &message);
   void HideSelected();
   void ShowAll();

private:
    void createConnections();
    void loadDxfFile(const QString &file);
    void loadIgesFile(const QString &file);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
};

#endif // OCCQT_H
