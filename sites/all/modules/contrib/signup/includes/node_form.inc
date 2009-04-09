<?php
// $Id: node_form.inc,v 1.4 2008/12/14 07:36:43 dww Exp $


/**
 * @file
 * Signup-related code needed while editing a node.
 */

/**
 * Save signup-related information when a node is created or edited.
 *
 * This is a helper function invoked via signup_nodeapi().  It ensures that
 * the currently selected signup values are properly saved into the database.
 * If the node is signup-enabled, the per-node configuration values are saved
 * to the {signup} table. If signups are disabled, the record from {signup} is
 * cleared.  If the signup administrator editing the node decided to remove
 * all signup data, all the records from the {signup_log} table for this node
 * are also removed.  This function is also responsible for testing if the
 * node * has a start time and if the autoclose period has already begun, in
 * which case signups are closed.  Finally, if the signup limit was changed
 * while editing the node, the function compares the limit against the current
 * total number of signups and opens or closes signups as appropriate.
 *
 * @param $node
 *   The node object given to signup_nodeapi().
 * @param $op
 *   The hook_nodeapi() operation, either 'insert' or 'update'.
 *
 * @return
 *   Nothing, this function is expected to update the database.
 *
 * @see signup_nodeapi()
 */
function signup_save_node($node, $op) {
  // See if the form indicates that signups are enabled on this node.
  if (isset($node->signup_enabled)) {
    if ($node->signup_enabled == 1) {
      $values = array(
        $node->signup_forwarding_email,
        $node->signup_send_confirmation,
        $node->signup_confirmation_email,
        $node->signup_close_signup_limit,
      );
      // If we're dealing with a node that doesn't have a start time, these
      // fields are missing from the signup settings form, so we can't assume
      // they're defined.
      $values[] = isset($node->signup_send_reminder) ? $node->signup_send_reminder : 0;
      $values[] = isset($node->signup_reminder_days_before) ? $node->signup_reminder_days_before : 0;
      $values[] = isset($node->signup_reminder_email) ? $node->signup_reminder_email : '';
    }
  }
  elseif ($op == 'insert' && variable_get('signup_node_default_state_'. $node->type, 'disabled') == 'enabled_on') {
    // The form doesn't include any information about signups, but the node
    // type is signup-enabled. This would happen if a user without any signup
    // admin permissions creates a node that has been signup-enabled based on
    // the node type. In this case, we use the site-wide default signup
    // settings.
    $defaults = db_fetch_object(db_query("SELECT * from {signup} WHERE nid = 0"));
    $values = array(
      $defaults->forwarding_email,
      $defaults->send_confirmation,
      $defaults->confirmation_email,
      $defaults->close_signup_limit,
      $defaults->send_reminder,
      $defaults->reminder_days_before,
      $defaults->reminder_email,
    );
  }

  if (isset($values)) {
    // If $values is set, we need to save them to the DB.

    // Before we update the DB, see if the limit is changing, so we can take
    // appropriate action after we update to the new settings.
    $has_signup_record = FALSE;
    $limit_changed = FALSE;
    if ($op == 'update') {
      $signup = db_fetch_object(db_query("SELECT close_signup_limit, status FROM {signup} WHERE nid = %d", $node->nid));
      if ($signup !== FALSE) {
        $cur_limit = $signup->close_signup_limit;
        $node->signup_status = $signup->status;
        $has_signup_record = TRUE;
        $limit_changed = $cur_limit != $node->signup_close_signup_limit;
      }
      else {
        // Newly signup-enabled node.
        $node->signup_status = 1;
      }
    }

    // See if we need to update an existing record or insert a new one.
    // Either way, we always the nid as the final value. The nid will either
    // be used as the last column in the INSERT, or the argument to the WHERE
    // clause for the UPDATE.
    $values[] = $node->nid;
    if ($has_signup_record) {
      db_query("UPDATE {signup} SET forwarding_email = '%s', send_confirmation = %d, confirmation_email = '%s', close_signup_limit = %d, send_reminder = %d, reminder_days_before = %d, reminder_email = '%s' WHERE nid = %d", $values);
    }
    else {
      db_query("INSERT INTO {signup} (forwarding_email, send_confirmation, confirmation_email, close_signup_limit, send_reminder, reminder_days_before, reminder_email, nid) VALUES ('%s', %d, '%s', %d, %d, %d, '%s', %d)", $values);
    }
    if (_signup_node_completed($node) && !empty($node->signup_status)) {
      // If this is an time-based node, and it's already past the close in
      // advance time, and signups are still open, close them now (and don't
      // consider the limit for changing the status).
      signup_close_signup($node->nid);
      drupal_set_message(t('%node_type start time is already past the signup close-in-advance time, signups now closed.', array('%node_type' => node_get_types('name', $node->type))));
    }
    elseif ($limit_changed) {
      $node->signup_total = db_result(db_query("SELECT COUNT(*) FROM {signup_log} WHERE nid = %d", $node->nid));
      _signup_check_limit($node, 'limit');
    }
  }
  elseif ($op == 'update' && isset($node->signup_enabled)) {
    // $values was not set, because signups are now disabled on this node.
    switch ($node->signup_enabled) {
      case 2: // Disabled, and delete {signup_log}, too
        db_query("DELETE FROM {signup_log} WHERE nid = %d", $node->nid);
        // No break, fall through and remove from {signup} too.
      case 0: // Disabled, but leave {signup_log} alone
        db_query("DELETE FROM {signup} WHERE nid = %d", $node->nid);
        break;
    }
  }
}

