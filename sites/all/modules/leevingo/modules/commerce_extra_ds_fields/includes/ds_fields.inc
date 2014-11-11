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

function commerce_extra_ds_fields_invoice_conditions($field) {
  $output = '<div class="invoice-conditions">';
  $output .= '<div class="invoice-conditions-general">';
  $output .=  'Algemene verkoopsvoorwaarden : algemeen is de Belgische wet toepasselijk';
  $output .= '</div>';
  $output .= '<div class="invoice-conditions-specific">';
  $output .=   'Bijzondere verkoopsvoorwaarden : facturen zijn contant betaalbaar te Dikkebus.
                Er wordt geen korting toegestaan. Bij achterstallige betaling na 30 dagen, is van
                rechtswege en zonder ingebrekestelling een interest verschuldigd van 1,5% per maand
                op het factuurbedrag vanaf de vervaldag. Elke begonnen maand wordt aanzien als
                een volledige maand. Inzake geschillen is alleen de rechtbank van Ieper bevoegd.';
  $output .= '</div>';
  $output .='</div>';

  return $output;
}

?>
