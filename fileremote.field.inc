<?php

/**
 * Implenments hook_field_info()
 */
function fileremote_field_info() {
  // Define a single field type
  return array(
    'fileremote_upload' => array(
      'label' => t('Remote File (Dropbox)'),
      'description' => t('Remote file field.'),
      'default_widget' => 'fileremote_upload',
      'default_formatter' => 'fileremote_upload',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function fileremote_field_is_empty($item, $field) {
  // right now, we're simply storing an id to match with a folder at Dropbox
  return empty($item['upload_id']);
}


/**
 * Implements hook_field_widget_info().
 */
function fileremote_field_widget_info() {
  // Define a edit widget
  return array(
    'fileremote_upload' => array(
      'label' => t('Fileremote Uploader Widget'),
      'field types' => array('fileremote_upload'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE  /* No field instance default values */
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function fileremote_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Generate or retrieve Upload ID value
  $value = isset($items[$delta]['upload_id']) ? $items[$delta]['upload_id'] : _fileremote_newid();

  // Add the fine-uploader library and set some variables
  $my_settings = array(
    'upload_id' => $value,
    'field' =>$element['#field_name']
  );
  drupal_add_js(array('fileremote' => $my_settings), 'setting');
  libraries_load('fine-uploader');

  // Upload ID Field
  $widget = $element;
  $widget['#delta'] = $delta;
  $widget += array(
    '#type' => 'hidden',
    '#default_value' =>$value,
  );
  $element['upload_id'] = $widget;

  // Fine  Uploader Markup
  $widgetFU = $element;
  $widgetFU['#delta'] = $delta;
  $widgetFU += array(
    '#type' => 'markup',
    '#theme' => 'fileremote',
  );
  $element['fileremote_fineupload'] = $widgetFU;

  return $element;
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
  /* Allow for display of multiple item ids. Same complexity, and it might be a
   * thing some day. */
  $element = array();
  foreach( $items as $delta => $item ) {
    $element[$delta] = array('#markup' => strip_tags($item['upload_id']));
  }
  return $element;
}


/**
 * Implements hook_field_settings_form().
 */
function fileremote_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $my_token = isset($settings['bearertoken']) ? $settings['bearertoken'] : "";

  $form['bearertoken'] = array(
    '#type' => 'textfield',
    '#title' => t('Bearer Token'),
    '#required' => TRUE,
    '#default_value' => $my_token,

  );

  $my_path = isset($settings['remotepath']) ? $settings['remotepath'] : "";

  $form['remotepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote Path'),
    '#required' => TRUE,
    '#default_value' => $my_path,
  );

  return $form;
}
