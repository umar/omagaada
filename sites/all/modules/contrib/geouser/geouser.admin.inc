<?php
//$Id: geouser.admin.inc,v 1.9 2009/02/25 16:55:52 zidong Exp $

/**
 * Form the admin panel
 *
 * @return string
 */

function geouser_admin_settings(){

  $form = array();

  /**
   * Select type of a geographic map
   */
  $form['map_type'] = array(
          '#type'  => 'radios',
          '#title' => t("Type of map"),
          '#description' 	=> t("Select type of a geographic map to be displayed."),
          '#options' => array(t('Google Map'), t('Yahoo Map')),
          '#default_value' => variable_get('map_type', 0), 
          '#collapsible' => TRUE,
		  '#collapsed' 	=> true,
  );



  /**
   * Yahoo Map settings
   */
  $form['ymap'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("Yahoo map settings"),
		'#description' 	=> t("Present geographic location visually by Yahoo Map"),
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> true,
  );
  $form['ymap']['geouser_ymap_appid'] = array(
  '#type' => 'textfield', 
  '#title' => t('Yahoo Application ID'), 
  '#default_value' => variable_get('geouser_ymap_appid','YahooDemo'), 
  '#description' => t('You can apply for your yahoo app id for free on http://developer.yahoo.com'),
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );


  $form['ymap']['geouser_ymap_width'] = array(
  '#type' => 'textfield', 
  '#title' => t('Width of Map'), 
  '#default_value' => variable_get('geouser_ymap_width',550), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );

  $form['ymap']['geouser_ymap_height'] = array(
  '#type' => 'textfield', 
  '#title' => t('Height of Map'), 
  '#default_value' => variable_get('geouser_ymap_height',350), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );


  /**
   * Google Map settings
   */
  $form['gmap'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("Google map settings"),
		'#description' 	=> t("Present geographic location visually by Google Map"),
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> true,
  );
  $form['gmap']['geouser_gmap_appid'] = array(
  '#type' => 'textfield', 
  '#title' => t('Google Application ID'), 
  '#default_value' => variable_get('geouser_gmap_appid','Replace your own app id here'), 
  '#description' => t('You can apply for your google app id for free on http://code.google.com/apis/maps/'),
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );


  $form['gmap']['geouser_gmap_width'] = array(
  '#type' => 'textfield', 
  '#title' => t('Width of Map'), 
  '#default_value' => variable_get('geouser_gmap_width',550), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );

  $form['gmap']['geouser_gmap_height'] = array(
  '#type' => 'textfield', 
  '#title' => t('Height of Map'), 
  '#default_value' => variable_get('geouser_gmap_height',350), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );

  $form['gmap']['geouser_gmap_zoom'] = array(
  '#type' => 'textfield', 
  '#title' => t('Zoom of Map'), 
  '#default_value' => variable_get('geouser_gmap_zoom',6), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );


  /**
   * IP Database settings
   */
  $form['database'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("IP database settings"),
		'#description' 	=> t("GeoUser uses GeoLiteCity, a IP database, download it from http://www.maxmind.com/app/geolitecity"),
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> false,
  );

  $form['database']['geouser_db_path'] = array(
  '#type' => 'textfield', 
  '#title' => t('Path of database'), 
  '#default_value' => variable_get('geouser_db_path','../IPDatabase/GeoLiteCity.dat'), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );

  /**
   *Permission of user as her geographic location.
   */
  require_once('GeoIP.php');
  sort(Net_GeoIP::$COUNTRY_NAMES);
  Net_GeoIP::$COUNTRY_NAMES[0] = 'All';
  $form['permission'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("Permission settings"),
		'#description' 	=> t("Permission of user as her geographic location"),
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> true,
  );
  $form['permission']['register'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("User Register"),

        '#collapsible'	=> TRUE,
		'#collapsed' 	=> false,
  );

  $geouser_allow_countries = variable_get('geouser_allow_countries', array());
  $geouser_allow_countries = array_keys($geouser_allow_countries);
  $form['permission']['register']['geouser_allow_countries'] = array(
		'#type'		=> 'select',
        '#description' => 'For multi-selection, press CTRL and click',
        '#multiple' => true,
        '#size' => 10,
		'#title'	=> t("Countries allowed"),
		'#options'    => Net_GeoIP::$COUNTRY_NAMES,
        '#default_value' => $geouser_allow_countries,
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> false,
  );
  
  $form['list'] = array(
		'#type'		=> 'fieldset',
		'#title'	=> t("GeoUser list settings"),
		'#description' 	=> t("List users on the page as her geographic location"),
        '#collapsible'	=> TRUE,
		'#collapsed' 	=> true,
  );
  $form['list']['geouser_list_limit'] = array(
  '#type' => 'textfield', 
  '#title' => t('Limit of users on one page'), 
  '#default_value' => variable_get('geouser_list_limit',20), 
  '#size' => 50, 
  '#maxlength' => 128, 
  '#required' => TRUE,
  );


  return system_settings_form($form);
}
