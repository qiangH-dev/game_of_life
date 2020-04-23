import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import Qt.labs.settings 1.0

import net.phoneyou.GAMELIFE.BB 1.0 as BB
import net.phoneyou.AMSTER 1.0 as AS

import "as_functions.js" as ASF

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 800
    title: qsTr("covid-19 [fps: %1]").arg(AS.BBAccessor.fps)
    background: Rectangle {
        anchors.fill: parent
        color: "gray"
    }


    AS.TypeFilter {
        id: cellModel
        selTypes: [BB.EntityType.Cell]
        source: AS.BBAccessor.entityModel()
    }

//    MainContent {
//        id: content
//        anchors.fill: parent
//    }


}
