#ifndef DXFREADER_H
#define DXFREADER_H

#include <QVector>
#include "DXFHeaders/dl_creationadapter.h"
#include "linesorarcswithvertex.h"

class LineOrArc;
class occQt;
class DXFReader : public DL_CreationAdapter {
public:
    DXFReader(const occQt *mainClass);

    virtual void addLayer(const DL_LayerData& data);
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void add3dFace(const DL_3dFaceData& data);

    virtual void endEntity();

    void printAttributes();

private:
    const occQt *m_mainClass;
    LinesOrArcsWithVertex m_polyline;
};



#endif // DXFREADER_H
