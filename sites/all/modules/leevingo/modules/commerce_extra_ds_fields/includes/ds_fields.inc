<?php

/**
 * Functions to render display suite fields
 */
function commerce_extra_ds_fields_user_mail($field) {
  return $field['entity']->mail;
}

function commerce_extra_ds_fields_user_default_language($field) {
  return $field['entity']->language;
}

function commerce_extra_ds_fields_user_timezone($field) {
  return $field['entity']->timezone;
}

function commerce_extra_ds_fields_order_number($field) {
    $order_id = $field['entity']->order_id;
    $order = commerce_order_load($order_id);
    if (!empty($order->order_number)) {
      return $order->order_number;
    }
  }


function commerce_extra_ds_fields_order_date_created($field) {
    $order_id = $field['entity']->order_id;
    $order = commerce_order_load($order_id);
    if (!empty($order->order_number)) {
      $created = $order->created;
      return date('d-m-Y', $created);
    }
  }

?>
