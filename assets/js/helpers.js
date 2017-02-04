(function($){
    "use strict";

    window.helperFunctions = window.helperFunctions || {
        selectedElement:{text:'', dataLength:0, fieldName:'', textLength:0},
        keyDownEvent: function(event){
            if ((event.ctrlKey) && ((event.keyCode == 0xA)||(event.keyCode == 0xD))) {
                helperFunctions.verifySelectedTextAndSendRequest();
            }
        },
        verifySelectedTextAndSendRequest: function() {
            var text = helperFunctions.selectedElement.text;

            if (helperFunctions.selectedElement && text.length > 0) {
                var textLength = helperFunctions.selectedElement.textLength,
                    dataLength = helperFunctions.selectedElement.dataLength;

                console.log(text);

                if (textLength > dataLength) {
                    alert(Drupal.t('Selected text is too long'));
                } else {

                }
            }
        },
        getSelectedText: function() {
            $('body').mouseup(function() {
                helperFunctions.selectedElement = {text:'', dataLength:0, fieldName:'', textLength:0};

                if (window.getSelection) {
                    var selection = window.getSelection(),
                        selectedText = $.trim(window.getSelection().toString());

                    if (selectedText && selectedText.length > 0) {

                        var selectedElement = null,
                            clerklyElement = null,
                            dataLength = 0,
                            fieldName = '';

                        while (selectedElement = $(selection.focusNode.parentNode)) {

                            clerklyElement = selectedElement.closest('div[class*="field-name-"]');

                            if (clerklyElement && clerklyElement.hasClass('clerkly-field')) {
                                console.log(clerklyElement);
                                dataLength = parseInt(clerklyElement.attr('data-length'), 10);
                                fieldName = clerklyElement.attr('data-field');
                                helperFunctions.selectedElement = {
                                    text:selectedText,
                                    dataLength:dataLength,
                                    fieldName:fieldName,
                                    textLength:parseInt(selectedText.length, 10)
                                };

                                break;
                            }
                        }
                    }
                }

            });
        }
    };
})(jQuery);