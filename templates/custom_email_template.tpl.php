<div>
    <p><?=t('Selected text: ');?><strong><?=$typo_text;?></strong></p>
    <p><?=t('Field Machine Name: ');?><strong><?=$field_name;?></strong></p>
    <p><?=t('Field Title: ');?><strong><?=$field_name_ui;?></strong></p>
    <p><?=t('Node type: ');?><strong><?=$node_type;?></strong></p>
    <p><?=t('Node link: ');?><strong><?=url("node/$nid");?></strong></p>
    <p><?=t('Requested time: ');?><strong><?=$request_time;?></strong></p>
    <p><?=t('User comment: ');?><strong><?=$comment_text;?></strong></p>

</div>