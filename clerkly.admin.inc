<?php

/**
 * @file
 * Admin settings form for vk_openapi module
 */

/**
 * Clerky settings.
 */

function clerkly_admin_settings()
{
    $form = array();

    $form['clerkly_max_chars'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum characters in selection'),
        '#required' => true,
        '#default_value' => variable_get('clerkly_max_chars', 20),
        '#element_validate' => array('element_validate_integer_positive', 'element_validate_max_length')
    );

    $form['clerkly_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Information email'),
        '#description' => t('Clerkly information email for typos'),
        '#default_value' => variable_get('clerkly_email', variable_get('site_mail')),
        '#element_validate' => array('element_validate_email_settings')

    );

    $form['clerkly_popup_text'] = array(
        '#type' => 'textfield',
        '#title' => t('Popup text'),
        '#description' => t('Text that would be displayed for user on Ctrl+Enter click'),
        '#required' => true,
        '#default_value' => variable_get('clerkly_popup_text', t('Do you want to send a notice to a webmaster?')),
    );

    $form['clerkly_typo_ip_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Typos per IP'),
        '#description' => t('Set number to limit typos from the same IP address'),
        '#default_value' => variable_get('clerkly_typo_ip_limit', ''),
        '#element_validate' => array('element_validate_integer_positive')
    );

    return system_settings_form($form);
}

function correction_logs()
{

    $header = array(
        array('data' => t('Username'),'field' => 'u.name', 'sort' => 'ASC'),
        array('data' => t('IP Address'),'field' => 'ip', 'sort' => 'ASC'),
        array('data' => t('Field UI'),'field' => 'field_name_ui', 'sort' => 'ASC'),
        array('data' => t('Field Machine name'),'field' => 'field_name', 'sort' => 'ASC'),
        array('data' => t('Node Type'),'field' => 'node_type'),
        array('data' => t('Selected text')),
        array('data' => t('User comment')),
        array('data' => t('Created Date'), 'field' => 'date_created', 'sort' => 'DESC')
    );

    $result = _get_table_rows($header);

    $rows = array();

    foreach ($result as $row) {

        $rows[] = array(
            'data' => array(
                $row->name,
                long2ip($row->ip),
                $row->field_name_ui,
                $row->field_name,
                l($row->node_type, $row->node_url),
                $row->typo_text,
                $row->comment_text,
                $row->date_created
            )

        );
    }

    $output = theme('table', array(
                                'header' => $header,
                                'rows' => $rows,
                                'caption' => '<h2>'.t('USER CORRECTION LIST').'</h2>'
                            )
                );

    $output .= theme('pager');

    return $output;
}
