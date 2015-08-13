#include "dxfreader.h"
#include "occQt.h"
#include <qdebug.h>

#include <iostream>
#include "basicheaders.h"

DXFReader::DXFReader(const occQt *mainClass)
            : m_mainClass(mainClass)
{

}


/**
 * Sample implementation of the method which handles layers.
 */
void DXFReader::addLayer(const DL_LayerData& data) {
    printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
    printAttributes();
}

/**
 * Sample implementation of the method which handles point entities.
 */
void DXFReader::addPoint(const DL_PointData& data) {
    printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
           data.x, data.y, data.z);
    printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 */
void DXFReader::addLine(const DL_LineData& data)
{
    // qDebug() << "addline";
    gp_Pnt p1(data.x1, data.y1, data.z1);
    gp_Pnt p2( data.x2, data.y2, data.z2);
    TopoDS_Shape line = BRepBuilderAPI_MakeEdge(p1, p2).Shape();
    m_mainClass->draw(line);

    /*printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
           data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
    printAttributes();
    */
}

/**
 * Sample implementation of the method which handles arc entities.
 */
void DXFReader::addArc(const DL_ArcData& data)
{
    gp_Pnt p1(data.cx, data.cy, data.cz);
    gp_Ax2 a2(p1, gp_Dir(0, 0, 1));
    gp_Circ circle(a2, data.radius);
    GC_MakeArcOfCircle geoArc(circle, data.angle1, data.angle2, true);

    TopoDS_Shape arc = BRepBuilderAPI_MakeEdge(geoArc.Value()).Shape();
    m_mainClass->draw(arc);
    /*printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
           data.cx, data.cy, data.cz,
           data.radius, data.angle1, data.angle2);*/
    printAttributes();
}

/**
 * Sample implementation of the method which handles circle entities.
 */
void DXFReader::addCircle(const DL_CircleData& data) {
     qDebug() << "addcircle";
    gp_Pnt p1(data.cx, data.cy, data.cz);
    gp_Ax2 a2(p1, gp_Dir());
    gp_Circ c(a2, data.radius);
    TopoDS_Shape circle = BRepBuilderAPI_MakeEdge(c).Shape();
    m_mainClass->draw(circle);

    /*printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
           data.cx, data.cy, data.cz,
           data.radius);

    printAttributes();*/
}


/**
 * Sample implementation of the method which handles polyline entities.
 */
void DXFReader::addPolyline(const DL_PolylineData& data) {

    printf("POLYLINE \n");
    printf("flags: %d\n", (int)data.flags);
    printAttributes();
}


/**
 * Sample implementation of the method which handles vertices.
 */
void DXFReader::addVertex(const DL_VertexData& data) {
    printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
           data.x, data.y, data.z,
           data.bulge);
    printAttributes();
}


void DXFReader::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i=0; i<4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
            i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}


void DXFReader::printAttributes() {
    printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());
    printf(" Color: ");
    if (attributes.getColor()==256)	{
        printf("BYLAYER");
    } else if (attributes.getColor()==0) {
        printf("BYBLOCK");
    } else {
        printf("%d", attributes.getColor());
    }
    printf(" Width: ");
    if (attributes.getWidth()==-1) {
        printf("BYLAYER");
    } else if (attributes.getWidth()==-2) {
        printf("BYBLOCK");
    } else if (attributes.getWidth()==-3) {
        printf("DEFAULT");
    } else {
        printf("%d", attributes.getWidth());
    }
    //printf(" Type: %s\n", attributes.getLineType().c_str());
}



// EOF
