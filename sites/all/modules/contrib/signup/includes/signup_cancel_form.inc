<?php
// $Id: signup_cancel_form.inc,v 1.1.2.1 2008/12/19 01:00:35 dww Exp $


/**
 * @file
 * Code for the signup cancel form.
 */

/**
 * Submit handler for the cancel signup form.
 *
 * @param $form
 *   The form being submitted.
 * @param $form_state
 *   The state of the submitted form, including the values.
 */
function signup_cancel_form_submit($form, &$form_state) {
  signup_cancel_signup($form_state['values']['sid']);
}

/**
 * Form builder for the cancel signup form.
 *
 * @param $form_state
 *   The form state from the FormAPI.
 * @param $sid
 *   The signup ID (sid) to cancel (primary key of the {signup_log} table).
 */
function signup_cancel_form(&$form_state, $sid) {
  $form['sid'] = array('#type' => 'value', '#value' => $sid);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Cancel signup'));
  return $form;
}

