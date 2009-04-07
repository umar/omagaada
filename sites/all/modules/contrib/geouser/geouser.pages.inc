<?php

// $Id: geouser.pages.inc,v 1.3 2009/02/25 16:57:39 zidong Exp $

/**
 * Form the page /geouser to list all the users according to their geographic location.
 *
 * @return string
 */
function geouser(){


  $page = '';
  $countryName = $_GET['Country'];
  $regionName = $_GET['Region'];
  $cityName = $_GET['City'];
  $sortBy = $_GET['Sort'];
  $limit = variable_get('geouser_list_limit',20);


  //Set up the breadcrumb

  $locations = array('Country' => $countryName, 'Region' => $regionName, 'City' => $cityName);

  $breadcrumb = array(l('home',$base_url), l('geouser','geouser'));
  $querys = array();
  if($sortBy == 'Name'){
    $querys['Sort'] = 'Name';
  }
  foreach($locations as $key => $value){
    if(!empty($value)){
      $querys[$key] = $value;


      $breadcrumb[] = l($value, 'geouser', array('query' => $querys));
    }

    else{
      break;
    }
  }

  drupal_set_breadcrumb($breadcrumb);

  $geousers = GeoUser::getGeoUsers($countryName, $regionName, $cityName, $sortBy, $limit);

  // Form the links : 'sort by access', 'sort by name'
  $sortLinks = array();

  if($sortBy == 'Name'){
    unset($querys['Sort']);
   $sortLinks[] = l('Sort by access', 'geouser', array('query' => $querys));
   $sortLinks[] = 'Sort by name';
  }else{
   $sortLinks[] = 'Sort by access';
    $querys['Sort'] = 'Name';
   $sortLinks[] = l('Sort by name', 'geouser', array('query' => $querys));
  }

  $page .= theme('item_list', $sortLinks, NULL, 'ul', array('class' => 'pager sort_link'));
  $page .= theme('user_list', $geousers);
  $page .= theme('pager', NULL, $limit, 0);

  return $page;
}



