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
            if (helperFunctions.selectedElement.text) {
                $('#clerkly-selected-info').before(Drupal.t('Selected text: '));
                $('#clerkly-selected-info').text(helperFunctions.selectedElement.text);
                $('#clerkly-selection').val(helperFunctions.selectedElement.text);

            }
        }
    };


})(jQuery);