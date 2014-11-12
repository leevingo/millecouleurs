<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Email address'),
  'format callback' => 'addressfield_format_email_generate',
  'type' => 'phone',
  'weight' => -99,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_email_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    $format['email_block'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('custom-block')),
      '#weight' => 200,
    );
    $format['email_block']['email'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('addressfield-container-inline')),
    );
    $format['email_block']['email']['email'] = array(
      '#title' => t('Email'),
      '#attributes' => array('class' => array('email')),
      '#type' => 'textfield',
      '#tag' => 'span',
      '#default_value' => isset($address['email']) ? $address['email'] : '',
    );
  }
  else {
    // Add our own render callback for the format view
    $format['#pre_render'][] = '_leevingo_addressfield_email_render_address';
  }
}

