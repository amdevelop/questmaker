import QtQuick 1.1

Rectangle {
    id: container
    width: parent.width

    Component {
        id: item_delegate

        TextInput {
            width: container.width
            height: 50
        }
    }

    ListModel{

    }

    ListView {
        width: parent.width


    }

    //    width: 100
    //    height: 62
}
