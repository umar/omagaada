<?php
/**
 * Enter description here...
 *
 */
class nodemap_handler_field_maprender extends views_handler_field {
    function option_definition() {
        $options = parent::option_definition();
        $options['width'] = array('default' => variable_get('nodemap_node_block_view_container_width', 450));
        $options['height'] = array('default' => variable_get('nodemap_node_block_view_container_height', 300));
        $options['type'] = array('default' => variable_get('nodemap_node_block_view_default_map_type', "map"));
        $options['zoom'] = array('default' => variable_get('nodemap_node_block_view_default_map_zoom_level', 5));
        $options['showtitle'] = array('default' => variable_get('nodemap_node_block_view_show_title', 1));
        $options['showaddress'] = array('default' => variable_get('nodemap_node_block_view_show_address', 1));
        return $options;
    }
    function options_form(&$form, &$form_state) {
        parent::options_form($form, $form_state);
        $form['width'] = array (
        '#type' => 'textfield',
        '#title' =>t ('Map width'),
        '#size' => 4,
        '#maxlength' => 4,
        '#default_value' => $this->options['width'],
        '#description' => t('Enter map width in pixels.')
        );
        $form['height'] = array (
        '#type' => 'textfield',
        '#title' =>t ('Map height'),
        '#size' => 4,
        '#maxlength' => 4,
        '#default_value' => $this->options['height'],
        '#description' => t('Enter map height in pixels.')
        );
        $form['type'] = array (
        '#type' => 'radios',
        '#title' => t('Map type'),
        '#options' => array (
        'map' => t('Map'),
        'sat' => t('Satellite'),
        'hyb' => t('Hybrid')
        ),
        '#default_value' => $this->options['type'],
        '#description' => t('Enter default map type.')
        );
        $form['zoom'] = array (
        '#type' => 'textfield',
        '#title' => t('Map zoom'),
        '#size' => 2,
        '#maxlength' => 2,
        '#default_value' => $this->options['zoom'],
        '#description' => t('Enter default map zoom level. Beetween <b>1 & 16</b>.')
        );
        $form['showtitle'] = array (
        '#type' => 'checkbox',
        '#title' => t('Show node title'),
        '#default_value' => !empty($this->options['showtitle']),
        '#description' => t('Check this if you want to show the node title on the map marker.')
        );
        $form['showaddress'] = array (
        '#type' => 'checkbox',
        '#title' => t('Show address'),
        '#default_value' => !empty($this->options['showaddress']),
        '#description' => t('Check this if you want to show the address on the map marker.')
        );
    }
    function query() {
        $this->ensure_my_table();
    }
    function render($values) {
        //return "<pre>".print_r($values, true)."</pre>";
        $aMapRender = nodemap_load_map(node_load(array('nid'=>$values->nid)), false, ($this->options['showtitle']==1?true:false), ($this->options['showaddress']==1?true:false), true, true, $this->options);
        return $aMapRender['content'];
    }
}