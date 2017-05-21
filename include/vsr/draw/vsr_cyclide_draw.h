/*
 * License
 * ---------------------------------------------------------------------------
 *
 * Versor Geometric Algebra Library
 * Copyright (C) 2010  Pablo Colapinto
 * All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * =====================================================================================
 *
 *       Filename:  vsr_cyclide_draw.h
 *
 *    Description: temporary drawing routines for cyclides
 *
 *        Version:  1.0
 *        Created:  11/20/2015 18:09:09
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef  vsr_cyclide_draw_INC
#define  vsr_cyclide_draw_INC

#include <vsr/form/vsr_graph.h>
#include <vsr/form/vsr_cyclide.h>

#include <gfx/gfx_mesh.h>
#include <gfx/gfx_print.h>

#include <vsr/draw/vsr_cga3D_draw.h>

namespace gfx{

/*-----------------------------------------------------------------------------
 *  DATA
 *-----------------------------------------------------------------------------*/
using vsr::cga::CyclideQuad;
using vsr::cga::Round;
using vsr::cga::Vec;
using vsr::cga::Point;

struct CyclideMeshData{
  Vec pos;
  Vec normal;
};


/// Immediate Draw Routine of Cyclidic Patch, draws 4 Frames
template<> void Renderable<CyclideQuad>::DrawImmediate(const CyclideQuad& s){
  for (int i=0;i<4;++i){
     auto tx = -Round::dir(s.tframe[i].tan[0]).copy<Vec>().unit();
     auto ty = -Round::dir(s.tframe[i].tan[1]).copy<Vec>().unit();
     auto tz = -Round::dir(s.tframe[i].tan[2]).copy<Vec>().unit();

     DrawAt( tx, s.tframe[i].pos(), 1,0,0);
     DrawAt( ty, s.tframe[i].pos(), 0,1,0);
     DrawAt( tz, s.tframe[i].pos(), 0,0,1);
  }
}

/// Immeidate Draw of HalfEdge Graph of MeshData
template<>
   void Renderable<vsr::HEGraph<CyclideMeshData>,0>::DrawImmediate( const vsr::HEGraph<CyclideMeshData>& graph){
     glBegin(GL_TRIANGLES);
     for (auto& i : graph.face()){
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c();
          //glColor4f(.2,1,.2,.7);
          GL::normal( a.normal.begin() );
          GL::vertex( a.pos.begin() );
          GL::normal( b.normal.begin() );
          GL::vertex( b.pos.begin() );
          GL::normal( c.normal.begin() );
          GL::vertex( c.pos.begin() );
     }
     glEnd();
  }



struct MeshRender{

  template<class T>
  static void Draw(const vector<T>& mesh, int w, int h, float alpha=1){
    bool bColor = false;
    glBegin(GL_QUADS);
    for (int i =0;i<w-1;++i){
      for (int j =0;j<h-1; ++j){
          int a = i*h+j;
          int b = a+h;
          int c = b+1;
          int d = a+1;

          if (alpha < 1 ) glColor4f(bColor*.8,bColor,bColor,alpha);
          else glColor3f(bColor*.8,bColor,bColor);
          GL::normal(mesh[a].normal.begin());
          GL::vertex(mesh[a].pos.begin());
          GL::vertex(mesh[b].pos.begin());
          GL::vertex(mesh[c].pos.begin());
          GL::vertex(mesh[d].pos.begin());
          bColor = !bColor;
      }
      bColor = !bColor;
    }
    glEnd();

  }

  template<class T>
  static void DrawEdges(const vector<T>& mesh, int w, int h){
    bool bColor = false;
    glColor3f(0,1,1);
    for (int i =0;i<=w;++i){
       if ( i==0 || i==w ){

       glBegin(GL_LINE_STRIP);
        for (int j =0;j<=h; ++j){
           int a = i*(h+1)+j;
         //  GL::normal(mesh[a].normal.begin());
           GL::vertex(mesh[a].pos.begin());
        }
       glEnd();

      }
    }
    for (int j =0;j<=h;++j){
      if ( j==0 || j==h ){

       glBegin(GL_LINE_STRIP);
       for (int i =0;i<=w; ++i){

           int a = i*(h+1)+j;
       //  GL::normal(mesh[a].normal.begin());
           GL::vertex(mesh[a].pos.begin());
       }
       glEnd();
      }
    }


  }

