#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QDebug>
#include <threepp/quick/Three.h>

#include "ShadowMapViewer.h"
#ifdef WITH_REACTPHYSICS
#include "HingeEditorModelRef.h"
#endif

int main(int argc, char *argv[])
{
  //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  //QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  QGuiApplication app(argc, argv);
  QQmlApplicationEngine qmlEngine;

  three::quick::init();
  qmlRegisterType<three::quick::ShadowMapViewer>("three.quick", 1, 0, "ShadowMapViewer");
#ifdef WITH_REACTPHYSICS
  qmlRegisterType<three::quick::HingeEditorModelRef>("three.quick", 1, 0, "HingeEditorModelRef");
#endif

  QQmlComponent maincomponent(&qmlEngine);
  //maincomponent.loadUrl(QUrl("qrc:///geometries.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///spotlight.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///material_envmap.qml"));
  maincomponent.loadUrl(QUrl("qrc:///orbit_controls.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///3ds_model.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///orthographic_camera.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///cubes_ortho_picker.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///load_model.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///cubemap_model.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///voxelpainter.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///hemisphere.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///cameras.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///3D_text.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///SVG_extrude.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///drag_cubes.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///webgl_shadowmap.qml"));
  //maincomponent.loadUrl(QUrl("qrc:///model_physics.qml"));

  if (maincomponent.isError()) {
    qCritical() << "error loading main screen: " << maincomponent.errorString();
    return -1;
  }

  QObject *mc = maincomponent.create(qmlEngine.rootContext());
  int ret = app.exec();

  delete mc;
  return ret;
}
