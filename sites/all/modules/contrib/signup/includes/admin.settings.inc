<?php
// $Id: admin.settings.inc,v 1.2.2.1 2008/12/20 06:31:27 dww Exp $


/**
 * @file
 * Code required for the signup settings page (admin/settings/signup).
 */

/**
 * Form builder for the settings page under admin/setttings/signup
 */
function signup_settings_form() {
  module_load_include('inc', 'signup', 'includes/node_settings');
  if (signup_site_has_dates()) {
    $form['signup_close_early'] = array(
      '#title' => t('Close x hours before'),
      '#type' => 'textfield',
      '#default_value' => variable_get('signup_close_early', 1),
      '#size' => 5, '#maxlength' => 10,
      '#description' => t('The number of hours before the event which signups will no longer be allowed. Use negative numbers to close signups after the event start (example: -12).'),
    );
  };
  $form['node_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default signup information'),
    '#description' => t('New signup-enabled nodes will start with these settings.'),
    '#collapsible' => TRUE,
  );
  $form['node_defaults']['signup_node_settings_form'] = signup_node_settings_form(array(), NULL, NULL, signup_site_has_dates());

  $form['adv_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['adv_settings']['signup_date_format'] = array(
    '#title' => t('Format string for displaying signup-related dates'),
    '#type' => 'select',
    '#options' => array(
      'small' => t('Small'),
      'medium' => t('Medium'),
      'large' => t('Large'),
    ),
    '#default_value' => variable_get('signup_date_format', 'small'),
    '#description' => t('Whenever this module needs to print a date (both in the administrative interface, and in the various e-mail messages it can send), this setting controls which date format string to use. The format strings are defined at the <a href="!settings_url">Date and time settings page</a>.', array('!settings_url' => url('admin/settings/date-time'))),
  );
  $form['adv_settings']['signup_form_location'] = array(
    '#title' => t('Location of the signup form'),
    '#type' => 'radios',
    '#options' => array(
      'node' => t('At the bottom of each node'),
      'tab' => t('Under the %signups tab', array('%signups' => t('Signups'))),
      'none' => t('Do not display signup form'),
    ),
    '#default_value' => variable_get('signup_form_location', 'node'),
    '#description' => t('On every signup-enabled node, users with permission to
 sign up can be presented with a form. This setting controls where this form should be displayed: either directly on the node itself, on a separate tab, or not at all.'),
    '#prefix' => '<div class="signup-form-location-radios">',
    '#suffix' => '</div>',
  );

  // The rest of the advanced settings are conditional on if/where the signup
  // form is being displayed.  We use jQuery to hide settings when they're not
  // relevant.
  $signup_path = drupal_get_path('module', 'signup');
  drupal_add_js($signup_path .'/js/admin.settings.js');

  // For each setting that should be hidden, system.css will hide all the
  // settings on page load if JS is enabled.
  $class = 'signup-fieldset_collapsed-setting';
  if (variable_get('signup_form_location', 'node') != 'node') {
    $class .= ' js-hide';
  }
  $form['adv_settings']['signup_fieldset_collapsed'] = array(
    '#title' => t('Default fieldset behavior for per-node signup form'),
    '#type' => 'radios',
    '#options' => array(1 => t('Collapsed'), 0 => t('Expanded')),
    '#default_value' => variable_get('signup_fieldset_collapsed', 1),
    '#description' => t('If the signup form is included at the bottom of each node, the signup form will be encapsulated in a collapsible fieldset. This setting controls if that fieldset is expanded or collapsed by default.'),
    '#prefix' => '<div class="'. $class .'">',
    '#suffix' => '</div>',
  );

  // If views.module is enabled provide the option to display the list
  // of signed-up users in a tab and/or at the bottom of the node.
  $display_options = array();
  $display_options['signup'] = t('Use the built-in listing');
  $views_help_text = '';
  if (module_exists('views')) {
    $display_options['embed-view'] = t('Embed a view');
    $views_help_text = t('If you choose to embed a view, you will be able to select which view you want to use below. The view you select will have a single argument passed in, the node id (nid) of the signup-enabled node being viewed. You can also use views to display this listing on its own tab or in a block if you disable this setting.');
  }
  else {
    $views_help_text = t('If you enable the !views_url, you will be able to embed a view directly onto the page for this listing.', array('!views_url' => l(t('Views module'), 'http://drupal.org/project/views', array('absolute' => TRUE))));
  }
  $display_options['none'] = t('Do not display a listing at all');

  $form['adv_settings']['signup_display_signup_user_list'] = array(
    '#title' => t('How to display the list of signed-up users'),
    '#type' => 'radios',
    '#options' => $display_options,
    '#default_value' => variable_get('signup_display_signup_user_list', 'signup'),
    '#description' => t('Users with the %view_signups permission can be presented with a list of all users who have signed up. This setting controls if and how that list should be generated. !views_help_text', array('%view_signups' => t('view all signups'), '!views_help_text' => $views_help_text)),
    '#prefix' => '<div class="signup-display-signup-user-list-setting">',
    '#suffix' => '</div>',
  );

  if (module_exists('views')) {
    $class = 'signup-user-list-view-settings';
    if (variable_get('signup_form_location', 'node') == 'none' || variable_get('signup_display_signup_user_list', 'signup') != 'embed-view') {
      $class .= ' js-hide';
    }
    $form['adv_settings']['view_settings'] = array(
      '#prefix' => '<div class="'. $class .'">',
      '#suffix' => '</div>',
    );

    $views = views_get_all_views();
    foreach ($views as $view) {
      foreach (array_keys($view->display) as $display_id) {
        if ($display_id != 'default') {
          $key = $view->name .':'. $display_id;
          $view_options[$key] = theme('signup_settings_view_label', $view, $display_id);
        }
      }
    }
    $form['adv_settings']['view_settings']['signup_user_list_view'] = array(
      '#title' => t('View to embed for the signup user list'),
      '#type' => 'select',
      '#options' => $view_options,
      '#default_value' => variable_get('signup_user_list_view', 'signup_user_list:page_1'),
      '#description' => t("If the signup user list is being generated by embedding a view, this selects which view should be used. The view's name, description, and display(s) it defines are listed.  NOTE: if you enable or customize the view being used for this, you should strongly consider disabling the view's menu items to prevent a duplicate tab showing the same information."),
    );
  }

  // Use our own submit handler, so we can do some processing before
  // we hand control to system_settings_form_submit.
  $form = system_settings_form($form);
  unset($form['#submit']);
  $form['#submit'][] = 'signup_settings_form_submit';
  return $form;
}

