/**
* Fill fields, on double click event on the map:
*/
function nodemap_fill_geo_fields(latValue, lonValue) {
    var fLat = parseFloat(latValue).toFixed(6);
    var fLon = parseFloat(lonValue).toFixed(6);
    document.getElementById('edit-nodemap-latitude-field').value = fLat;
    document.getElementById('edit-nodemap-longitude-field').value = fLon;
}
/**
* Read the geocode fields, and manually put the marker on the map:
*/
function nodemap_manual_marker(verbose) {
    var serviceType = "yahoo";
    var latValue = document.getElementById('edit-nodemap-latitude-field').value;
    var lonValue = document.getElementById('edit-nodemap-longitude-field').value;
    if (latValue.length != 0 && lonValue.length != 0) {
        // parse to float & round decimals:
        var fLat = parseFloat(latValue).toFixed(6);
        var fLon = parseFloat(lonValue).toFixed(6);
        if (!isNaN(fLat) && !isNaN(fLon)) {
            // put the values back:
            document.getElementById('edit-nodemap-latitude-field').value = fLat;
            document.getElementById('edit-nodemap-longitude-field').value = fLon;
            switch (serviceType) {
                case "yahoo":
                // clear current marker first:
                var yahooMapMarkers = yahooMap.getMarkerIDs();
                for (var iMarker=0; iMarker<yahooMapMarkers.length; iMarker++) {
                    yahooMap.removeMarker(yahooMapMarkers[iMarker]);
                }
                // mark location the map:
                var nodeGeoLocation = new YGeoPoint(fLat, fLon);
                //var nodeMarker = new YMarker(nodeGeoLocation);
                //nodeMarker.addLabel("<div>test label</div>");
                //nodeMarker.openAutoExpand();
                //nodeMarker.addAutoExpand("<p>test label</p>");
                yahooMap.addMarker(nodeGeoLocation);
                //yahooMap.addOverlay(nodeMarker);
                // center the map on the new marker:
                //var aYahooMapGeoPoints = new Array();
                //aYahooMapGeoPoints[0] = nodeGeoLocation;
                //yahooMap.drawZoomAndCenter(nodeGeoLocation, yahooMap.getZoomLevel(aYahooMapGeoPoints));
                //yahooMap.drawZoomAndCenter(nodeGeoLocation, nmDefaultMapZoomLevel);
                yahooMap.drawZoomAndCenter(nodeGeoLocation, yahooMap.getZoomLevel());
                //alert(yahooMap.getZoomLevel(aYahooMapGeoPoints));
                break;
            }
        }
        else {
            alert('Invalid entries. Please use numeric values. (int/float).');
        }
    }
    else {
        if (verbose==1) {
            alert("One or both geofields are empty!");
        }
    }
}
/**
* Get geocodes from yahoo:
*/
var aNodeMapSearchLocationsList = new Array();
function nodemap_set_map_location(iPos) {
    nodemap_fill_geo_fields(aNodeMapSearchLocationsList[iPos]['latitude'], aNodeMapSearchLocationsList[iPos]['longitude']);
    document.getElementById('edit-nodemap-serialized-field').value = aNodeMapSearchLocationsList[iPos]['serialized'];
    nodemap_manual_marker(0);
    //fill address field, if none retuned from geo search:
    document.getElementById('edit-nodemap-address-field').value = aNodeMapSearchLocationsList[iPos]['address'];
    if (aNodeMapSearchLocationsList[iPos]['address'].length == 0) {
        document.getElementById('edit-nodemap-address-field').value = aNodeMapSearchLocationsList[iPos]['city']+', '+aNodeMapSearchLocationsList[iPos]['state']+', '+aNodeMapSearchLocationsList[iPos]['country'];
    }
}
/**
* Get geocodes from yahoo:
*/
function nodemap_get_address_geocodes() {
    // hide the container:
    var oSearchLocationsListContainer = document.getElementById('nodemap_search_locations_list_container');
    // take address:
    var addressValue = document.getElementById('edit-nodemap-address-field').value;
    if (addressValue.length != 0) {
        // display progress animation:
        oSearchLocationsListContainer.innerHTML = '<img src="'+Drupal.settings.nodemap.base_path+'misc/progress.gif" style="width:100%; height:15px; margin-top:15px;"/>';
        oSearchLocationsListContainer.style.display = 'block';
        //anti js caching:
        var milisecTime = new Date().getTime();
        var sRequestURL = Drupal.settings.nodemap.base_path+"nodemap/getgeocodes/"+milisecTime+"/"+addressValue;
        // make a json request through jQuery:
        $.getJSON(sRequestURL, function(data) {
            // fill fields:
            if (data['status'] == 1) {
                // define the locations list array:
                var aLocations = new Array();
                // fill the array:
                // from yahoo:
                if (data['geocode_data']['yahoo'].length > 0) {
                    for (var i=0; i<data['geocode_data']['yahoo'].length; i++) {
                        var aTemp = data['geocode_data']['yahoo'][i];
                        aTemp['provider'] = "!Yahoo";
                        // add to locations array:
                        aLocations.push(aTemp);
                        aTemp = new Array();
                        delete aTemp;
                    }
                }
                //from google:
                if (data['geocode_data']['google'].length > 0) {
                    for (var i=0; i<data['geocode_data']['google'].length; i++) {
                        var aTemp = data['geocode_data']['google'][i];
                        aTemp['provider'] = "Google";
                        // add to locations array:
                        aLocations.push(aTemp);
                        aTemp = new Array();
                        delete aTemp;
                    }
                }
                aNodeMapSearchLocationsList = aLocations;
                // generate html table:
                var sLocationsListTableStart = '<table>';
                var sLocationsListTableContens = '';
                sLocationsListTableContens += '<tr>';
                sLocationsListTableContens += '<td colspan="2" align="left" valign="middle">';
                sLocationsListTableContens += data['text'];
                sLocationsListTableContens += '</td>';
                sLocationsListTableContens += '</tr>';
                sLocationsListTableContens += '<tr>';
                sLocationsListTableContens += '<td align="left" valign="middle">';
                sLocationsListTableContens += '<i>Location</i>';
                sLocationsListTableContens += '</td>';
                sLocationsListTableContens += '<td align="left" valign="middle">';
                sLocationsListTableContens += '<i>Provider</i>';
                sLocationsListTableContens += '</td>';
                sLocationsListTableContens += '</tr>';
                var sLocationsListTableEnd = '</table>';
                for (var i=0; i<aLocations.length; i++) {
                    var sLocationName = aLocations[i]['address'];
                    if (aLocations[i]['address'].length==0) {
                        //sLocationName = 'Lat: '+aLocations[i]['latitude']+', Lon: '+aLocations[i]['longitude'];
                        sLocationName = aLocations[i]['city']+', '+aLocations[i]['state']+', '+aLocations[i]['country'];
                    }
                    sLocationsListTableContens += '<tr>';
                    sLocationsListTableContens += '<td align="left" valign="middle">';
                    sLocationsListTableContens += '<a href="javascript:nodemap_set_map_location('+i+');">';
                    sLocationsListTableContens += sLocationName;
                    sLocationsListTableContens += '</a>';
                    sLocationsListTableContens += '</td>';
                    sLocationsListTableContens += '<td align="left" valign="middle">';
                    sLocationsListTableContens += aLocations[i]['provider'];
                    sLocationsListTableContens += '</td>';
                    sLocationsListTableContens += '</tr>';
                    sLocationsListTableContens += '';
                    sLocationsListTableContens += '';
                    sLocationsListTableContens += '';
                    sLocationsListTableContens += '';
                    sLocationsListTableContens += '';
                }
                // fill the container div & set its display to block:
                oSearchLocationsListContainer.innerHTML = sLocationsListTableStart+sLocationsListTableContens+sLocationsListTableEnd;
                oSearchLocationsListContainer.style.display = 'block';
            }
            else {
                oSearchLocationsListContainer.innerHTML = '<table><tr><td>'+data['text']+'</td></tr></table>';
                //oSearchLocationsListContainer.style.display = 'none';
                //alert(data['text']);
            }
        });
    }
    else {
        alert("Empty address field!");
    }
}