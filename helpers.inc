<?php

/**
 * @file
 * Provides Helper functions logic.
 */

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

    }
    else {
        $result['clerkly_is_active'] = false;
    }

    if (isset($field_settings['clerkly_email_typo']) && !empty($field_settings['clerkly_email_typo'])) {
        $result['clerkly_email_typo'] = $field_settings['clerkly_email_typo'];

    }
    else {
        $result['clerkly_email_typo'] = $module_settings['email'];
    }

    if (isset($field_settings['clerkly_typo_length']) && !empty($field_settings['clerkly_typo_length'])) {
        $result['clerkly_max_chars'] = $field_settings['clerkly_typo_length'];

    }
    else {
        $result['clerkly_max_chars'] = $module_settings['max_chars'];
    }

    if (isset($field_settings['clerkly_popup_text']) && !empty($field_settings['clerkly_popup_text'])) {
        $result['clerkly_popup_text'] = $field_settings['clerkly_popup_text'];

    }
    else {
        $result['clerkly_popup_text'] = $module_settings['popup_text'];
    }

    if (isset($field['#title']) && !empty($field['#title'])) {
        $result['field_title'] = $field['#title'];
    }
    else {
        $result['field_title'] = '';
    }

    return $result;
}



function _on_modal_show($node) {
    $commands = array();
    $output = drupal_get_form('response_form', $node);

    $commands[] = ctools_modal_command_display(t('Send request for text correction'), $output);
    $commands[] = array('command' => 'setModalData');

    print ajax_render($commands);
    drupal_exit();
}

function _send_form_reply($field_type, $selection, $comment, $node_info) {

    global $user;
    global $language;
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

    $variables = array(
                    'node_type' => $node->type,
                    'node_url' => url('node/'. $node->nid),
                    'field_name_ui' => $field_settings['field_title'],
                    'field_name' => $field_type,
                    'date_created' => date('Y-m-d H:i:s', time()),
                    'typo_text' => $selection,
                    'comment_text' => $comment

                );


    if (user_is_logged_in()) {
        $variables += array('uid' => $user->uid);
    }

    $variables += array('ip' => ip2long(ip_address()));



    $message = drupal_mail('clerkly', 'clerkly_mail' , $field_settings['clerkly_email_typo'],
                            $language->language, $variables);

    if ($message['send'] && $message['result']) {

        if (!_insert_record_log($variables)) {
            return false;
        }

        return true;

    }

    return $result;
}

function _get_markup_response($message_type) {

    if (!isset($message_type)) {
        return '';
    }


    if ($message_type == 'error') {
        $message = t('Request wasn\'t sent due to error');
    }
    else {
        $message = t('Thanks for your request.');
    }

    return theme('container', array(
        'element' => array(
            '#children' => $message,
            '#id' => 'clerkly-message-block',
            '#attributes' => array(
                'class' => array(
                    'info-message',
                    'info-'.$message_type
                )
            )
        )
    ));
}



