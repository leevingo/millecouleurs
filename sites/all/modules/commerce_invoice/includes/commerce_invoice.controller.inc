<?php

/**
 * @file
 * The controller for the invoice entity containing the CRUD operations.
 */

/**
 * The controller class for invoices contains methods for the order CRUD
 * operations. The load method is inherited from the default controller.
 */
class CommerceInvoiceEntityController extends DrupalCommerceEntityController {

    /**
     * Create a default invoice.
     *
     * @param array $values
     *   An array of values to set, keyed by property name
     *
     * @return
     *   An invoice object with all default fields initialized.
     */
    public function create(array $values = array()) {
        return (object) ($values + array(
            'invoice_id' => '',
            'invoice_number' => '',
            'supplier_tid' => '',
            'order_id' => 0,
            'uid' => '',
            'created' => '',
            'changed' => '',
            'type' => 'commerce_invoice',
                ));
    }

    /**
     * Saves an invoice.
     *
     * When saving an invoice, the function will automatically create an invoice number
     * based 
     *
     * @param $invoice
     *   The full invoice object to save.
     * @param $transaction
     *   An optional transaction object.
     *
     * @return
     *   The saved invoice object.
     */
    public function save($invoice, DatabaseTransaction $transaction = NULL) {
        try {
            $invoice->changed = REQUEST_TIME;

            // Inserting new invoice
            if (empty($invoice->invoice_id)) {
                $invoice->created = REQUEST_TIME;
                $invoice->invoice_number = $this->generate_invoice_id($invoice);

                $this->invoke('presave', $invoice);
                $invoice = $this->_save($invoice, $transaction);
                $this->invoke('insert', $invoice);
            } else {
                $this->invoke('presave', $invoice);
                $invoice = $this->_save($invoice, $transaction);
                $this->invoke('insert', $invoice);
            }

            return $invoice;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * Saves an invoice (helper function)
     *
     * @param $invoice
     *   The full invoice object to save.
     * @param $transaction
     *   An optional transaction object.
     *
     * @return
     *   The saved invoice object.
     */
    private function _save($invoice, DatabaseTransaction $transaction = NULL) {
        $transaction = isset($transaction) ? $transaction : db_transaction();
        try {
            if (empty($invoice->invoice_id)) {
                // Save the new invoice
                drupal_write_record('commerce_invoice', $invoice);
                field_attach_insert('commerce_invoice', $invoice);
            } else {
                drupal_write_record('commerce_invoice', $invoice, 'invoice_id');
                field_attach_update('commerce_invoice', $invoice);
            }
            // Ignore slave server temporarily to give time for the
            // saved invoice to be propagated to the slave.
            db_ignore_slave();


            return $invoice;
        } catch (Exception $e) {
            $transaction->rollback();
            watchdog_exception('commerce_invoice', $e);
            throw $e;
        }
    }
    /**
   * Generates invoice id
   * 
   * @return
   *   The generated invoice id
   */
    protected function generate_invoice_id($invoice) {
    // TODO: there is probably a better way to do this
    // Invoice supplier.
    $supplier_tid = $invoice->supplier_tid;
    // Invoice generation method
    dsm($supplier_tid, 'supplier');
    $method = variable_get('commerce_invoice_number_method', COMMERCE_INVOICE_METHOD_YEAR);
    // Get last invoice created
    if ($method == COMMERCE_INVOICE_METHOD_SUPPLIER) {
      $result = db_select('commerce_invoice', 'i')
          ->fields('i')
          ->condition('i.supplier_tid', $supplier_tid)
          ->orderBy('invoice_id', 'DESC')
          ->range(0, 1)
          ->execute();
      
      if ($record = $result->fetchAssoc()) {
        dsm($record, 'record');
        $last_number_for_supplier = $record['invoice_number'];
        $new_number_for_supplier = $last_number_for_supplier + 1;
        $return = str_replace('[invoice_number]', $new_number_for_supplier, $method);
        dsm($return, 'return');
        return $return;
      }
      else {
        // First invoice being generated
        $new_number_for_supplier = 1;
        $return = str_replace('[invoice_number]', $new_number_for_supplier, $method);
        dsm($return, 'return');
        return $return;
      }
      
      
    }
    else {
      $result = db_select('commerce_invoice', 'i')
          ->fields('i')
          ->orderBy('created', 'DESC')
          ->range(0, 1)
          ->execute();
      if ($record = $result->fetchAssoc()) {
        $last_number = $record['invoice_number'];
        switch ($method) {
          case COMMERCE_INVOICE_METHOD_INFINITE:
            $last_id = $record['invoice_id'];
            $id = $last_id + 1;
            $return = str_replace('[invoice_id]', $id, $method);
            break;
          case COMMERCE_INVOICE_METHOD_YEAR:
            // Are we in the same year as the last invoice
            $dash_pos = strpos($last_number, '-');
            $last_year = substr($last_number, 0, $dash_pos);
            if ($last_year == date('Y')) {
              // Get last invoice id
              $last_id = strstr($last_number, '-');
              $last_id = str_replace('-', '', $last_id);
              // Increment invoice id
              $id = $last_id + 1;
              $return = $last_year . '-' . $id;
            }
            else {
              // Reset invoice id to 1
              $return = date('Y') . '-1';
            }
            break;
          case COMMERCE_INVOICE_METHOD_MONTH:
            $parts = explode('-', $last_number);
            $last_year = $parts[0];
            $last_month = $parts[1];
            $last_id = $parts[2];
            $year = date('Y');
            $month = date('m');
            if ($last_year == $year) {
              if ($last_month == $month) {
                $id = $last_id + 1;
              }
              else {
                $id = 1;
              }
            }
            else {
              $id = 1;
            }
            $return = date('Y') . '-' . date('m') . $id;
            break;
        }
        return $return;
      }
      else {
        // First invoice being generated
        $id = 1;
        $result = str_replace('[invoice_id]', $id, $method);
        return date($result);
      }
    }
  }

  /**
     * Deletes multiple invoices by ID.
     *
     * @param $invoice_ids
     *   An array of invoice IDs to delete.
     * @param $transaction
     *  An optional transaction object.
     *
     * @return
     *   TRUE on success, FALSE otherwise.
     */
    public function delete($invoice_ids, DatabaseTransaction $transaction = NULL) {
        if (!empty($invoice_ids)) {
            $invoices = $this->load($invoice_ids, array());

            $transaction = isset($transaction) ? $transaction : db_transaction();

            try {
                db_delete('commerce_invoice')
                        ->condition('invoice_id', $invoice_ids, 'IN')
                        ->execute();

                // Reset the cache as soon as the changes have been applied.
                $this->resetCache($invoice_ids);

                foreach ($invoices as $id => $invoice) {
                    $this->invoke('delete', $invoice);
                }

                // Ignore slave server temporarily to give time for the
                // saved invoice to be propagated to the slave.
                db_ignore_slave();
            } catch (Exception $e) {
                $transaction->rollback();
                watchdog_exception('commerce_invoice', $e);
                throw $e;
            }

            // Clear the page and block and line_item_load_multiple caches.
            cache_clear_all();
            $this->resetCache();
        }

        return TRUE;
    }

    /**
     * Builds a structured array representing the entity's content.
     *
     * The content built for the entity will vary depending on the $view_mode
     * parameter.
     *
     * @param $entity
     *   An entity object.
     * @param $view_mode
     *   View mode, e.g. 'administrator'
     * @param $langcode
     *   (optional) A language code to use for rendering. Defaults to the global
     *   content language of the current request.
     * @return
     *   The renderable array.
     */
    public function buildContent($invoice, $view_mode = 'administrator', $langcode = NULL, $content = array()) {
        // Load the order this invoice is attached to.
        $order = commerce_order_load($invoice->order_id);

        $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        // Populate an array of all line items on the order.
        foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
            $supplier_term = $line_item_wrapper->commerce_product->field_product_supplier->value();
            // If the current line item doesn't belong to the right supplier...
            if ($supplier_term->tid != $invoice->supplier_tid) {
                // Remove the reference from the order and continue to the next value.
                $order_wrapper->commerce_line_items->offsetUnset($delta);
            }
        }
        $order->commerce_line_items[LANGUAGE_NONE] = array_values($order->commerce_line_items[LANGUAGE_NONE]);

        commerce_order_calculate_total($order);

        $content['invoice_number'] = array(
            '#type' => 'item',
            '#title' => t('Invoice number'),
            '#markup' => $invoice->invoice_number,
            '#prefix' => '<div class="invoice-number">',
            '#suffix' => '</div>',
        );

        $content['created'] = array(
            '#type' => 'item',
            '#title' => t('Date'),
            '#markup' => format_date($invoice->created, 'short'),
            '#prefix' => '<div class="invoice-created">',
            '#suffix' => '</div>',
        );

        $supplier_term = taxonomy_term_load($invoice->supplier_tid);
        $supplier_name = $supplier_term->name;
        $supplier_street = $supplier_term->field_term_supplier_street[LANGUAGE_NONE][0]['value'];
        $supplier_building_nr = $supplier_term->field_term_supplier_building_nr[LANGUAGE_NONE][0]['value'];
        $supplier_zip_code = $supplier_term->field_term_supplier_zip_code[LANGUAGE_NONE][0]['value'];
        $supplier_city = $supplier_term->field_term_supplier_city[LANGUAGE_NONE][0]['value'];
        $supplier_vat_number = $supplier_term->field_term_supplier_vat_number[LANGUAGE_NONE][0]['value'];
        $supplier_bank_acc_nr = $supplier_term->field_term_supplier_bank_acc_nr[LANGUAGE_NONE][0]['value'];
        $supplier_full =
                "<div>
                <span>" . $supplier_name . "</span><br>
                <span>" . $supplier_street . " " . $supplier_building_nr . "</span><br>
                <span>" . $supplier_zip_code . " " . $supplier_city . "</span><br>  
                <span>BTW: " . $supplier_vat_number . "</span><br> 
                <span>Bankrek: " . $supplier_bank_acc_nr . "</span><br>  
              </div>";
        $content['supplier'] = array(
            '#type' => 'item',
            '#title' => t('Supplier'),
            '#markup' => $supplier_full,
            '#prefix' => '<div class="invoice-supplier">',
            '#suffix' => '</div>',
        );

        $content['commerce_order'] = entity_build_content('commerce_order', $order, $view_mode, $langcode);


        return parent::buildContent($invoice, $view_mode, $langcode, $content);
    }

}

