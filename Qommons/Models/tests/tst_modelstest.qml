import QtQuick 2.3
import QtTest 1.0
import Qommons.Models 0.1

TestCase {
    name: "ModelsTest"

    function initTestCase() {
    }

    function cleanupTestCase() {
    }

    ListModel {
        id: model

        ListElement { a: 0; v: 1; z: 0 }
        ListElement { a: 0; v: 2; z: 0 }
        ListElement { a: 0; v: 3; z: 0 }
    }

    SortFilterProxyModel {
        id: proxyModel

        sourceModel: model

        filter: EqualsFilter {
            roleName: "v"
            value: 2
        }
    }

    function test_equals() {
        compare(proxyModel.count, 1);
        compare(proxyModel.get(0, "v"), 2);
    }
}
