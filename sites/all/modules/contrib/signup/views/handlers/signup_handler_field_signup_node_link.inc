<?php
// $Id: signup_handler_field_signup_node_link.inc,v 1.1 2008/12/20 08:43:28 dww Exp $

/**
 * Field handler to generate links to the various signup subtabs on a node.
 */
class signup_handler_field_signup_node_link extends views_handler_field_node_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'node', 'field' => 'uid');
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['tab'] = array(
      '#type' => 'select',
      '#title' => t('Signup tab to link to'),
      '#options' => array(
        'signup' => t('Signup current user form'),
        'list' => t('Signup user list'),
        'admin' => t('Administer signups'),
        'add' => t('Form to sign up other users'),
        'broadcast' => t('Signup broadcast form'),
      ),
      '#default_value' => $this->options['tab'],
    );
    $form['destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set view as return destination'),
      '#description' => t('After submitting a form, do you want to be redirected back to this view, or stay at the other page?'),
      '#default_value' => $this->options['destination'],
    );
  }

  function render($values) {
    $tab = $this->options['tab'];
    $node = new StdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};

    // Ensure the current user can access this link for the given node.
    if (!$this->check_access($tab, $node)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : $this->get_default_link_text($tab);
    $url = "node/$node->nid/signups/$tab";
    $link_options = array();
    if ($tab != 'list' && !empty($this->options['destination'])) {
      $link_options['query'] = drupal_get_destination();
    }
    return l($text, $url, $link_options);
  }

  function get_default_link_text($tab) {
    switch ($tab) {
      case 'signup':
        return t('signup form');

      case 'list':
        return t('signup list');

      case 'admin':
        return t('administer signups');

      case 'add':
        return t('signup other users');

      case 'broadcast':
        return t('signup broadcast');

    }
  }

  function check_access($tab, $node) {
    global $user;
    switch ($tab) {
      case 'signup':
        return variable_get('signup_form_location', 'node') == 'tab' && user_access('sign up for content');

      case 'list':
        return variable_get('signup_display_signup_user_list', 'signup') != 'none' && user_access('view all signups');

      case 'admin':
      case 'add':
        $admin_all = user_access('administer all signups');
        $admin_own = user_access('administer signups for own content') && ($node->uid == $user->uid);
        return ($admin_all || $admin_own);

      case 'broadcast':
        $email_all = user_access('email all signed up users');
        $email_own = user_access('email users signed up for own content') && $user->uid == $node->uid;
        return $email_all || $email_own;

    }
  }

}

