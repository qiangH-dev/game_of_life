import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import Qt.labs.settings 1.0

import net.phoneyou.GAMELIFE.BB 1.0 as BB
import net.phoneyou.AMSTER 1.0 as AS

import "as_functions.js" as ASF

Item {
    id: root

    GridViewer {
        anchors.fill: parent
        model: cellModel
    }
    

}
