(function($){
    "use strict";

    Drupal.behaviors.sendTypoEvent = {
        attach: function(context, settings) {
            window.addEventListener("keydown", helperFunctions.keyDownEvent);
        }
    };
})(jQuery);