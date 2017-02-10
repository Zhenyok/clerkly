<?php

function _get_typo_count() {

    $result = 0;
    $ip_address = ip2long(ip_address());
    $query_result = db_query('SELECT COUNT(id) as total FROM clerkly_log WHERE DATE(date_created) = CURDATE() 
                              AND ip = :ip',
                        array('ip' => $ip_address));

    $result = $query_result->fetchColumn(0);

    return $result;
}

function _insert_record_log($params = array()) {

    $is_inserted = false;

    try {

        $fields = array(
            'node_type' => $params['node_type'],
            'nid' => $params['nid'],
            'field_name_ui' => $params['field_name_ui'],
            'field_name' => $params['field_name'],
            'date_created' => $params['date_created'],
            'typo_text' => $params['typo_text'],
            'comment_text' => $params['comment_text'],
        );

        if (isset($params['ip']) && !empty($params['ip'])) {
            $fields += array('ip' => $params['ip']);
        }

        if(isset($params['uid']) && !empty($params['uid'])) {
            $fields += array('uid' => $params['uid']);
        }

        $id = db_insert('clerkly_log')
                ->fields($fields)
                ->execute();

        if ($id != null) {
            return true;
        }

    } catch(Exception $e) {
        watchdog('database', $e->getMessage());
    }

    return $is_inserted;
}
