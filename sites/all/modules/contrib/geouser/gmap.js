function createMarker(point,html) {
        var marker = new GMarker(point);
        GEvent.addListener(marker, "click", function() {
          marker.openInfoWindowHtml(html);
        });
        return marker;
      }

$(document).ready(function() {

      if (GBrowserIsCompatible()) {
        var map = new GMap2(document.getElementById("gmap"));
        map.addControl(new GLargeMapControl());
        map.addControl(new GMapTypeControl());
        map.setCenter(new GLatLng($("input#latitude").val(), $("input#longitude").val()), parseInt($("input#zoom").val()));
      }

var point = new GLatLng($("input#latitude").val(), $("input#longitude").val());
var marker = createMarker(point,'<div style="width:240px">User: '+ $("input#username").val() +'' +
		                        '<br />Latitude: '+$("input#latitude").val()
		                        +'<br />Longitude: '+$("input#longitude").val()+'</div>');
map.addOverlay(marker);  

});  


