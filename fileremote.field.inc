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
  return empty($item['upload_id']);
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
    '#prefix' => '<div class="FILEREMOTE-WRAPPER"> <h1>FILE REMOTE PREFIX</h1>',
    '#suffix' => '</div> <h1>FILE REMOTE SUFFIX</h1>',
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
  $element = array();

  /* Formatter TBD */

  return $element;
}


/**
 * Implements hook_field_settings_form().
 */
function fileremote_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form['bearertoken'] = array(
    '#type' => 'textfield',
    '#title' => t('Bearer Token'),
    '#required' => TRUE,
    '#default_value' => $settings['bearertoken'],

  );

  $form['remotepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote Path'),
    '#required' => TRUE,
    '#default_value' => $settings['remotepath'],
  );

  return $form;
}
