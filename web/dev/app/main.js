var Vue = require('vue');
require('./helpers/resources').init();

new Vue({
    el: 'body',
    data: {
        name: '',
        connected: false,
        network: 'Votum',
        refreshSeconds: 25,
    },
    methods: {
        refreshData: function () {
            setInterval(function () {
                if (this.refreshSeconds == 0) {
                    this.refreshSeconds = 25;

                    // For Demo Purposes
                    this.connected = !this.connected;

                    return;
                }
                this.refreshSeconds = this.refreshSeconds - 1;
            }.bind(this), 1000);
        }
    },
    ready: function () {
        console.log('Application Ready');
        this.refreshData();
    }
});