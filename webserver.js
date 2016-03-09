var express = require('express');
var app = express();
app.use(express.static('web/dev'));


// Start the Webserver
app.listen(1337, function () {
    console.log('VOTUM Lamp Prototype Project is listening on port 1337!');
});