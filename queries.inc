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