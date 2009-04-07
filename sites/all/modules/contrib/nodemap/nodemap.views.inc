<?php
/*
*  This file is used to tell the views module about the nodemap views
*/
/**
 * ...hook _views_data()
 */
function nodemap_views_data() {
    $data = array();
    // define nodemap table:
    $data['nodemap']['table']['group'] = t('NodeMap');
    $data['nodemap']['table']['join'] = array (
    'node' => array (
    'left_field'=>'nid',
    'field'=>'nodemap_nid'
    )
    );
    // latitude field:
    $data['nodemap']['nodemap_latitude'] = array (
    'title' => t('Latitude'),
    'help' => t('NodeMap\'s Latitude'),
    // Information for displaying:
    'field' => array (
    'handler' => 'views_handler_field_numeric',
    'click sortable' => TRUE,
    ),
    // Information for accepting as an argument:
    /*'argument' => array (
    'handler' => 'views_handler_argument_numeric'
    ),*/
    // Information for accepting as a filter:
    'filter' => array (
    'title' => t('Latitude'),
    'help' => t('NodeMap\'s Latitude, float number.'),
    'handler' => 'nodemap_handler_filter_float',
    ),
    // Information for sorting:
    /*
    'sort' => array (
    'handler' => 'views_handler_sort',
    )
    */
    );
    // longitude field:
    $data['nodemap']['nodemap_longitude'] = array (
    'title' => t('Longitude'),
    'help' => t('NodeMap\'s Longitude'),
    // Information for displaying:
    'field' => array (
    'handler' => 'views_handler_field_numeric',
    'click sortable' => TRUE,
    ),
    // Information for accepting as an argument:
    /*'argument' => array (
    'handler' => 'views_handler_argument_numeric'
    ),*/
    // Information for accepting as a filter:
    'filter' => array (
    'title' => t('Longitude'),
    'help' => t('NodeMap\'s Longitude, float number.'),
    'handler' => 'nodemap_handler_filter_float',
    ),
    // Information for sorting:
    /*
    'sort' => array (
    'handler' => 'views_handler_sort',
    )
    */
    );
    // address field:
    $data['nodemap']['nodemap_address'] = array (
    'title' => t('Address'),
    'help' => t('NodeMap\'s Address'),
    // Information for displaying:
    'field' => array (
    'handler' => 'views_handler_field',
    'click sortable' => TRUE,
    ),
    // Information for accepting as an argument:
    /*'argument' => array (
    'handler' => 'views_handler_argument_string'
    ),*/
    // Information for accepting as a filter:
    'filter' => array (
    'title' => t('Address'),
    'help' => t('NodeMap\'s Address, string.'),
    'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting:
    'sort' => array (
    'title' => t('Address'),
    'help' => t('NodeMap\'s Address, string.'),
    'handler' => 'views_handler_sort',
    )
    );
    // maprender field:
    $data['nodemap']['nodemap_maprender'] = array (
    // Information for displaying:
    'field' => array (
    'title' => t('Map'),
    'help' => t('NodeMap\'s rendered map'),
    'handler' => 'nodemap_handler_field_maprender'
    )
    );
    return $data;
}
/**
 * ...hook _views_handlers()
 */
function nodemap_views_handlers() {
    $aHandlers = array (
    'info' => array (
    'path' => drupal_get_path('module', 'nodemap') . '/views/handlers',
    ),
    'handlers' => array (
    'nodemap_handler_field_maprender' => array (
    'parent' => 'views_handler_field',
    'file' => 'nodemap_handler_field_maprender.inc'
    ),
    /*'views_handler_filter_float' => array (
    'parent' => 'views_handler_filter_numeric',
    ),*/
    'nodemap_handler_filter_float' => array (
    'parent' => 'views_handler_filter_numeric',
    'file' => 'nodemap_handler_filter_float.inc'
    )
    )
    );
    return $aHandlers;
}