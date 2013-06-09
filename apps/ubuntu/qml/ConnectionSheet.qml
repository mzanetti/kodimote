import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1
import Ubuntu.Components.ListItems 0.1 as ListItems
import Xbmc 1.0

Dialog {
    id: sheet
    title: "Select Host"


    signal hostSelected(int index);


}
