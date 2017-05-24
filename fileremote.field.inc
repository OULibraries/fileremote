<?php
/**
 * @file
 *
 */
function fileremote_field_info() {
  return array(
    // We name our field as the associative name of the array.
    'fileremote_upload' => array(
      'label' => t('Remote File (Dropbox)'),
      'description' => t('Remote file field.'),
      'default_widget' => 'fileremote_upload',
      'default_formatter' => 'fileremote_upload',
    ),
  );
}


/**
 * TBD Implements hook_field_validate().
 */

/**
 * Implements hook_field_is_empty().
 */
function fileremote_field_is_empty($item, $field) {
  return empty($item['filename']);
}



/**
 * Implements hook_field_widget_info().
 */
function fileremote_field_widget_info() {
  return array(
    'fileremote_upload' => array(
      'label' => t('Test field widget'),
      'field types' => array('fileremote_upload'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */

function fileremote_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  ddl($items);

  $value = isset($items[$delta]['filename']) ? $items[$delta]['filename'] : '';

  $widget = $element;

  $widget['#delta'] = $delta;
  $widget += array(
    '#type' => 'hidden',
    '#prefix' => '<div class="FILEREMOTE-WRAPPER"> <h1>Yep!</h1>',
    '#suffix' => '</div>',
    '#theme' => 'fileremote',
  );



  $element['filename'] = $widget;
  return $element;
}


function theme_fileremote_upload_widget($variables) {

  return "<h1> figured out how to theme a widget</h1>";



}






/**
 * Implements hook_field_formatter_info().
 */
function fileremote_field_formatter_info() {
  return array(
    'fileremote_upload' => array(
      'label' => t('Test field formatter'),
      'field types' => array('fileremote_upload'),
    ),
  );
}


/**
 * Implements hook_field_formatter_view().
 */
function fileremote_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  /* Formatter TBD */

  return $element;
}
