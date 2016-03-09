var Vue = require('vue');
require('./helpers/resources').init();

new Vue({
    el: 'body',
    data: {
        name: 'World'
    },
    ready: function () {
        console.log('Application Ready');
    }
});