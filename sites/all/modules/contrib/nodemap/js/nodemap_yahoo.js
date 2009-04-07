// yahoo start function:
var yahooMap;
function startYahooMap() {
	// start an yahoo map object:
	yahooMap = new YMap(document.getElementById('nodemap_map_container'));
	//document.getElementById('nodemap_map_container').style.cursor = 'default';
	// Add map type control
	yahooMap.addTypeControl();
	// Add map zoom (long) control
	yahooMap.addZoomLong();
	// Set map type to either of: YAHOO_MAP_SAT, YAHOO_MAP_HYB, YAHOO_MAP_REG
	switch (Drupal.settings.nodemap.def_edit_map_type) {
		case "map":
		yahooMap.setMapType(YAHOO_MAP_REG);
		break;
		case "sat":
		yahooMap.setMapType(YAHOO_MAP_SAT);
		break;
		case "hyb":
		yahooMap.setMapType(YAHOO_MAP_HYB);
		break;
		default:
		yahooMap.setMapType(YAHOO_MAP_REG);
		break;
	}
	//
	yahooMap.disableKeyControls();
	yahooMap.disablePanOnDoubleClick();
	// Specifying the Map starting location and zoom level
	if (document.getElementById('edit-nodemap-latitude-field').value.length != 0 &&
		document.getElementById('edit-nodemap-longitude-field').value.length != 0
	) {
		nodemap_manual_marker(0);
	}
	else {
		yahooMap.drawZoomAndCenter(Drupal.settings.nodemap.def_edit_map_location, Drupal.settings.nodemap.def_edit_map_zoom);
	}

	// get geocodes:
	var processYahooMap = function (_e, _c) {
		// fill geo fields:
		nodemap_fill_geo_fields(_c.Lat, _c.Lon);
		// clear current marker first:
		var yahooMapMarkers = yahooMap.getMarkerIDs();
		for (var iMarker=0; iMarker<yahooMapMarkers.length; iMarker++) {
			yahooMap.removeMarker(yahooMapMarkers[iMarker]);
		}
		// mark location the map:
		var nodeGeoLocation = new YGeoPoint(_c.Lat, _c.Lon);
		yahooMap.addMarker(nodeGeoLocation);
	}
	// capture mouse double-click:
	YEvent.Capture(yahooMap, EventsList.MouseDoubleClick, processYahooMap);
}
// add to page onload:
if (Drupal.jsEnabled) {
	$(document).ready(function () {
		startYahooMap();
	});
}