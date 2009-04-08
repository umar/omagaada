<?php
// $Id: node.inc,v 1.1 2008/11/12 23:22:19 dww Exp $


/**
 * @file
 * Theme functions when viewing a signup-enabled node.
 */

/**
 * Controls the output displayed if this node is closed for signups.
 *
 * @param $node
 *   The fully loaded node object.
 * @param $current_signup
 *   If the user already signed up, an HTML representation of their current
 *   signup information, otherwise an empty string.
 *
 * @return
 *   Themed output to display for a node with closed signups.
 *
 * @see _signup_node_output()
 * @see _signup_print_current_signup()
 */
function theme_signup_signups_closed($node, $current_signup = '') {
  $output = '<h3>'. t('Signups closed for this %node_type', array('%node_type' => node_get_types('name', $node->type))) .'</h3>';
  $output .= $current_signup;
  return $output;
}

/**
 * Controls the output for anonymous users who can't signup.
 *
 * @param $anon_login_text
 *   The translated HTML help text telling users to login (and if allowed on
 *   this site, register) so they can signup, including login/register links.
 *
 * @return
 *   The themed HTML to display the login (and maybe register) help text.
 */
function theme_signup_anonymous_user_login_text($anon_login_text) {
  if (!empty($anon_login_text)) {
    return '<div class="signup_anonymous_login">'. $anon_login_text .'</div>';
  }
}

/**
 * Return HTML desired at the top of the signup output for a node.
 *
 * @param $node
 *   The fully loaded node object to generate a header for.
 *
 * @return
 *   HTML to display at the top of the signup output.
 *
 * @see _signup_node_output()
 */
function theme_signup_node_output_header($node) {
  return '<a name="signup"></a>';
}

/**
 * Controls the output of the users signup data and optional cancel button.
 *
 * @param $signup_data
 *   Array containing information about the user's signup.  Contains:
 *   'signup_timestamp' - Integer timestamp when the user signed up.
 *   'custom_data' - Array containing the user's custom signup data.
 * @param $cancel_signup_form
 *   Optional HTML for a "Cancel signup" button if the user is allowed.
 *
 * @return
 *   Themed output containing the user's current signup information.
 */
function theme_signup_current_signup($signup_data, $cancel_signup_form = '') {
  $output = theme('signup_custom_data_table', $signup_data['custom_data']);
  $output .= $cancel_signup_form;
  return $output;
}

/**
 * Renders custom signup user data into a table.
 *
 * @param $data
 *   Array of custom user signup data.
 *
 * @return
 *   The themed table with custom signup user data.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_custom_data_table($data) {
  $output = '';
  if (is_array($data)) {
    $header = array(array('data' => t('Your signup information'), 'colspan' => 2));
    $rows = theme('signup_custom_data_rows', $data);
    $output .= theme('table', $header, $rows);
  }
  return $output;
}

/**
 * Renders custom signup user data into table rows.
 *
 * WARNING: This theme function is recursive (it calls itself for
 * nested data), so if you override it, be sure not to change the part
 * where it does "call_user_func(__FUNCTION__)".
 *
 * @param $data
 *   Array of custom user signup data.
 *
 * @return
 *   An array of table rows.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_custom_data_rows($data) {
  $rows = array();
  // Loop through each first level element.
  foreach ($data as $key => $value) {
    if (is_array($value)) {
      // Element is nested, render it recursively.
      // Instead of the overhead of theme(), just call ourself directly.
      $rows += call_user_func(__FUNCTION__, $value);
    }
    else {
      $rows[] = array($key .':', check_plain($value));
    }
  }
  return $rows;
}

