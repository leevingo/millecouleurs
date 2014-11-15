<?php

/**
 * Field handler to present an invoice's operations links.
 */
class commerce_invoice_handler_field_invoice_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['invoice_id'] = 'invoice_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to edit and delete operation links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $invoice_id = $this->get_value($values, 'invoice_id');

    // Get the operations links.
    $links = menu_contextual_links('commerce-invoice', 'admin/commerce/invoices', array($invoice_id));

    if (!empty($links)) {
      // Add the destination to the links if specified.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          // Only include the destination for the edit and delete forms.
          $link['query'] = drupal_get_destination();
        }
      }

      drupal_add_css(drupal_get_path('module', 'commerce_order') . '/theme/commerce_order.admin.css');
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
