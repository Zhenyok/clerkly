(function($){
    "use strict";
    window.helperFunctions = window.helperFunctions || {
        keyDownEvent: function(event){
            if ((event.ctrlKey) && ((event.keyCode == 0xA)||(event.keyCode == 0xD))) {
                if (window.getSelection) {
                    var selectedObject = window.getSelection();
                    console.log(document.createRange());
                }
            }
        },
        getSelectedText: function() {
            var selectedText = {text:'',
                                field:''};

            if (window.getSelection) {
                var range = window.getSelection();
                selectedText = range.toString();
            } else {
                if (document.selection.createRange) {
                    var range = document.selection.createRange();
                    selectedText = range.text;
                }
            }
        }
    };
})(jQuery);