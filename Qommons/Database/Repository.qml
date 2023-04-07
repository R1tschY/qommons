import QtQuick 2.0

QtObject {
    property string table
    property list columns: []
    property string idColumn
    property list changelog: []

    property string dataBaseId

    property var _db

    onDataBaseIdChanged: {
        if (dataBaseId) {
            _db = _open(dataBaseId)
        } else {
            _db = null
        }
    }

    function _open() {
        console.log("Load DB " + dataBaseId)
        var db = LocalStorage.openDatabaseSync(dataBaseId, "", "", 1000000)
        _migrate(db)
        return db
    }

    function _migrate(db) {
        var version = db.version === "" ? 0 : parseInt(db.version)

        for (var i = version; version < changelog.length; version++) {
            var next = i + 1
            console.log("Upgrade DB " + dataBaseId
                + " from version " + db.version + " to version " + next)
            db.changeVersion(db.version, next.toString(), function (tx) {
                tx.executeSql(changelog[i])
            })
        }
    }

    function getById(id) {
        var res = []
        _db.readTransaction(function (tx) {
            var results = tx.executeSql(
                'SELECT ' + columns.join(", ") + ' FROM ' + table + ' WHERE ' + idColumn + " = ?",
                [id])

            var length = results.rows.length
            for (var i = 0; i < length; i++) {
                var item = results.rows.item(i)
                //var obj = {}
                //for (var c = 0; c < columns.length; ++c) {
                //    obj[columns[c]] = item[columns[c]]
                //}
                res.push(item)
            }
        })
        return res[0]
    }

    function getAll(orderBy) {
        var res = []
        _db.readTransaction(function (tx) {
            var results = tx.executeSql(
                'SELECT ' + columns.join(", ") + ' FROM ' + table + ' ORDER BY ' + orderBy)

            var length = results.rows.length
            for (var i = 0; i < length; i++) {
                var item = results.rows.item(i)
                //var obj = {}
                //for (var c = 0; c < columns.length; ++c) {
                //    obj[columns[c]] = item[columns[c]]
                //}
                res.push(item)
            }
        })
        return res
    }

    function deleteById(id) {
        _db.transaction(function (tx) {
            tx.executeSql('DELETE FROM ' + table + ' WHERE ' + idColumn + ' = ?', [id])
        })
    }

    function clear() {
        _db.transaction(function (tx) {
            tx.executeSql('DELETE FROM ' + table)
        })
    }
}
