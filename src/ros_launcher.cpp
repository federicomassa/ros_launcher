#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/window.qml")));
  
  return app.exec(); 
}