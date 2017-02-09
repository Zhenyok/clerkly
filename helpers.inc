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



function _on_modal_show($node) {
    $commands = array();
    $output = drupal_render(drupal_get_form('response_form', $node));

    $commands[] = ctools_modal_command_display(t('Send request for text correction'), $output);
    $commands[] = array('command' => 'setModalData');

    print ajax_render($commands);
    drupal_exit();
}

function _send_form_reply($field_type, $selection, $comment, $node_info) {
    $result = false;

    if (empty($field_type) || !isset($node_info['args']) || !isset($node_info['args'][0])
        || !is_object($node_info['args'][0])) {
            return $result;
    }

    $node = $node_info['args'][0];
    $field_info = field_view_field('node', $node, $field_type);
    $field_settings = _get_usable_settings($field_info);

    if (!_verify_vital_settings($field_settings)) {
        return $result;
    }

    return $result;
}

function _verify_vital_settings($field_settings = array()) {
    $result = false;

    if (!isset($field_settings['clerkly_email_typo']) || empty($field_settings['clerkly_email_typo'])
        || !isset($field_settings['clerkly_is_active'])) {
            return $result;
    }

    if (boolval($field_settings['clerkly_is_active']) == true) {
        $typo_count = _get_typo_count();
    }
}

function _get_typo_count() {
    global $user;

    $result = 0;
    $ip_address = ip2long(ip_address());
    $query = 'SELECT COUNT(id) FROM clerkly_log WHERE log_date = NOW() AND ';



    if (user_is_logged_in()) {
        $query .= '(ip_addr = :ip OR uid = :uid)';
    } else {
        $query .= 'ip_addr = :ip';
    }

    $result = db_query($query, array('ip' => $ip_address, 'uid' => $user->uid));

    return $result;
}

function _get_markup_response($variables) {

    if (!isset($variables['message_type']) || !isset($variables['message']) ) {
        return;
    }

    return '<div id="clerkly-message-block" class="message info-'.$variables['message_type'].'">'.$variables['message'].'</div>';
}



