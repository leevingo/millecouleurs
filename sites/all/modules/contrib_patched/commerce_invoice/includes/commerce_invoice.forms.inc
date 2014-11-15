<?php

/**
 * @file
 * Forms for creating / editing and deleting invoices.
 */


/**
 * Form callback: edit an invoice.
 *
 * @param $invoice
 *   The order object to edit through the form.
 */
function commerce_invoice_ui_invoice_form($form, &$form_state, $invoice) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_invoice') . '/includes/commerce_invoice.forms.inc';

  $form_state['commerce_invoice'] = $invoice;
  field_attach_form('commerce_invoice', $invoice, $form, $form_state);

  $form['#validate'][] = 'commerce_invoice_ui_invoice_form_validate';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($invoice->invoice_id) ? t('Update project') : t('Save project'),
    '#weight' => 50,
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  return $form;

}

/**
 * Validation callback for commerce_invoice_ui_invoice_form().
 */
function commerce_invoice_ui_invoice_form_validate($form, &$form_state) {
  $invoice = $form_state['commerce_invoice'];
  // Notify field widgets to validate their data.
  field_attach_form_validate('commerce_invoice', $invoice, $form, $form_state);
}

/**
 * Submit callback for commerce_invoice_invoice_form().
 */
function commerce_invoice_ui_invoice_form_submit($form, &$form_state) {
  $invoice = $form_state['commerce_invoice'];
  commerce_invoice_save($invoice);
  drupal_set_message(t('The invoice: @id has been saved.', array('@id' => $invoice->invoice_id)));
  $form_state['redirect'] = 'admin/commerce/invoices';
}

/**
 * Form callback: confirmation form for deleting an invoice.
 *
 * @param $invoice
 *   The invoice object to delete through the form.
 *
 * @return
 *   The form array to add or edit an invoice.
 *
 * @see confirm_form()
 */
function commerce_invoice_ui_invoice_delete_form($form, &$form_state, $invoice) {
  $form_state['invoice'] = $invoice;

  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_invoice') . '/includes/commerce_invoice.forms.inc';

  $form['#submit'][] = 'commerce_invoice_ui_invoice_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete invoice @number?', array('@number' => $invoice->invoice_number)),
    '',
    '<p>' . t('Deleting this invoice cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_invoice_ui_invoice_delete_form().
 */
function commerce_invoice_ui_invoice_delete_form_submit($form, &$form_state) {
  $invoice = $form_state['invoice'];

  if (commerce_invoice_delete($invoice->invoice_id)) {
    drupal_set_message(t('Invoice @number has been deleted.', array('@number' => $invoice->invoice_number)));
    watchdog('commerce_invoice', 'Deleted invoice @number.', array('@number' => $invoice->invoice_number), WATCHDOG_NOTICE);
  }
  else {
    drupal_set_message(t('Invoice @number could not be deleted.', array('@number' => $invoice->invoice_number)), 'error');
  }
}
