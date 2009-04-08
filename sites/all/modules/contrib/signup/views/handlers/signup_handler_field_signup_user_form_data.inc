<?php
// $Id: signup_handler_field_signup_user_form_data.inc,v 1.1 2008/11/12 01:00:58 dww Exp $

/**
 * Field handler to display custom (site-specific) signup form data.
 */
class signup_handler_field_signup_user_form_data extends views_handler_field_prerender_list {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['form_data_fieldname'] = array(
      '#type' => 'textfield',
      '#title' => t(' Field Name'),
      '#description' => t('If you want to display a single field from signup.theme, enter its name here, otherwise, all fields will be displayed.'),
      '#default_value' => isset($this->options['form_data_fieldname']) ? $this->options['form_data_fieldname'] : '',
    );
  }

  function pre_render($values) {
    if (!$this->options['form_data_fieldname']) {
      foreach ($values as $key => $result) {
        $form_data = unserialize($result->{$this->field_alias});
        if ($form_data) {
          foreach ($form_data as $key => $value) {
            $this->items[$result->{$this->field_alias}][$key] = check_plain($key .': '. $value);
          }
        }
      }
    }
  }

  function render($values) {
    if ($this->options['form_data_fieldname']) {
      $form_data = unserialize($values->{$this->field_alias});
      return check_plain($form_data[$this->options['form_data_fieldname']]);
    }
    else {
      return parent::render($values);
    }
  }
}

