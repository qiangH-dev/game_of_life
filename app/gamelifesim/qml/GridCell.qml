import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import net.phoneyou.GAMELIFE.BB 1.0 as BB

import "."
import "bb_functions.js" as BBF

Item{
    id: root
    BB.EntityCast {
        id: _cast
        from: entity
    }
    property alias cell: _cast.toCell
    property var cellType: cell.info.type
    property var entityId: metId
    property var pos: cell.pos
//    property int degree: grid.contact.degree
    property color typeColor: cellType === cellType.Survive ? "red" : "gray"
    property bool actived: cellType === cellType.Survive
    Rectangle{
        id: _backgroud
        anchors.fill: parent
        color: typeColor
    }
}
