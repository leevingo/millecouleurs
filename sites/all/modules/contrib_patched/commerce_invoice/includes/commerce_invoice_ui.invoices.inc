<?php

/**
 * @file
 * Page callbacks and form builder functions for administering invoices.
 */


/**
 * Form callback: edit the global invoice settings.
 */
function commerce_invoice_settings_form($form, &$form_state) {
  $form['commerce_invoice_number_method'] = array(
    '#type' => 'radios',
    '#title' => t('Invoice number creation method'),
    '#description' => t('This determines the way invoice numbers are generated.'),
    '#default_value' => variable_get('commerce_invoice_number_method', COMMERCE_INVOICE_METHOD_YEAR),
    '#options' => array(
      COMMERCE_INVOICE_METHOD_INFINITE => t('Infinite (one single number, that is never reset, and incremented at each invoice generation)'),
      COMMERCE_INVOICE_METHOD_SUPPLIER => t('Supplier generation'),
      COMMERCE_INVOICE_METHOD_YEAR => t('Reset every year, with an id incremented at each invoice generation (@invoice_number)', array('@invoice_number' => date('Y').'-[invoice_id]')),
      COMMERCE_INVOICE_METHOD_MONTH => t('Reset every month, with an id incremented at each invoice generation (@invoice_number)', array('@invoice_number' => date('Y-m').'-[invoice_id]'))
    )
  );

  return system_settings_form($form);
}


/**
 * Form callback wrapper: edit an invoice.
 *
 * @param $invoice
 *   The invoice object to edit through the form.
 *
 * @see commerce_invoice_ui_invoice_form()
 */
function commerce_invoice_ui_invoice_form_wrapper($invoice) {

  // Include the forms file from the Order module.
  module_load_include('inc', 'commerce_invoice', 'includes/commerce_invoice.forms');
  return drupal_get_form('commerce_invoice_ui_invoice_form', $invoice);
}

/**
 * Form callback wrapper: confirmation form for deleting an invoice.
 *
 * @param $invoice
 *   The invoice object to delete through the form.
 *
 * @see commerce_invoice_ui_invoice_delete_form()
 */
function commerce_invoice_ui_invoice_delete_form_wrapper($invoice) {
  // Include the forms file from the invoice module.
  module_load_include('inc', 'commerce_invoice', 'includes/commerce_invoice.forms');
  return drupal_get_form('commerce_invoice_ui_invoice_delete_form', $invoice);
}