<?php
/**
 * @file
 * Default print module template
 *
 * @ingroup print
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $print['language']; ?>" xml:lang="<?php print $print['language']; ?>">
  <head>
    <?php print $print['head']; ?>
    <?php print $print['base_href']; ?>
    <title><?php print $print['title']; ?></title>
    <?php print $print['scripts']; ?>
    <?php print $print['sendtoprinter']; ?>
    <?php print $print['robots_meta']; ?>
    <?php print $print['favicon']; ?>
    <?php /* print $print['css']; */ ?>
    <style type="text/css" media="all">
      /**
       * GLOBAL
       */
      body {
        color: #000;
        background: #fff;
        font: normal 12px "Lucida Grande", "Lucida Sans Unicode", sans-serif;
      }
      /**
       * Tables.
       */
      table {
        width: 100%;
        font-size: 0.923em;
        margin: 0 0 10px;
        border: 1px solid #bebfb9;
      }
      table td,
      table th {
        vertical-align: middle;
        padding: 8px 10px;
        border: 0;
        color: #000;
      }
      tr.even,
      tr.odd {
        border-width: 0 1px 0 1px;
        border-style: solid;
        border-color: #bebfb9;
        background: #f3f4ee;
      }
      tr.odd {
        background: #fff;
      }
      table th {
        text-transform: uppercase;
        background: #e1e2dc;
        font-weight: normal;
        border-width: 1px;
        border-style: solid;
        border-color: #bebfb9;
        padding: 3px 10px;
      }
      /**
       * Exception for webkit bug with the right border of the last cell
       * in some tables, since it's webkit only, we can use :last-child
       */
      tr td:last-child {
        border-right: 1px solid #bebfb9; /* LTR */
      }

      /* 8.6 Invoice */
      .print-wrapper label,
      .print-wrapper .field-label {
        font-weight: bold;
      }
      .print-wrapper label,
      .print-wrapper .field-label,
      .print-wrapper .field-items,
      .print-wrapper .field-item {
        display: inline;
      }
      .print-wrapper {
        max-width: 665px;
        margin: 0 auto;
      }
      .print-header {
        position: relative;
        height: 150px;
      }
      .print-logo-wrapper {
        top: 0;
        left: 0;
        height: 150px;
        width: 150px;
        position: absolute;
      }
      .print-logo {
        width: 100%;
        height: auto;
      }
      .print-site_name {
        top: 60px;
        left: 150px;
        height: 150px;
        position: absolute;
        font-size: 30px;
        font-weight: bolder;
      }
      .print-title {
        position: relative;
        font-size: 25px;
        font-weight: bolder;
        text-align: center;
        padding: 10px 0;
        border-bottom: 1px solid black;
        border-top: 1px solid black;
      }
      .print-content .content {
        position: relative;
        font-size: 12px;
      }
      .print-content .form-item,
      .print-content .field-name-number,
      .print-content .field-name-commerce-customer-billing,
      .print-content .field-name-date-created {
        margin: 10px 0;
        padding: 0;
      }
      .print-content .invoice-number {
        position: absolute;
        left: 0px;
        top: 170px;
      }
      .print-content .invoice-created {
        position: absolute;
        left: 0px;
        top: 187px;
      }
      .print-content .invoice-supplier  {
        position: absolute;
        left: 0px;
        top: 0px;
      }
      .print-content .field-name-commerce-customer-billing {
        position: absolute;
        left: 50%;
        top: 0px;
      }
      .print-content .field-name-commerce-customer-shipping {
        position: absolute;
        left: 50%;
        top: 150px;
      }
      .print-content .field-name-number {
        position: absolute;
        left: 0px;
        top: 134px;
      }
      .print-content .field-name-date-created {
        position: absolute;
        left: 0px;
        top: 150px;
      }
      .print-content .field-name-commerce-line-items {
        position: relative;
        padding-top: 265px;
      }
      .view-line-items-of-processing-orders #edit-actions,
      .view-line-items-of-processing-orders thead,
      .view-line-items-of-processing-orders caption {
        display: none;
      }
      .view-line-items-of-processing-orders td {
        padding: 0 0 0 10px;
      }
      .view-line-items-of-processing-orders td .form-type-textfield {
        margin: 0;
      }

      .view-line-items-of-processing-orders .views-field-line-item-title {
        width: 250px;
      }
      .view-line-items-of-processing-orders .views-field-rendered-entity {
        width: 250px;
      }
      .print-content .field-name-invoice-conditions {
        padding-top: 10px;
      }
    </style>
  </head>
  <body>
    <div class="print-wrapper">
      <?php if ($print['node']->title != 'Processing orders') : ?>
        <div class="print-header">
          <div class="print-logo-wrapper"><?php print $print['logo']; ?></div>
          <div class="print-site_name"><?php print $site_name; ?></div>
        </div>
      <div class="print-title"><?php print t('Invoice'); ?></div>
      <?php endif; ?>
      <div class="print-content"><?php print $print['content']; ?></div>
      <div class="print-footer"><?php print $print['footer_message']; ?></div>
      <div class="print-source_url"><?php print $print['source_url']; ?></div>
      <div class="print-links"><?php print $print['pfp_links']; ?></div>
      <?php print $print['footer_scripts']; ?>
    </div>
  </body>
</html>
