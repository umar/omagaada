<?php
// $Id: node_settings.inc,v 1.1.2.1 2008/12/20 04:28:56 dww Exp $


/**
 * @file
 * Code related to the per-node signup settings form.
 */

/**
 * Returns the form for the per-node signup settings.
 *
 * This is shared by the settings page and the node edit page.
 *
 * @param $node
 *   The fully loaded node object if we've got it.
 * @param $node_type
 *   The type of the node.  When creating new content, the caller can know the
 *   node type, even if $node is NULL.
 * @param $has_date
 *   Boolean flag indicating if this node (or site) has signup-aware
 *   date functionality, which is required for reminder emails to be in
 *   the form.
 *
 * @return
 *   The form array for the per-node signup settings.
 *
 */
function signup_node_settings_form($form_state, $node = NULL, $node_type = NULL, $has_date = FALSE) {
  if (module_exists('token')) {
    $signup_token_description = t('Supported string substitutions: %node_title, %node_url, %node_start_time, %user_name, %user_mail, %user_signup_info (additional information from the signup form), %cancel_signup_url (access to this link is denied to users without the "%cancel_own_signups" permission), and any tokens in the %replacement_tokens list.', array('%replacement_tokens' => t('Replacement tokens'), '%cancel_own_signups' => t('cancel own signups')));
  }
  else {
    $signup_token_description = t('Supported string substitutions: %node_title, %node_url, %node_start_time, %user_name, %user_mail, %user_signup_info (additional information from the signup form), and %cancel_signup_url (access to this link is denied to users without the "%cancel_own_signups" permission).', array('%cancel_own_signups' => t('cancel own signups')));
  }

  // Load the default admin form data for new nodes.
  if (!$node || !$node->signup) {
    $result = db_fetch_object(db_query("SELECT * FROM {signup} WHERE nid = 0"));
    $node->signup_forwarding_email = $result->forwarding_email;
    $node->signup_send_confirmation = $result->send_confirmation;
    $node->signup_confirmation_email = $result->confirmation_email;
    $node->signup_send_reminder = $result->send_reminder;
    $node->signup_reminder_days_before = $result->reminder_days_before;
    $node->signup_reminder_email = $result->reminder_email;
    $node->signup_close_signup_limit = $result->close_signup_limit;
  }

  $form['signup_forwarding_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Send signups to'),
    '#default_value' => $node->signup_forwarding_email,
    '#size' => 40, '#maxlength' => 64,
    '#description' => t('Email address where notification of new signups will be sent. Leave blank for no notifications.'),
  );
  $form['signup_send_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send confirmation'),
    '#default_value' => $node->signup_send_confirmation,
  );
  $form['signup_confirmation_email'] = array(
    '#type' => 'textarea',
    '#title' => t('Confirmation email'),
    '#default_value' => $node->signup_confirmation_email,
    '#cols' => 40, '#rows' => 6,
    '#description' => t('Email sent to user upon signup. !token_description', array('!token_description' => $signup_token_description)),
  );
  if (module_exists('token')) {
    module_load_include('inc', 'signup', 'includes/token_help');
    _signup_token_help($form, 'signup_confirmation_token_fieldset');
  }

  if ($has_date) {
    // Define a sub-tree to wrap the next 2 form elements together in an
    // inline div for better display.
    $form['signup_reminder'] = array(
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );
    $form['signup_reminder']['signup_send_reminder'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send reminder'),
      '#default_value' => $node->signup_send_reminder,
    );
    $options = array();
    for ($i = 1; $i <= 60; $i++) {
      $options[$i] = $i;
    }
    $node_type_name = isset($node_type) ? node_get_types('name', $node_type) : '';
    $form['signup_reminder']['signup_reminder_days_before'] = array(
      '#type' => 'select',
      '#default_value' => $node->signup_reminder_days_before,
      '#options' => $options,
      '#suffix' => !empty($node_type_name) ? t('day(s) before this %node_type', array('%node_type' => $node_type_name)) : t('day(s) before start time'),
    );
    $form['signup_reminder_email'] = array(
      '#type' => 'textarea',
      '#title' => t('Reminder email'),
      '#default_value' => $node->signup_reminder_email,
      '#cols' => 40, '#rows' => 6,
      '#description' =>  !empty($node_type_name) ? t('Email sent to user as a reminder before the %node_type starts. !token_description', array('%node_type' => $node_type_name, '!token_description' => $signup_token_description)) : t('Email sent to user as a reminder before the start time. !token_description', array('!token_description' => $signup_token_description)),
    );
    if (module_exists('token')) {
      module_load_include('inc', 'signup', 'includes/token_help');
      _signup_token_help($form, 'signup_reminder_token_fieldset');
    }
  }

  $form['signup_close_signup_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Signup limit'),
    '#default_value' => $node->signup_close_signup_limit,
    '#size' => 4, '#maxlength' => 8,
    '#description' => t('Maximum number of users who can sign up before signups are automatically closed. If set to 0, there is no limit.'),
  );
  $form['signup'] = array('#type' => 'hidden', '#value' => 1);

  return $form;
}

