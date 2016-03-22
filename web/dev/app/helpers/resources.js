var Vue = require('vue');
var endpoints = require('./endpoints');
var store = require('./store');

module.exports = {
    init: function () {

        this.refreshData();

        setInterval(function () {
            if (store.app.refreshTimer == 0) {
                this.refreshData();
            }
            store.app.refreshTimer = store.app.refreshTimer - 1;
        }.bind(this), 1000);
    },

    refreshData: function () {
        store.app.refreshTimer = 10;
        Vue.http.get(endpoints.IP + endpoints.GET_LAMP_STATUS).then(function (data) {
            store.light = data.data;
            store.app.refreshTimer = 10;
        });
    },

    setLampStatus: function (bright, program) {

        Vue.http.post(endpoints.IP + endpoints.SET_LAMP_STATUS, {
            brightness: bright,
            program: program
        }, {
            emulateJSON: true
        }).then(function (data) {

            store.light.brightness = bright;
            store.light.program = program;

        });

    }
};