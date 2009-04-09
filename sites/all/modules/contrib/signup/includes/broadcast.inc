<?php
// $Id: broadcast.inc,v 1.1.2.1 2008/12/20 04:28:56 dww Exp $


/**
 * @file
 * Code related to sending signup broadcast messages.
 */

/**
 * Form builder for the signup broadcast form.
 *
 * @param $node
 *   The node that the broadcast form is being attached to.
 */
function signup_broadcast_form($form_state, $node) {
  $addresses = signup_get_email_addresses($node->nid);
  if (empty($addresses)) {
    $form['no_users'] = array(
      '#value' => t('No users have signup up for this %node_type.', array('%node_type' => node_get_types('name', $node->type))),
    );
    return $form;
  }

  $tokens = array('%node_title', '%node_url', '%user_mail', '%user_name', t('%cancel_signup_url (access to this link is denied to users without the "cancel own signups" permission)'));
  if (_signup_get_node_scheduler($node) != 'none') {
    $tokens = array_merge(array('%node_start_time'), $tokens);
  }
  if (module_exists('token')) {
    $token_text = t('Supported string substitutions: %tokens, and any tokens in the %replacement_tokens list.', array('%tokens' => implode(', ', $tokens), '%replacement_tokens' => t('Replacement tokens')));
  }
  else {
    $token_text = t('Supported string substitutions: %tokens.', array('%tokens' => implode(', ', $tokens)));
  }

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body'),
    '#required' => TRUE,
    '#description' => t('Body of the email message you wish to send to all users who have signed up for this %node_type. !token_description', array('%node_type' => node_get_types('name', $node->type), '!token_description' => $token_text)),
    '#rows' => 10,
  );

  if (module_exists('token')) {
    module_load_include('inc', 'signup', 'includes/token_help');
    _signup_token_help($form, 'message_tokens_fieldset');
  }

  $form['copy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send yourself a copy.'),
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  global $user;
  if (user_access('administer site configuration')) {
    $form['from'] = array(
      '#type' => 'textfield',
      '#title' => t('From'),
      '#required' => TRUE,
      '#default_value' => $user->mail,
      '#weight' => '-10',
    );
  }
  else {
    $form['from'] = array(
      '#value' => t('This message will be sent from: %from', array('%from' => $user->mail)),
      '#pre' => '<strong>',
      '#post' => '</strong>',
    );
  }
  return $form;
}

/**
 * Retrieve a list of all users who have signed up for a node.
 *
 * @param $nid
 *
 * @return An array of objects containing signup data
 */
function signup_get_email_addresses($nid) {
  $signup_data = array();
  $signups = db_query("SELECT u.uid, u.name, u.mail, s_l.sid, s_l.anon_mail, s_l.form_data FROM {signup_log} s_l INNER JOIN {users} u ON u.uid = s_l.uid WHERE s_l.nid = %d", $nid);
  while ($signup_entry = db_fetch_object($signups)) {
    $signup_data[] = $signup_entry;
  }
  return $signup_data;
}

/**
 * Send an email message to all those signed up to a node.
 *
 * @param $form_id
 * @param $form_values
 */
function signup_broadcast_form_submit($form, &$form_state) {
  global $user;
  $addresses = signup_get_email_addresses($form_state['values']['nid']);
  if (is_array($addresses)) {
    if (user_access('administer site configuration')) {
      $from = $form_state['values']['from'];
    }
    else {
      $from = $user->mail;
    }
    $node = node_load($form_state['values']['nid']);
    foreach ($addresses as $signup) {
      $user_mail = _signup_get_email($signup);
      $params = array(
        'subject' => $form_state['values']['subject'],
        'body' => $form_state['values']['message'],
        'node' => $node,
        'signup' => $signup,
      );
      if (module_exists('token')) {
        $params['body'] = token_replace($params['body'], 'node', $node);
      }
      $language = user_preferred_language($signup);
      drupal_mail('signup', 'signup_broadcast_mail', $user_mail, $language, $params, $from);
      watchdog('signup', 'Broadcast email for %title sent to %email.', array('%title' => $node->title, '%email' => $user_mail), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
    }
    if ($form_state['values']['copy']) {
      $sender_email = _signup_get_email($user);
      $signup_tokens = _signup_get_email_tokens($node, $user);
      $message = strtr($form_state['values']['message'], $signup_tokens);
      if (module_exists('token')) {
        // If the token.module is enabled, also handle any tokens it provides.
        $message = token_replace($message, 'node', $node);
      }
      $final_text = theme('signup_broadcast_sender_copy', $form_state['values']['message'], $message);
      $params = array(
        'subject' => $form_state['values']['subject'],
        'body' => $final_text,
        'ignore_tokens' => TRUE,
      );
      $language = user_preferred_language($user);
      drupal_mail('signup', 'signup_broadcast_mail', $sender_email, $language, $params, $from);
      watchdog('signup', 'Broadcast email copy for %title sent to %email.', array('%title' => $node->title, '%email' => $sender_email), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
      drupal_set_message(t('Sent a copy of this message to %email', array('%email' => $sender_email)));
    }
  }
  drupal_set_message(t('Message sent to all users who have signed up'));
}

