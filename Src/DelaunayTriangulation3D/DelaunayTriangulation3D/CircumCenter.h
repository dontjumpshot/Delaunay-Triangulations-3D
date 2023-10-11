// http://www.mcs.anl.gov/~fathom/meshkit-docs/html/circumcenter_8cpp_source.html

#pragma once

//using namespace QM;
void TetCircumCenter( float *a, float *b, float *c, float *d, float *r, float *p);  //计算四面体的外接球圆心。
void TriCircumCenter2D(float *a, float *b, float *c, float *r, float *p);  //计算二维三角形的外接圆圆心。
void TriCircumCenter3D(float *a, float *b, float *c, float *r, float *p);  //计算三维三角形的外接圆圆心。
void TriCircumCenter3D(float *a, float *b, float *c, float *r);  //计算三维三角形的外接圆圆心。
