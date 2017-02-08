(function($){
    "use strict";


    Drupal.behaviors.sendTypoEvent = {
        attach: function(context, settings) {
            helperFunctions.getSelectedText();
            $('body').keydown(helperFunctions.keyDownEvent);
        }
    };

    Drupal.ajax.prototype.commands.setModalData = function(ajax,response,status) {
        if (helperFunctions.selectedElement) {
            if (helperFunctions.selectedElement.fieldName) {
                $('#clerkly-popup-window #clerkly-popup-field').val(helperFunctions.selectedElement.fieldName);

            }
            if (helperFunctions.selectedElement.popupText) {
                $('#clerkly-popup-window #clerkly-popup-text').html(helperFunctions.selectedElement.popupText);

            }
        }
    };


})(jQuery);