(function($){
    "use strict";
    window.helperFunctions = window.helperFunctions || {
        keyDownEvent: function(event){
            if ((event.ctrlKey) && ((event.keyCode == 0xA)||(event.keyCode == 0xD))) {
                helperFunctions.getSelectedText();
            }
        },
        getSelectedText: function() {
            var selectedText = {text:'',
                                field:''};
            if (window.getSelection) {
                var range = document.getSelection();
                selectedText = range.toString();
                console.log(range.getRangeAt(0).commonAncestorContainer);
            } else {
                if (document.selection.createRange) {
                    var range = document.selection.createRange();
                    selectedText = range.text;
                }
            }
        }
    };
})(jQuery);