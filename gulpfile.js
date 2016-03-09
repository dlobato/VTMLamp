var elixir = require('laravel-elixir');

elixir.config.production = true;

elixir(function (mix) {
    mix.browserify('./web/dev/app/main.js', './web/build/main.js');
    mix.browserify('./web/dev/app/main.js', './web/dev/main.js');
});