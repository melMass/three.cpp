//
// Created by byter on 24.09.17.
//

#ifndef THREE_QT_RESOLVER_H
#define THREE_QT_RESOLVER_H

#if 0
#include <light/AmbientLight.h>
#include <light/DirectionalLight.h>
#include <light/HemisphereLight.h>
#include <light/PointLight.h>
#include <light/RectAreaLight.h>
#include <light/SpotLight.h>
#endif

#include <memory>
#include <functional>

#define DEF_RESOLVER() \
struct ResolverBase  { \
  using Ptr = std::shared_ptr<ResolverBase>; \
  virtual void call(Functions &functions) const {} \
  virtual void call(Functions &functions, unsigned index) const {} \
}; \
template <typename L> \
class Resolver {};

#define MK_RESOLVER(Clz, func) \
template <> \
struct Resolver<Clz> : public ResolverBase \
{ \
  Clz &target; \
  Resolver(Clz &target) : target(target) {} \
  static Ptr make(Clz &target) {return Ptr(new Resolver<Clz>(target));} \
  void call(Functions &f) const override {f.func(target);} \
};

#define MK_VOID_RESOLVER(func) \
template <> \
struct Resolver<void> : public ResolverBase \
{ \
  static Ptr make() {return Ptr(new Resolver<void>());} \
  void call(Functions &f) const override {f.func();} \
};

namespace three {

class AmbientLight;
class DirectionalLight;
class HemisphereLight;
class PointLight;
class RectAreaLight;
class SpotLight;

template <typename L>
using ResolveFunc = std::function<void(L &)>;

using VoidFunc = std::function<void()>;

namespace light {

struct Functions
{
  ResolveFunc<AmbientLight> ambient;
  ResolveFunc<DirectionalLight> directional;
  ResolveFunc<HemisphereLight> hemisphere;
  ResolveFunc<PointLight> point;
  ResolveFunc<RectAreaLight> rectarea;
  ResolveFunc<SpotLight> spot;
};

DEF_RESOLVER()

MK_RESOLVER(AmbientLight, ambient)
MK_RESOLVER(DirectionalLight, directional)
MK_RESOLVER(HemisphereLight, hemisphere)
MK_RESOLVER(PointLight, point)
MK_RESOLVER(RectAreaLight, rectarea)
MK_RESOLVER(SpotLight, spot)

}

class CubeTexture;
class Texture;
class Color;

namespace scene {

struct Functions
{
  ResolveFunc<std::shared_ptr<CubeTexture>> cubeTexture;
  ResolveFunc<std::shared_ptr<Texture>> texture;
  ResolveFunc<Color> color;
  VoidFunc _void;
};

DEF_RESOLVER()

MK_RESOLVER(std::shared_ptr<CubeTexture>, cubeTexture)
MK_RESOLVER(std::shared_ptr<Texture>, texture)
MK_RESOLVER(Color, color)
MK_VOID_RESOLVER(_void)

}

class Light;
class Sprite;
class LensFlare;
class Mesh;
class Line;
class Points;

namespace object {

struct Functions
{
  ResolveFunc<Light> light;
  ResolveFunc<Sprite> sprite;
  ResolveFunc<LensFlare> lensFlare;
  ResolveFunc<Mesh> mesh;
  ResolveFunc<Line> line;
  ResolveFunc<Points> points;
  VoidFunc _void;
};

DEF_RESOLVER()

MK_RESOLVER(Light, light)
MK_RESOLVER(Sprite, sprite)
MK_RESOLVER(LensFlare, lensFlare)
MK_RESOLVER(Mesh, mesh)
MK_RESOLVER(Line, line)
MK_RESOLVER(Points, points)
MK_VOID_RESOLVER(_void)

}

class InterleavedBufferAttribute;

namespace bufferattribute {

struct Functions
{
  ResolveFunc<InterleavedBufferAttribute> interleaved;
  VoidFunc _void;
};

DEF_RESOLVER()

MK_RESOLVER(InterleavedBufferAttribute, interleaved)
MK_VOID_RESOLVER(_void)

}

class DefaultFog;
class FogExp2;

namespace fog {

struct Functions
{
  ResolveFunc<DefaultFog> fogDefault;
  ResolveFunc<FogExp2> fogExp2;
};

DEF_RESOLVER()

MK_RESOLVER(DefaultFog, fogDefault)
MK_RESOLVER(FogExp2, fogExp2)

}
}
#endif //THREE_QT_RESOLVER_H
