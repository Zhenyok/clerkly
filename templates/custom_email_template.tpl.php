<!DOCTYPE html>
<html>
    <head>

    </head>
    <body>
        <div>
            <p><?=t('Selected text: ');?><strong><?=$typo_text;?></strong></p>
            <p><?=t('Field Machine Name: ');?><strong><?=$field_name;?></strong></p>
            <p><?=t('Field Title: ');?><strong><?=$field_name_ui;?></strong></p>
            <p><?=t('Node type: ');?><strong><?=$node_type;?></strong></p>
            <p><?=t('Node link: ');?><strong><?=$node_url;?></strong></p>
            <p><?=t('Requested time: ');?><strong><?=$date_created;?></strong></p>
            <p><?=t('User comment: ');?><strong><?=$comment_text;?></strong></p>
            <p><?=t('User IP: ')?>
                <?php
                    if(isset($ip)):
                        echo $ip;
                    endif;
                ?>
            </p>
            <p><?=t('Username: ')?>
                <?php
                    if (isset($uid)):
                        echo $uid;
                    endif;
                ?>
            </p>
        </div>
    </body>
</html>
