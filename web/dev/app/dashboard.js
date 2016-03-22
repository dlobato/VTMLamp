var Vue = require('vue');
var resources = require('./helpers/resources');
var store = require('./helpers/store');

module.exports = Vue.extend({

    template: require('./views/dashboard.html'),

    methods: {
        toggleLightStatus: function (program) {
            if (program == store.light.program) {
                resources.setLampStatus(100, 0);
                return;
            }
            resources.setLampStatus(50, program);
        }
    },
    data: function () {
        return {
            store: store
        }
    }
});