//
// Created by byter on 4/6/18.
//

#include "VertexNormals.h"
#include <threepp/core/BufferAttribute.h>

namespace three {

struct LinearGeometryAccess
{
  static void update(helper::VertexNormals &helper, LinearGeometry::Ptr geometry, BufferAttributeT<float>::Ptr position)
  {
    const auto &vertices = geometry->_vertices;
    const auto &faces = geometry->_faces;

    unsigned idx = 0;

    for(unsigned i = 0, l = faces.size(); i < l; i ++ ) {

      const auto &face = faces[ i ];

      for ( unsigned j = 0, jl = face.vertexNormals.size(); j < jl; j ++ ) {

        int faceVertex = -1;
        switch(j) {
          case 0: faceVertex = face.a; break;
          case 1: faceVertex = face.b; break;
          case 2: faceVertex = face.c; break;
        }

        auto vertex = vertices[ faceVertex ];
        auto normal = face.vertexNormals[ j ];

        vertex.apply( helper._object->matrixWorld() );

        normal.apply( helper.normalMatrix ).normalize();
        normal *= helper._config.size;
        normal += vertex;

        position->setXYZ( idx, vertex.x(), vertex.y(), vertex.z() );

        idx = idx + 1;

        position->setXYZ( idx, normal.x(), normal.y(), normal.z() );

        idx = idx + 1;
      }
    }
    position->needsUpdate();
  }
};

struct BufferGeometryAccess
{
  static void update(helper::VertexNormals &helper, BufferGeometry::Ptr geometry, BufferAttributeT<float>::Ptr position)
  {
    const auto &objPos = geometry->position();
    const auto &objNorm = geometry->normal();

    unsigned idx = 0;

    // for simplicity, ignore index and drawcalls, and render every normal
    for ( unsigned j = 0, jl = objPos->itemCount(); j < jl; j ++ ) {

      math::Vector3 v1( objPos->get_x( j ), objPos->get_y( j ), objPos->get_z( j ) );
      v1.apply( helper._object->matrixWorld() );

      math::Vector3 v2( objNorm->get_x( j ), objNorm->get_y( j ), objNorm->get_z( j ) );
      v2.apply( helper.normalMatrix ).normalize();
      v2 *= helper._config.size;
      v2 += v1;

      position->setXYZ( idx, v1.x(), v1.y(), v1.z() );

      idx = idx + 1;

      position->setXYZ( idx, v2.x(), v2.y(), v2.z() );

      idx = idx + 1;

    }
    position->needsUpdate();
  }
};

namespace helper {

void VertexNormals::update()
{
  _object->updateMatrixWorld( true );

  normalMatrix = _object->matrixWorld().normalMatrix();

  LinearGeometry::Ptr linearGeometry = std::dynamic_pointer_cast<LinearGeometry>(_object->geometry());
  if ( linearGeometry ) {
    LinearGeometryAccess::update(*this, linearGeometry, _geometry->getPosition());
  }
  else {
    BufferGeometry::Ptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(_object->geometry());
    if(bufferGeometry)
      BufferGeometryAccess::update(*this, bufferGeometry, _geometry->getPosition());
  }
}

}
}
