import QtQuick 2.0

Item {
    Image {
        source: "qrc:/konten/gambar/kompas/compass.svg"
        width: 151
        height: 131
        x: -5
        y: -1
    }
    Image {
        x: 62
        y: 24
        width: 16
        height: 80
        source: "qrc:/konten/gambar/kompas/arrow.svg"
        transform: Rotation {
            origin.x: 8
            origin.y: 41
            angle: 0
            objectName: "rect"
        }
    }
}

