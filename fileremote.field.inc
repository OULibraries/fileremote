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

  $value = isset($items[$delta]['filename']) ? $items[$delta]['filename'] : '';

  $widget = $element;

  libraries_load('fine-uploader');

  $widget['#delta'] = $delta;
  $widget += array(
    '#type' => 'hidden',
    '#prefix' => '<div class="FILEREMOTE-WRAPPER"> <h1>FILE REMOTE PREFIX</h1>',
    '#suffix' => '</div> <h1>FILE REMOTE SUFFIX</h1>',
    '#theme' => 'fileremote',
  );



  $element['filename'] = $widget;
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
  );

  $form['remotepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote Path'),
  );

  return $form;
}