/**
 * Alters the node form to inject the appropriate per-node signup settings.
 */
function signup_alter_node_form(&$form, &$form_state, $form_id) {
  global $user;

  // Load the node if it already exists.
  if (!empty($form['nid']['#value'])) {
    $node = node_load($form['nid']['#value']);
  }
  else {
    $node = NULL;
  }
  $node_type = $form['type']['#value'];

  $signup_type_default = variable_get('signup_node_default_state_'. $node_type, 'disabled');
  if (!empty($node)) {
    $node_scheduler = _signup_get_node_scheduler($node);
  }
  else {
    $node_scheduler = _signup_get_node_type_scheduler($node_type);
  }
  $node_has_date = $node_scheduler != 'none';

  // If signups are possible, and the current user either has the global
  // 'administer all signups' permission or has the 'administer signups
  // for own content' permission and is creating new content or editing
  // their own content, add a fieldset for signup-related settings.

  // Signups are possible if they're not explicitly disallowed for this
  // node type, or if this node is already signup-enabled (in case an
  // admin erroneously marks a node-type to disallow signups when there
  // are already nodes of that type with signups enabled).
  $signups_possible = $signup_type_default != 'disabled' || (!empty($node) && !empty($node->signup));
  $admin_all = user_access('administer all signups');
  $admin_own = user_access('administer signups for own content') && (empty($node) || ($node->uid == $user->uid));
  if ($signups_possible && ($admin_all || $admin_own)) {
    $form['signup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Signup settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 30,
    );

    // Figure out what the options should be.  If there are already
    // people signed-up for this node, we need a 3rd choice: disable
    // signups and remove all signup data.
    $has_signups = !empty($node) && db_result(db_query("SELECT COUNT(*) from {signup_log} WHERE nid = %d", $node->nid));
    $radio_options[1] = t('Enabled');
    if ($has_signups) {
      $radio_options[0] = t('Disabled, but save existing signup information');
      $radio_options[2] = t('Disabled, and remove all signup information') .' <strong>('. t('This can not be undone, use with extreme caution!') .')</strong>';
    }
    else {
      $radio_options[0] = t('Disabled');
    }

    // Figure out what the default selection for signups should be.
    if (isset($node->signup)) {
      $default_option = $node->signup;
    }
    else {
      $default_option = $signup_type_default == 'enabled_on' ? 1 : 0;
    }
    if ($default_option == 1) {
      $hint = t('If enabled, you can control whether users may sign up by visiting the !signups tab and toggling if signups are %open or %closed for this %node_type.', array('!signups' => !empty($node) ? l(t('Signups'), 'node/'. $node->nid .'/signups') : theme('placeholder', t('Signups')), '%open' => t('open'), '%closed' => t('closed'), '%node_type' => node_get_types('name', $node_type)));
    }
    else {
      $hint = '';
    }
    // Add the form element to toggle if signups are allowed.
    $form['signup']['signup_enabled'] = array(
      '#type' => 'radios',
      '#options' => $radio_options,
      '#default_value' => $default_option,
      '#description' => $hint .'<div class="js-hide">'. t('If disabled, all of the other signup settings will be ignored.') .'</div>',
      '#prefix' => '<div class="signup-allow-radios">',
      '#suffix' => '</div>',
    );

    // If JS is enabled, system.css will hide all the settings on page
    // load if signups aren't enabled on this node.
    $settings_class = "signup-node-settings";
    if ($default_option != 1) {
      $settings_class .= " js-hide";
    }

    // Add the actual settings.  We wrap this in a div to make it easy
    // to use jQuery to hide these settings when signups are disabled.
    drupal_add_js(drupal_get_path('module', 'signup') .'/js/node_form.js');
    $form['signup']['node_settings'] = array(
      '#prefix' => '<div class="'. $settings_class .'">',
      '#suffix' => '</div>',
    );
    module_load_include('inc', 'signup', 'includes/node_settings');
    $form['signup']['node_settings']['settings'] = signup_node_settings_form(array(), $node, $node_type, $node_has_date);
  }
}

