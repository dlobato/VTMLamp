var Vue = require('vue');
var VueRouter = require('vue-router');
var store = require('./helpers/store');
Vue.use(require('vue-resource'));
Vue.use(VueRouter);

require('./helpers/resources').init();
var endpoints = require('./helpers/endpoints');

var router = new VueRouter();

router.map({

    '/': {
        component: require('./dashboard')
    },

    '/network': {
        component: require('./network')
    }

});

var App = Vue.extend({
    data: function () {
        return {
            store: store
        }
    }
});

router.start(App, 'body');