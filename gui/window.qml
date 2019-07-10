import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

ApplicationWindow
{
  id: window
  visible: true
  title: qsTr("ROS Launcher")
  width: 800
  height: 600
  
  
  RowLayout{
    anchors.left: parent.left
    anchors.leftMargin: parent.width*0.05
    anchors.right: parent.right
    anchors.rightMargin: parent.width*0.05
    spacing: 10
    
    TextField
    {
      anchors.fill: parent
    }
    Button
    {
      text: "Test"
    }
  }
}