  static void Draw(const vector<CyclideMeshData>& mesh, int w, int h, bool bNormals =false){

    bool bColor = false;
    glBegin(GL_QUADS);
    for (int i =0;i<w-1;++i){
      for (int j =0;j<h-1; ++j){
          int a = i*h+j;
          int b = a+h;
          int c = b+1;
          int d = a+1;

          glColor4f(bColor*.8,bColor,bColor,.5);
          GL::normal(mesh[a].normal.begin());
          GL::vertex(mesh[a].pos.begin());
          GL::vertex(mesh[b].pos.begin());
          GL::vertex(mesh[c].pos.begin());
          GL::vertex(mesh[d].pos.begin());
          bColor = !bColor;
      }
      bColor = !bColor;
    }
    glEnd();

    if (bNormals){
      glColor3f(0,1,0);
      glBegin(GL_LINES);
      for (int i=0;i<mesh.size();++i){
       //DrawBAt( mesh[i].normal, mesh[i].pos, 0, 1, 0 );
       GL::vertex((mesh[i].pos).begin()) ;
       GL::vertex((mesh[i].pos+mesh[i].normal).begin()) ;
      }
      glEnd();
    }
  }
};

/// Settings for Drawing Cyclidic Patches (i.e. whether to draw surface)
/// @todo generalize how to do this for objects
struct CyclideDraw{
  /// Half Edge Data Pointing to Vertices
  HEGraph<CyclideMeshData> graph;
  /// The Vertices themselves (positions and normals)
  vector<CyclideMeshData> mesh;

  int width = 0;
  int height = 0;

  bool bDrawCyclide = true;
  bool bDrawCircle = false;
  bool bDrawPlunge = false;
  bool bDrawSurface = true;
  bool bDrawNormals = false;

  void init(int w, int h){
    width =w ; height =h;
    mesh = vector<CyclideMeshData>( (width+1)*(height+1));
    graph.UV( width+1,height+1,mesh);
  }

  /// Draw a cylclidic patch using this data containers settings
  /// @param surf which patch ratio to draw(leave at 0)
   void draw(const CyclideQuad& cyclide, int surf=0){

     if (width==0) init(10,10);

      if (bDrawCyclide) Draw(cyclide);
      if (bDrawCircle) Draw( cyclide.circle(),0,1,0 );
      if (bDrawPlunge) {
        Draw( cyclide.tframe[2].sphere[2].dual() ^ cyclide.tframe[0].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[2].sphere[2].dual() ^ cyclide.tframe[1].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[1].sphere[2].dual() ^ cyclide.tframe[0].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[1].sphere[2].dual() ^ cyclide.tframe[3].tan[2], 1, 0, 1 );
      }

      if (bDrawSurface){
      for (int i =0;i<=width;i++){
       auto tu = (float)i/width;
       for (int j =0;j<=height;j++){
         auto tv = (float)j/height;

         Point np;
         vsr::cga::Pair tan;
         switch(surf){
          case 0:
            np = cyclide.eval(tu,tv);
            tan = cyclide.apply( cyclide.tframe[0].tan[2], tu,tv);
            break;
          case 1:
             np = cyclide.evalXZ(tu,tv);
             tan = cyclide.applyXZ( cyclide.tframe[0].tan[1], tu,tv);
             break;
          case 2:
             np = cyclide.evalYZ(tu,tv);
             tan = cyclide.applyYZ( cyclide.tframe[0].tan[0], tu,tv);
             break;
         }

         int idx = i*(height+1) + j;
         mesh[idx].pos = np;
         mesh[idx].normal = -Round::dir(tan).copy<Vec>().unit();
      }
      }
     }

     MeshRender::Draw(mesh,width+1,height+1,bDrawNormals);
   }
};


}


#endif   /* ----- #ifndef vsr_cyclide_draw_INC  ----- */
