var express = require('express');
var bodyParser = require('body-parser');
var app = express();

app.use(bodyParser.json());       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
    extended: true
}));
app.use(express.static('web/dev'));

var selectedNetwork = "Votum";

app.post('/ajax/get-networks', function (req, res) {
    res.send({
        'status': true,
        'connected': true,
        'network': selectedNetwork,
        'available': [
            {
                'id': 'hash-1',
                'title': 'Votum',
                'signal': 1,
                'encryption': 'WPA'
            },
            {
                'id': 'hash-2',
                'title': 'Other Network',
                'signal': 1,
                'encryption': 'WPA-2'
            }
        ]
    });
});

app.post('/ajax/connect', function (req, res) {
    var newNetwork = req.body.network;

    selectedNetwork = newNetwork;

    res.send({
        'status': true,
        'connected': true
    })
});

// Start the Webserver
app.listen(1337, function () {
    console.log('VOTUM Lamp Prototype Project is listening on port 1337!');
});