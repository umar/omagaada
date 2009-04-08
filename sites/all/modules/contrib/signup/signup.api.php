<?php
// $Id: signup.api.php,v 1.1 2008/12/19 18:05:12 dww Exp $


/**
 * @file
 * This file documents the hooks invoked by the Signup module.
 */

/**
 * Hook invoked when a signup is being canceled.
 *
 * At the time this hook is invoked the record about the signup in the
 * {signup_log} table still exists, but the node has already had its signup
 * total decremented.
 *
 * @param $node
 *   The fully-loaded node object that the signup is being canceled from.
 * @param $signup
 *   An object containing all the known information about the signup being
 *   canceled. Contains all the data from the {signup_log} row representing
 *   the canceled signup. See the schema definition for descriptions of each
 *   field and what they represent.
 */
function hook_signup_cancel($signup, $node) {
  $info = array();
  $info[] = t('Signup ID: @sid', array('@sid' => $signup->sid));
  $info[] = t('Node ID: @nid', array('@nid' => $signup->nid));
  $info[] = t('User ID: @uid', array('@uid' => $signup->uid));
  $info[] = t('Email address for anonymous signup: @anon_mail', array('@anon_mail' => $signup->anon_mail));
  $info[] = t('Date/time when the signup was created: @signup_time', array('@signup_time' => $signup->signup_time));
  $form_data = unserialize($signup->form_data);
  $info[] = t('Custom signup form data: %signup_form_data', array('%signup_form_data' => theme('signup_custom_data_email', $form_data)));
  $info[] = t('Attendance record: %attended', array('%attended' => theme('signup_attended_text', $signup->attended)));

  drupal_set_message(theme('item_list', $info, t('Signup canceled for %node_title', array('%node_title' => $node->title))));
}

