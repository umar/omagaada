function nodemap_yahoo_render(map, fLat, fLong, sType, iZoom, sTitle, sAddress) {
	// Add map type control
	map.addTypeControl();
	// Add map zoom (long) control
	map.addZoomLong();
	// Set map type to either of: YAHOO_MAP_SAT, YAHOO_MAP_HYB, YAHOO_MAP_REG
	switch (sType) {
		case "map":
		map.setMapType(YAHOO_MAP_REG);
		break;
		case "sat":
		map.setMapType(YAHOO_MAP_SAT);
		break;
		case "hyb":
		map.setMapType(YAHOO_MAP_HYB);
		break;
		default:
		map.setMapType(YAHOO_MAP_REG);
		break;
	}
	//
	map.disableKeyControls();
	map.disablePanOnDoubleClick();
	// marker:
	var oMapGeoPoint = new YGeoPoint(fLat, fLong);
	// use simple marker if node title & address are empty:
	if (sTitle.length==0 && sAddress.length==0) {
		map.addMarker(oMapGeoPoint);
	}
	else {
		// initialize YMarker:
		var oYahooMarker = new YMarker(oMapGeoPoint);
		var sMarkerMarkup = '';
		// add node title:
		if (sTitle.length>0) {
			sMarkerMarkup += '<span>'+sTitle+'</span>';
		}
		// add address:
		if (sAddress.length>0) {
			if (sTitle.length>0) {
				sMarkerMarkup += '<br/>'+'<span>'+sAddress+'</span>';
			}
			else {
				sMarkerMarkup += '<span>'+sAddress+'</span>';
			}
		}
		// auto open markup:
		oYahooMarker.openSmartWindow(sMarkerMarkup);
		// put the marker on the map:
		map.addOverlay(oYahooMarker);
	}
	// Specifying the Map starting location and zoom level
	map.drawZoomAndCenter(oMapGeoPoint, iZoom);
}