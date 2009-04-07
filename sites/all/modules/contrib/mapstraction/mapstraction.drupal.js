// $Id: mapstraction.drupal.js,v 1.1.2.5 2009/02/23 23:08:51 diggersf Exp $
$(document).ready(function() {
  Drupal.mapstraction = [];
  $(Drupal.settings.mapstraction).each(function(index) {
    var id = 'mapstraction-' + this.viewName + '-' + this.currentDisplay;
    Drupal.mapstraction[id] = new Mapstraction(id, this.apiName);
    
    // Set start point
    var startPoint = new LatLonPoint(Number(this.initialPoint.latitude), Number(this.initialPoint.longitude));
    Drupal.mapstraction[id].setCenterAndZoom(startPoint, Number(this.initialPoint.zoom));
    
    // Set up controls
    Drupal.mapstraction[id].addControls(this.controls);
    
    // Set up markers & info bubbles
    $(this.markers).each(function(index) {
      marker = new Marker(new LatLonPoint(Number(this.lat), Number(this.lon)));
      marker.setInfoBubble(this.title);
      Drupal.mapstraction[id].addMarker(marker);
    });
  });
});
