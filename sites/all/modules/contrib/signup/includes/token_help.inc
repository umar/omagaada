<?php
// $Id: token_help.inc,v 1.1 2008/11/15 02:03:50 dww Exp $


/**
 * @file
 * Code for building help text for token.module support on various forms.
 */

/**
 * Private function to generate HTML for showing the available tokens.
 *
 * @param $form
 *   Reference to the form array to include the help fieldset in.
 * @param $element_name
 *   Name of the form element to use for the help fieldset.
 */
function _signup_token_help(&$form, $element_name) {
  $form[$element_name] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$element_name]['help_text'] = array(
    '#value' => _signup_build_token_help(),
  );
}

/**
 * Private function to generate HTML for showing the available tokens
 *
 * @return The themed representation of the available tokens.
 */
function _signup_build_token_help() {
  static $help_html = '';
  if (empty($help_html)) {
    $patterns = token_get_list('node');
    foreach ($patterns as $type => $pattern_set) {
      foreach ($pattern_set as $pattern => $description) {
        $tokens[$pattern] = $description;
      }
    }
    $help_html = theme('signup_token_help', $tokens);
  }
  return $help_html;
}

/**
 * theme_signup_token_help()
 *
 * @param $tokens
 *   An array of token patterns mapping to their description
 *
 * @return The themed representation of the tokens.
 */
function theme_signup_token_help($tokens) {
  $tokens_html = "<dl>\n";
  foreach ($tokens as $name => $description) {
    $tokens_html .= '<dt>['. $name .']</dt>';
    $tokens_html .= '<dd>'. $description ."</dd>\n";
  }
  $tokens_html .= "</dl>\n";

  return $tokens_html;
}

