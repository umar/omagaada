/* $Id: node_form.js,v 1.5 2008/11/06 10:20:48 dww Exp $ */

/**
 * On a node form, if the "Allow signups" radios are set to 1
 * ('Enabled'), then show the other signup-specific settings,
 * otherwise, hide them.
 */
Drupal.behaviors.signupShowNodeSettings = function () {
  $('div.signup-allow-radios input[@type=radio]').click(function () {
    $('div.signup-node-settings')[['hide', 'show', 'hide'][this.value]]();
  });
};
