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

/**
 * Implements hook_form.
 */
function response_form($form, $form_state) {
    $form = array();
    $form['#prefix'] = '<div id="clerkly-popup-window">';
    $form['#suffix'] = '</div>';

    $form['#attached'] = array('js' => array(
        'alert("Yay.");' => 'inline'));

    $form['clerkly_info'] = array(
        '#markup' => '<p id="clerkly-popup-text"></p>'
    );

    $form['clerkly_comment'] = array(
        '#title' => t('Write your comment here'),
        '#type' => 'textfield',
        '#attributes'=>array('id'=>'clerkly-modal-comment')
    );
    $form['field_type'] = array(
                                '#type' => 'hidden',
                                '#value' => '',
                                '#attributes'=>array(
                                        'id'=>'clerkly-popup-field'
                                    )
                                );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Send'),
        '#ajax'=>array(
            'callback'=>'_proccess_modal_request',
            'method'=>'replace',
            'wrapper'=>'info',
            'effect' => 'fade'
        )
    );
    return $form;
}

function _on_modal_show($form, $form_state) {

    $output = drupal_render(drupal_get_form('response_form'));
    $commands = array();
    $commands[] = ctools_modal_command_display(t('Send request for text correction'), $output);
    $commands[] = array('command' => 'setModalData');

    print ajax_render($commands);
    drupal_exit();
}



function _proccess_modal_request($form, $form_state) {

    $field_type = '';
    $comment = '';
    $commands = array();

    $commands[] = ajax_command_before('#clerkly-popup-text', theme('status_messages', array('message' => t('Request wasn\'t sent due to error'))));
    print ajax_render(array('#type' => 'ajax', '#commands' => $commands));
    drupal_exit();

    if (isset($form_state['input']['field_type']) && !empty($form_state['input']['field_type'])) {

        $field_type = $form_state['input']['field_type'];

    } else {

        $commands[] = ajax_command_before('#clerkly-popup-text', theme('status_messages', array('message' => t('Request wasn\'t sent due to error'))));
        print ajax_render(array('#type' => 'ajax', '#commands' => $commands));
        drupal_exit();

    }
    if (isset($form_state['input']['clerkly_comment'])) {
        $comment = $form_state['input']['clerkly_comment'];
    }
    $commands[] = ajax_command_replace(NULL, render($form));

    $commands[] = ajax_command_before('#clerkly-popup-text', theme('status_messages',array('message' => t('Request was sent'))));
    $commands[] = ctools_modal_command_dismiss();
    print ajax_render(array('#type' => 'ajax', '#commands' => $commands));
    drupal_exit();
}

