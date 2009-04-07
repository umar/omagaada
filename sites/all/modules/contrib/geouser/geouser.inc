<?php

// $Id: geouser.inc,v 1.23 2009/02/25 16:56:49 zidong Exp $

/**
 * The main class for GeoUser module
 *
 */
class GeoUser{

   
  /**
   * Get a Yahoo Map as PNG image
   *
   * @param string $appid
   * @param float $latitude
   * @param float $longitude
   * @param int $imageHeight
   * @param int $imageWidth
   * @return string map of html format
   * @throws Exception cannot get content from yahoo map service
   */
  public static function getPngMap($appid, $latitude, $longitude, $imageHeight, $imageWidth){

    $request  = 'http://local.yahooapis.com/MapsService/V1/mapImage';
    $request .= '?appid='.$appid;
    $request .= '&latitude='.$latitude;
    $request .= '&longitude='.$longitude;
    $request .= '&image_height='.$imageHeight;
    $request .= '&image_width='.$imageWidth;
    $request .= '&output=php';

    $resource = curl_init($request);
    curl_setopt($resource,CURLOPT_RETURNTRANSFER, TRUE);

    //var_dump($request);

    if($response = curl_exec($resource)){

      curl_close($resource);
      $phpobj = unserialize($response);
      $url = $phpobj['Result'];
      //var_dump($phpobj);
      $map = '<img src="' .$url .'" />';
      return $map;
    }else
    throw new Exception('Cannot get content from '.$request);
     
  }

  /**
   * Display a Google Map of Ajax form
   *
   * @param string $appid
   * @param float $latitude
   * @param float $longitude
   * @param int $zoom
   * @param int $imageHeight
   * @param int $imageWidth
   * @return string
   */
  public static function getAjaxMap($appid, $latitude, $longitude, $zoom, $imageHeight, $imageWidth){

    $map = '<input type="hidden" id="latitude" value="' .$latitude. '" />'."\n";
    $map .= '<input type="hidden" id="longitude" value="' .$longitude. '" />'."\n";
    $map .= '<input type="hidden" id="zoom" value="' .$zoom. '" />'."\n";
    $user = user_load(array('uid' => arg(1)));
    $map .= '<input type="hidden" id="username" value="' .$user->name. '" />'."\n";


    drupal_set_html_head('<script src="http://maps.google.com/maps?file=api&amp;hl=en&amp;v=2&amp;key=' . $appid . '" type="text/javascript"></script>');


    drupal_add_js(drupal_get_path('module', 'geouser').'/gmap.js');
    $map .= '<div id="gmap" style="width: '.$imageWidth.'px; height: '.$imageHeight.'px"></div>'."\n";
     
    return $map;
  }

  /**
   * Retrieve user's IP address from geouser table
   *
   * @param int $uid
   * @return string IP address
   * @throws Exception IP address of the user cannot be found
   */
  public static function getUserIP($uid){

    $addr = null;
    if($uid <= 0){

      $addr = ip_address();
      return $addr;

    }else{
      if($result = db_query("SELECT addr FROM {geouser} WHERE uid = %d", $uid) ){
        $geouser = db_fetch_object($result);
        if($geouser){

          $addr = $geouser->addr;
        }
      }
      if($addr == null)
      throw new Exception('IP address of the user cannot be found.');
      else
      return $addr;
    }
  }

  /**
   * Retrieve geographic information from IP database
   *
   *
   * @param string $addr || IP address
   * @return Net_GeoIP_Location || The full location record
   */
  public static function getGeoCity($addr){

    $location = null;
    $path = variable_get('geouser_db_path','../IPDatabase/GeoLiteCity.dat');

    require_once('GeoIP.php');
    try{
      $geo = new Net_GeoIP($path);
      $location = $geo->lookupLocation($addr);

    }catch(Exception $e){

      drupal_set_message($e->getMessage(),'error');
    }

    if(!$location instanceof Net_GeoIP_Location){

      throw new Exception("Geographic location of 'IP Address : $addr' cannot be found.");
    }

    return $location;
  }

