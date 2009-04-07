// $Id: README.txt,v 1.1 2009/02/25 16:58:41 zidong Exp $
************************************************
INTRODUCTION
************************************************
GeoUser is a module written for Drupal CMS(http://drupal.org).
GeoUser retrieves geographic information from GeoLiteCity, an IP database from (http://maxmind.com)
GeoUser includes API files of Net_GeoIP (http://pear.php.net/Net_GeoIP).
GeoUser generates geographic maps through Yahoo Maps API(http://maps.yahoo.com).
GeoUser generates geographic maps through Google Maps API(http://maps.google.com).
GeoUser can ONLY allow anonymous users from admin-defined geographic locations to register as a member. 
GeoUser can only work well with PHP 5.1 and above, and we recommend you update your PHP version to the latest.


***********************************************
INSTALLATION
***********************************************

Download GeoUser:
from http://drupal.org/project/geouser
or check out from Drupal CVS reposity.

Download GeoLiteCity:
from http://www.maxmind.com/app/geolitecity
unzip the package you've just downloaded

Recommended file structure:
         
         /IPDatabase/GeoLiteCity.dat
         /Drupal/
         
**********************************************
SETTINGS
********************************************** 
Go To: admin/settins/geouser      

**********************************************
USER PERMISSION
**********************************************  

Go to: admin/user/permissions