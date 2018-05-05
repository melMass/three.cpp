//
// Created by byter on 29.07.17.
//

#ifndef THREEPP_MESH
#define THREEPP_MESH

#include <threepp/core/Object3D.h>
#include <threepp/material/Material.h>
#include <threepp/material/MeshBasicMaterial.h>
#include <threepp/util/simplesignal.h>

namespace three {

/**
 * a mesh without geometry and material
 */
class Mesh : public virtual Object3D
{
  DrawMode _drawMode;

  std::vector<float> _morphTargetInfluences;
  std::unordered_map<std::string, MorphTarget> _morphTargetDictionary;

protected:
  Mesh() : Object3D(), _drawMode(DrawMode::Triangles)
  {
    Object3D::typer = object::Typer(this);
  }

  Mesh(const Mesh &mesh) : Object3D(mesh)
  {
    _drawMode = mesh._drawMode;
    _morphTargetInfluences = mesh._morphTargetInfluences;
    _morphTargetDictionary = mesh._morphTargetDictionary;
    Object3D::typer = object::Typer(this);
  }

public:
  using Ptr = std::shared_ptr<Mesh>;

  template <typename Geom, typename Mat>
  static Ptr make(std::shared_ptr<Geom> geom, std::shared_ptr<Mat> mat);

  template <typename Geom, typename Mat>
  static Ptr make(const std::string &name, std::shared_ptr<Geom> geom, std::shared_ptr<Mat> mat);

  DrawMode drawMode() const {return _drawMode;}

  void setDrawMode(DrawMode mode) {_drawMode = mode;}

  bool isShadowRenderable() const override {return true;}

  bool frontFaceCW() const override
  {
    return _matrixWorld.determinant() < 0;
  }

  const std::vector<float> &morphTargetInfluences() const {return _morphTargetInfluences;}

  float morphTargetInfluence(unsigned index) const {return _morphTargetInfluences.at(index);}

  void raycast(const Raycaster &raycaster, std::vector<Intersection> &intersects) override;

  Mesh *cloned() const override {
    return new Mesh(*this);
  }
};

/**
 * mesh template with geometry and material
 *
 * @tparam Geom
 * @tparam Mat
 */
template <typename Geom, typename Mat>
class MeshT : public Mesh, public Object3D_GM<Geom, Mat>
{
  friend class Mesh;

protected:
  MeshT(const typename Geom::Ptr &geometry, typename Mat::Ptr material)
     : Mesh(), Object3D_GM<Geom, Mat>(geometry, material)
  {
    setDrawMode(DrawMode::Triangles);
  }

  MeshT(const MeshT &mesh)
     : Mesh(), Object3D_GM<Geom, Mat>(typename Geom::Ptr(mesh.geometry_t()->cloned()),
                              typename Mat::Ptr(mesh.template material<0>()->cloned()))
  {
    setDrawMode(DrawMode::Triangles);
  }

public:
  using Ptr = std::shared_ptr<MeshT<Geom, Mat>>;

  static Ptr make(const typename Geom::Ptr &geometry, const std::shared_ptr<Mat> &material)
  {
    return Ptr(new MeshT(geometry, material));
  }

  static Ptr make(std::string name, const typename Geom::Ptr &geometry, const std::shared_ptr<Mat> &material)
  {
    Ptr p(new MeshT(geometry, material));
    p->_name = name;
    return p;
  }

  MeshT *cloned() const override {
    return new MeshT(*this);
  }
};

template <typename Geom, typename Mat>
Mesh::Ptr Mesh::make(std::shared_ptr<Geom> geom, std::shared_ptr<Mat> mat) {
  return Ptr(new MeshT<Geom, Mat>(geom, mat));
}

template <typename Geom, typename Mat>
Mesh::Ptr Mesh::make(const std::string &name, std::shared_ptr<Geom> geom, std::shared_ptr<Mat> mat) {
  auto ptr = Ptr(new MeshT<Geom, Mat>(geom, mat));
  ptr->setName(name);
  return ptr;
}

}


#endif //THREEPP_MESH
