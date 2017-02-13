<?php
/**
 * @file
 * Implementation of custom validators
 */

function element_validate_rich_text_enabled(&$element, &$form_state) {
/*    $value = &$element['#value'];
    $settings = $form_state['values']['instance']['settings'];

    if ($value == true && $settings['text_processing'] == 0) {
        form_set_value($element, false, $form_state);
    }*/
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

function element_validate_max_length($element, &$form_state) {
    $value = $element['#value'];

    if (!empty($value) && intval($value) > 255) {
        form_error($element, t('Maximum typo length is 255 characters'));
    }
}

function _verify_vital_settings($field_settings = array()) {
    $result = false;

    if (!isset($field_settings['clerkly_email_typo']) || empty($field_settings['clerkly_email_typo'])
        || !isset($field_settings['clerkly_is_active'])) {

            watchdog('Error: ', 'Clerkly validation error. Email is not set, or field activity is not defined');
            return $result;
    }

    if (boolval($field_settings['clerkly_is_active']) == true) {

        if (isset($field_settings['clerkly_typo_ip_limit']) && !empty($field_settings['clerkly_typo_ip_limit'])) {
            $typo_count = _get_typo_count();

            if ($typo_count >= intval($field_settings['clerkly_typo_ip_limit'])) {
                return false;
            }

        }

        return true;
    }

    return $result;
}
