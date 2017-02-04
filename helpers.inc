<?php

function _get_field_settings($field) {
    $result = array();
    if (isset($field['#entity_type']) && isset($field['#field_name']) && isset($field['#bundle'])) {
        $settings = field_info_instance($field['#entity_type'], $field['#field_name'], $field['#bundle']);

        if (isset($settings['settings']) && isset($settings['settings']['clerkly_settings'])) {
            return $settings['settings']['clerkly_settings'];
        }
    }

    return $result;
}

function _get_clerkly_settings() {

    $result = &drupal_static(__FUNCTION__);

    if (!isset($result)) {
        $max_chars = variable_get('clerkly_max_chars');
        $email = variable_get('clerkly_email');
        $popup_text = variable_get('clerkly_popup_text');
        $typo_ip_limit = variable_get('clerkly_typo_ip_limit');

        $result = array(
            'max_chars' => (int) $max_chars,
            'email' =>$email,
            'popup_text' => $popup_text,
            'typo_ip_limit' => $typo_ip_limit
        );
    }

    return $result;
}

function _get_usable_settings(array $field) {
    $result = array();

    $module_settings = _get_clerkly_settings();
    $field_settings = _get_field_settings($field);

    $result['clerkly_typo_ip_limit'] = !empty($module_settings['typo_ip_limit']) ? $module_settings['typo_ip_limit'] : '';

    if (isset($field_settings['clerkly_is_active']) && !empty($field_settings['clerkly_is_active'])) {
        $result['clerkly_is_active'] = $field_settings['clerkly_is_active'];

    } else {
        $result['clerkly_is_active'] = false;
    }

    if (isset($field_settings['clerkly_email_typo']) && !empty($field_settings['clerkly_email_typo'])) {
        $result['clerkly_email_typo'] = $field_settings['clerkly_email_typo'];

    } else {
        $result['clerkly_email_typo'] = $module_settings['email'];
    }

    if (isset($field_settings['clerkly_typo_length']) && !empty($field_settings['clerkly_typo_length'])) {
        $result['clerkly_max_chars'] = $field_settings['clerkly_typo_length'];

    } else {
        $result['clerkly_max_chars'] = $module_settings['max_chars'];
    }

    if (isset($field_settings['clerkly_popup_text']) && !empty($field_settings['clerkly_popup_text'])) {
        $result['clerkly_popup_text'] = $field_settings['clerkly_popup_text'];

    } else {
        $result['clerkly_popup_text'] = $module_settings['popup_text'];
    }

    return $result;
}