  /**
   * Retrieve name of region
   *
   * @param Net_GeoIP_Location $location
   * @return string || name of region
   */
  public static function getRegionName(Net_GeoIP_Location $location){

    $countryCode = $location->countryCode;
    $regionCode = $location->region;

    require_once('Net/GeoIP/geoipregionvars.php');
    $regionName = $GEOIP_REGION_NAME[$countryCode][$regionCode];

    return $regionName;
  }

  /**
   * Retrieve user's related information via her geographic information
   *
   * @param string $countryName
   * @param string $regionName
   * @param string $cityName
   * @param string $sortBy
   * @param int $limit
   * @return array || objects including (uid, name, etc)
   */
  public static function getGeoUsers($countryName, $regionName, $cityName, $sortBy = 'Access', $limit = 20){

    $geousers = array();

    $query = 'SELECT * FROM {geouser} g LEFT JOIN {users} u ON u.uid = g.uid';

    if(!empty($countryName) && !empty($regionName) && !empty($cityName)){

      $query .= ' WHERE g.country = "%s" && g.region = "%s" && g.city = "%s"';

    }
    else if(!empty($countryName) && !empty($regionName) && empty($cityName)){

      $query .= ' WHERE g.country = "%s" && g.region = "%s"';

    }
    else if(!empty($countryName) && empty($regionName) && empty($cityName)){

      $query .= ' WHERE g.country = "%s"';
    }

    if($sortBy == 'Name'){

      $query .= ' ORDER BY u.name ASC';

    }else{
       
      $query .= ' ORDER BY u.access DESC';

    }

    $result = pager_query($query, $limit, 0, NULL, $countryName, $regionName, $cityName);


    $rows = db_affected_rows($result);
    if($rows <= 0){
      drupal_set_message('No user is found.', 'warning');
    }
    if($rows >= 1){
       
      while($row = db_fetch_object($result)){

        $geousers[$row->uid] = $row;
      }
    }

    return $geousers;
  }

  /**
   * Check if user has permission to register as her geographic location
   *
   * @param int $uid
   * @return boolean
   */
  public static function checkGeoUserRegisterPermission(){

    global $user;
    if($user->uid == 1){

      return true;
    }
    require_once('GeoIP.php');
    $addr = ip_address();
     
    //$addr = '140.174.9.9';
    try{
      $location = self::getGeoCity($addr);

      $countryName = $location->countryName;
      $regionName = self::getRegionName($location);
      $cityName = $location->city;
    }catch(Exception $e){

      drupal_set_message($e->getMessage(), 'error');
      return false;
    }

    $countriesAllowed = variable_get('geouser_allow_countries', array());
    $countriesAllowed = array_keys($countriesAllowed);
    //$countriesAllowed = array(231,230);
    sort(Net_GeoIP::$COUNTRY_NAMES);
    foreach($countriesAllowed as $i => $countryAllowed){

      if($countryAllowed == 0){

        return true;
      }
      $countryAllowed = Net_GeoIP::$COUNTRY_NAMES[$countryAllowed];
      $countriesAllowed[$i] = $countryAllowed;
    }

    /**
     $regions_allow = variable_get('geouser_regions_allow', array());
     $cities_allow = variable_get('geouser_cities_allow', array());
     $perm_range = variable_get('geouser_perm_range', 'country');

     */
    $perm_range = 'country';
    switch ($perm_range){
      case 'country':
        if(in_array($countryName, $countriesAllowed)){

          return true;
        }
        else{

          return false;
        }
        break;


        //TODO: Check GeoUserRegisterPermission of Region, City
        /**
         case 'region':
         if(in_array($regionName, $regions_allow))
         return true;
         else
         return false;
         break;

         case 'city':
         if(in_array($cityName, $cities_allow))
         return true;
         else
         return false;
         break;
         */
    }

  }

}





