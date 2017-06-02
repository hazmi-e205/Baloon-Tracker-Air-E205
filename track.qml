import QtQuick 2.0
import QtWebEngine 1.0

item {
    width: 601
    height: 311
    visible: true
    WebEngineView {
        anchors.fill: parent
        url: "http://127.0.0.1/cesiumjs/Apps/Sandcastle/tracker/index2.html"
    }
}