/**
 * Submits the signup settings form
 *
 * @param $form_id The ID of the form being submitted.
 * @param $form_values The constructed form values array of the submitted form.
 */
function signup_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Save configuration') && db_result(db_query('SELECT COUNT(*) FROM {signup} WHERE nid = 0'))) {
    $values = array(
      $form_state['values']['signup_forwarding_email'],
      $form_state['values']['signup_send_confirmation'],
      $form_state['values']['signup_confirmation_email'],
      $form_state['values']['signup_close_signup_limit'],
    );
    $values[] = isset($form_state['values']['signup_send_reminder']) ? $form_state['values']['signup_send_reminder'] : 0;
    $values[] = isset($form_state['values']['signup_reminder_days_before']) ? $form_state['values']['signup_reminder_days_before'] : 0;
    $values[] = isset($form_state['values']['signup_reminder_email']) ? $form_state['values']['signup_reminder_email'] : '';
    $values[] = 0;  // "nid" of the row in {signup} for the global settings.
    db_query("UPDATE {signup} SET forwarding_email = '%s', send_confirmation = %d, confirmation_email = '%s', close_signup_limit = %d, send_reminder = %d, reminder_days_before = %d, reminder_email = '%s' WHERE nid = %d", $values);
  }
  else {
    module_load_include('install', 'signup', 'signup');
    db_query("DELETE FROM {signup} WHERE nid = 0");
    signup_insert_default_signup_info();
  }

  // Now, remove all the settings we just processed from our copy of
  // $form_state['values'], so system_settings_form_submit() doesn't see them.
  $settings = array(
    'signup_forwarding_email',
    'signup_send_confirmation',
    'signup_confirmation_email',
    'signup_send_reminder',
    'signup_reminder_days_before',
    'signup_reminder_email',
    'signup_close_signup_limit',
  );
  foreach ($settings as $setting) {
    unset($form_state['values'][$setting]);
  }
  // Remove the hidden element from signup_node_settings_form(), too.
  unset($form_state['values']['signup']);

  // Finally, let system_settings_form_submit() do its magic with the
  // rest of the settings.
  system_settings_form_submit($form, $form_state);
}

/**
 * Generates the appropriate selector label for a given view.
 *
 * @param $view
 *   An object containing data about the view to generate a label for.
 *   Contains at least a name (string), description (string), page
 *   (bool), and block (bool) fields.
 *
 * @return
 *   The plain text (no HTML allowed) to include as the label for this view in
 *   the drop-down selector for which view to embed on the site-wide signup
 *   settings page.
 *
 * @see signup_settings_form()
 */
function theme_signup_settings_view_label($view, $display_id) {
  $display_title = check_plain($view->display[$display_id]->display_title);
  $label = $view->name;
  $label .= ' ['. $display_title .']: ';
  $label .= $view->description;
  return $label;
}

