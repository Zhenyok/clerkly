(function($){
    "use strict";

    window.helperFunctions = window.helperFunctions || {
        selectedElement:{text:'', dataLength:0, fieldName:'', textLength:0, popupText:''},
        keyDownEvent: function(event){
            if ((event.ctrlKey) && ((event.keyCode == 0xA)||(event.keyCode == 0xD))) {
                helperFunctions.generateModalContent();
            }
        },
        verifySelectedTextAndSendRequest: function() {
            var text = helperFunctions.selectedElement.text;

            if (helperFunctions.selectedElement && text.length > 0) {
                var textLength = helperFunctions.selectedElement.textLength,
                    dataLength = helperFunctions.selectedElement.dataLength;

                if (textLength > dataLength) {
                    alert(Drupal.t('Selected text is too long'));
                    return;
                } else {
                    return true;
                }
            }

            return false;
        },

        generateModalContent: function() {
            if (helperFunctions.verifySelectedTextAndSendRequest() == true) {
                if (!Drupal.settings['nid']) {
                    return;
                }
                var url  = Drupal.settings.basePath + 'clerkly-popup/'+Drupal.settings['nid'];
                var link = $("<a></a>").attr('href', url).addClass('ctools-use-modal-processed')
                                                        .click(Drupal.CTools.Modal.clickAjaxLink);

                Drupal.ajax[url] = new Drupal.ajax(url, link.get(0), {
                    url: url,
                    event: 'click',
                    progress: { type: 'throbber' },
                    settings:{
                            width:200,
                            height:200
                    }
                });

                link.click();
            }

        },

        setSelectedElementObject: function(element, text) {
            if (element && text && element[0].innerText.match(text)) {
                var dataLength = parseInt(element.attr('data-length'), 10),
                    fieldName = element.attr('data-field'),
                    popupText = element.attr('data-popup-text');

                helperFunctions.selectedElement = {
                    text:text,
                    dataLength:dataLength,
                    fieldName:fieldName,
                    textLength:parseInt(text.length, 10),
                    popupText:popupText
                };
            }
        },

        traverseElements: function(selectedElement) {
            var field,
                selection = window.getSelection();

            if (selectedElement && selectedElement['nodeName'] !== 'DIV') {

                field = $(selection.anchorNode.parentNode).closest('div[class*="field-name-"]');

                if ($(selection.anchorNode.parentNode).closest('div[class*="field-name-"]').hasClass('clerkly-field')) {
                    helperFunctions.setSelectedElementObject(field, selectedElement.textContent);
                }

                return true;

            } else {

                if ($(selectedElement).hasClass('clerkly-field')) {
                    helperFunctions.setSelectedElementObject($(selectedElement), selectedElement.innerText);
                    return true;
                } else if($(selectedElement).closest('div[class*="field-name-"]').hasClass('clerkly-field')) {
                    helperFunctions.setSelectedElementObject($(selectedElement).closest('div[class*="field-name-"]'),
                        selectedElement.innerText);
                    return true;
                }
            }

            return false;
        },
        getSelectedText: function() {
            $('body').mouseup(function() {

                helperFunctions.selectedElement = {text:'', dataLength:0, fieldName:'', textLength:0, popupText:''};

                if (window.getSelection) {
                    var selection = window.getSelection(),
                        selectedText = $.trim(window.getSelection().toString());

                    if (selectedText && selectedText.length > 0) {

                        var range = selection.getRangeAt(0),
                            element = range.cloneContents(),
                            selectedElement,
                            field,
                            start = 0,
                            finish = 0,
                            traversed = false;

                        if (element && $(element)[0] && $(element)[0].childNodes) {

                            element = $(element)[0].childNodes;

                            if (selection.focusNode === range.endContainer) {
                                start = 0;
                                finish = element.length;
                            } else {
                                start = element.length;
                                finish = 0;
                            }

                            if (start < finish) {
                                for ( var i = start; i < finish; i++ ) {
                                    if (helperFunctions.traverseElements(element[i]) == true) {
                                        break;
                                    }
                                }
                            } else {
                                for ( var i = start; i >= finish; i-- ) {
                                    if (helperFunctions.traverseElements(element[i]) == true) {
                                        break;
                                    }
                                }
                            }
                        }

                    }
                }

            });
        }
    };
})(jQuery);