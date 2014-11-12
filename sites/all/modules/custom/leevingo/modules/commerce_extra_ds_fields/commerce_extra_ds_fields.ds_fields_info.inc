<?php
// create extra ds fields.
/**
 * Implements hook_ds_fields_info().
 */
function commerce_extra_ds_fields_ds_fields_info($entity_type) {
  global $user;
  $file = drupal_get_path('module', 'commerce_extra_ds_fields') . '/includes/ds_fields.inc';
  $fields = array();

  $fields['user']['email'] = array(
    'title' => t('Email'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'commerce_extra_ds_fields_user_mail',
    'file' => $file,
  );
  $fields['user']['language'] = array(
    'title' => t('Language'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'commerce_extra_ds_fields_user_default_language',
    'file' => $file,
  );
  $fields['user']['timezone'] = array(
    'title' => t('Timezone'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'commerce_extra_ds_fields_user_timezone',
    'file' => $file,
  );
  $fields['commerce_order']['number'] = array(
    'title' => t('Order Nr'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'commerce_extra_ds_fields_order_number',
    'file' => $file,
  );
  $fields['commerce_order']['date_created'] = array(
      'title' => t('Order created Date'),
      'field_type' => DS_FIELD_TYPE_FUNCTION,
      'function' => 'commerce_extra_ds_fields_order_date_created',
      'file' => $file,
  );
  $fields['commerce_order']['invoice_conditions'] = array(
      'title' => t('Invoice conditions'),
      'field_type' => DS_FIELD_TYPE_FUNCTION,
      'function' => 'commerce_extra_ds_fields_invoice_conditions',
      'file' => $file,
  );
return $fields;
}
?>
