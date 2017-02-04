<?php
/**
 * @file
 * Implementation of custom validators
 */

function element_validate_rich_text_enabled(&$element, &$form_state) {
    $value = &$element['#value'];
    $settings = $form_state['values']['instance']['settings'];

    if ($value == true && $settings['text_processing'] == 0) {
        form_set_value($element, false, $form_state);
    }
}

function element_validate_email($element, &$form_state) {
    $value = $element['#value'];

    if (!empty($value) && !valid_email_address($value)) {
        form_error($element, t('Email address has incorrect format. Correct format is: "sample@email.com"'));
    }
}

function element_validate_email_settings($element, &$form_state) {
    $value = $element['#value'];
    if (empty($value)) {
        form_set_value($element, variable_get('site_mail'), $form_state);
    } else {
        element_validate_email($element, $form_state);
    }

}
