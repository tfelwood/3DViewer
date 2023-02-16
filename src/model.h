#ifndef MODEL_H
#define MODEL_H


#include <QColor>

extern "C" {
    #include "parser.h"
    #include "carcass.h"
}

# define CONFIGURATION_FILE "./3DViewer.conf"



//struct model{
//    carcass_st  *crc;
//    QColor      facetColor;
//    QColor      vertexColor;
//    double      facetThickness;
//    double      vertexSize;
//    int         facetType; //what's type?
//    int         vertexType;  //what's type?
//    bool        projection;
//};

//struct position_set{
//    float scale_value;
//    double rotate_values[3];
//    double translate_values[3];
//    bool  projection;
//};

//typedef struct color_set{
//    float color_lines[4];
//    float color_background[4];
//    float color_vertexes[4];
//} t_color_set;

//typedef struct display_set{
//   float line_width;
//   float
//} t_display_set;

//typedef struct settings{
//    t_color_set clr;

//}

#endif // MODEL_H
