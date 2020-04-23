import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import net.phoneyou.GAMELIFE.BB 1.0 as BB

import "."

Item {
    id: root
    property alias model: _cellView.model
    property int cellX: BB.Global.Dimensions.cell_x
    property int cellY: BB.Global.Dimensions.cell_y
    property double cellW: width * 1.0 / cellX
    property double cellH: height * 1.0 / cellY
    property double cellSize: Math.min(cellW , cellH)
    property double offsetX: (width - cellSize * cellX) / 2
    property double offsetY: (height - cellSize * cellY) / 2
    Repeater {
        id: _cellView
        anchors.fill: parent
        delegate: GridCell {
            id: _item
            x: root.offsetX + _item.pos.x * root.cellSize
            y: root.offsetY + _item.pos.y * root.cellSize
            width: root.cellSize
            height: root.cellSize
        }
    }

}
