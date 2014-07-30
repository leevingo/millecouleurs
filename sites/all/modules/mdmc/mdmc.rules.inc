<?php

/**
 * @file
 * Rules integration for mdmc invoices.
 *
 * @addtogroup rules
 * @{
 */
 
/**
 * Implements hook_rules_action_info().
 */
function mdmc_rules_action_info() {
  $actions = array();

  $actions['mdmc_generate'] = array(
    'label' => t('Generate mdmc invoice'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
      ),
    ),
    'group' => t('MDMC Commerce Invoice'),
    'callbacks' => array(
      'execute' => 'mdmc_commerce_invoice_rules_generate',
    ),
  );
  
  return $actions;
}

function mdmc_commerce_invoice_rules_generate($order) {
  
  $available_suppliers = array();
  
  // Get all the suppliers for the product (line items) in the order.
  $line_items = field_get_items('commerce_order', $order, 'commerce_line_items');
  foreach ($line_items as $line_item) {
    $item = commerce_line_item_load($line_item['line_item_id']);
    $product_items = field_get_items('commerce_line_item', $item, 'commerce_product');
    $product = commerce_product_load($product_items[0]['product_id']);
    $supplier_items = field_get_items('commerce_product', $product, 'field_product_supplier');
    $supplier_term = taxonomy_term_load($supplier_items[0]['tid']);
    $available_suppliers[$supplier_items[0]['tid']] = $supplier_term->name;
  }
  
  
  foreach (array_keys($available_suppliers) as $tid) {
    $invoice = commerce_invoice_new($order->uid, $order->order_id);
    $invoice->invoice_id = NULL;
    $invoice->supplier_tid = $tid;
    commerce_invoice_save($invoice);  
  }
}

  
