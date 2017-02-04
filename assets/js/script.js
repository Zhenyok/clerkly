(function($){
    "use strict";

    Drupal.behaviors.sendTypoEvent = {
        attach: function(context, settings) {
            helperFunctions.getSelectedText();
            $('body').keydown(helperFunctions.keyDownEvent);
        }
    };
})(jQuery);