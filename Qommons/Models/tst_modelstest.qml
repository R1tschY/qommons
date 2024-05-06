import QtQuick 2.3
import QtTest 1.0
import Qommons.Models 0.1

TestCase {
    name: "ModelsTest"

    function initTestCase() {
    }

    function cleanupTestCase() {
    }

    SortFilterProxyModel {
        id: model
    }

    function test_case1() {
        compare(1 + 1, 2, "sanity check");
        verify(true);
    }
}
