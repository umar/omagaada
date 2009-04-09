<?php
// $Id: signup_form.inc,v 1.3 2008/11/19 18:35:11 dww Exp $


/**
 * @file
 * Site-specific code related to the form when users signup for a node.
 */

/**
 * Return the site-specific custom fields for the signup user form.
 *
 * To customize this for your site, copy this entire function into
 * your theme's template.php file, rename the function to
 * phptemplate_signup_user_form(), and modify to taste.  Feel free to
 * alter any elements in this section, remove them, or add any others.
 *
 * WARNING: If your site allows anonymous signups and you alter the
 * 'Name' field in this function, you will probably have to implement a
 * version of theme_signup_email_token_anonymous_username() for your site.
 *
 * In order for the form to be rendered properly, the name of the form
 * element must be $form['signup_form_data']['NameOfDataField'], where
 * NameOfDataField is replaced with the actual name of the data field.
 * We suggest that the displayed name of the field (the '#title'
 * property) be the same as the name of the data field, but it's not
 * required.  See below for examples.
 *
 * Fieldsets are not currently supported in this form.  Any
 * '#default_value' will be filled in by default when the form is
 * presented to the user.  Any field marked '#required' must be filled
 * in before the user can sign up.
 *
 * If you do not want any additional fields, the function can simply
 * return an empty array: "return array();"
 *
 * @param $node
 *   The fully loaded node object where this signup form is appearing.
 *
 * @return
 *   Array defining the form to present to the user to signup for a node.
 *
 * @see theme_signup_email_token_anonymous_username()
 */
function theme_signup_user_form($node) {
  global $user;
  $form = array();

  // If this function is providing any extra fields at all, the following
  // line is required for form form to work -- DO NOT EDIT OR REMOVE.
  $form['signup_form_data']['#tree'] = TRUE;

  $form['signup_form_data']['Name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 40, '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['signup_form_data']['Phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 40, '#maxlength' => 64,
  );

  // If the user is logged in, fill in their name by default.
  if ($user->uid) {
    $form['signup_form_data']['Name']['#default_value'] = $user->name;
  }

  return $form;
}

/**
 * Returns the value to use for the %user_name email token for anonymous users.
 *
 * WARNING: If you implemented your own version of theme_signup_form_data()
 * that changed or removed the custom 'Name' field and your site
 * allows anonymous signups, you will need to modify this, too.
 *
 * @param $form_data
 *   Array of custom signup form values for the current signup.
 * @param $email
 *   E-mail address of the anonymous user who signed up.
 * @return
 *   A string with the proper value for the %user_name email token.
 *
 * @see theme_signup_user_form()
 */
function theme_signup_email_token_anonymous_username($form_data, $email) {
  // In some cases, the best you can do is to use the anonymous user's
  // supplied email address, in which case, you should uncomment this:
  //return $email;

  // WARNING: This line is only valid if you left the 'Name' field in
  // your site's version of theme_signup_user_form().
  return $form_data['Name'];
}
