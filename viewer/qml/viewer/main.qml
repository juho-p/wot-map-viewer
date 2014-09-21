/*
 * WoT map viewer
 *
 * author: Juho Peltonen
 * license: GPL3, see gpl-3.0.txt included in the package
 */

import QtQuick 2.0
import mapviewer.siemapl 1.0

Rectangle {
    id: root

    width: 360
    height: 360

    Siema {
        id: map
        anchors.fill: parent

        onInfoChanged: {
            var id, veh;
            var info = JSON.parse(map.info);
            vehicles.clear();

            for (id in info.vehicles) {
                veh = info.vehicles[id];
                if (veh.dyn && veh.alive) {
                    vehicles.append(veh);
                }
            }
        }
    }

    ListModel {
        id: vehicles
    }

    Repeater {
        anchors.fill: parent
        model: vehicles
        delegate: Item {
            x: dyn.pos[0] * map.width
            y: -dyn.pos[1] * map.height + map.height
            Image {
                id: type_icon
                anchors.centerIn: parent
                scale: 0.25
                source: "../../img/" + type + '_' + team + (active ? 'a' : 'i') + '.svg'
            }
            Text {
                id: info_text
                x: 10; y: -5
                text: name + '\n' + tank
                font.pixelSize: 10
                color: active ? 'white' : 'grey'
            }
            Rectangle {
                id: health_bar
                x: 0
                y: -10
                width: 20
                height: 6
                color: 'black'
                Rectangle {
                    width: Math.round((parent.width - 2) * (dyn.health/max_health))
                    height: parent.height - 2
                    x: 1
                    y: 1
                    color: accurate ? (team == 'f' ? 'green' : 'red') : 'grey'
                }
            }
        }
    }
}
