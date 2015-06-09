<?php

/**
 * Phone number addition fields
 */
function contactaddress_form($form, &$form_state, $addressEntity = array(), $addressoptions = array()) {

	$form['#tree'] = TRUE;
	// first address number

	if (!isset($form_state['num_address'])) {
		$form_state['num_address'] = empty($addressEntity) ? 0 : count($addressEntity);
	}
	
	$form['contactaddress_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Additional Address'),
		'#prefix' => '<div id="contactaddress_fieldset-wrapper" style>',
		'#suffix' => '</div>',
	);
	
	for ($i = 0;$i < $form_state['num_address']; $i++) {
		
		$form['contactaddress_fieldset'][$i]['addresstype'] = array(
			'#type' =>'select',
			'#title' =>t('Address Type'),
			'#options' => $addressoptions,
			'#default_value' => isset($addressEntity[$i]->addresstype) ? $addressEntity[$i]->addresstype : NULL,
			'#prefix' => '<div id=."' . $i .'" class="container-inline">',
			'#required' => FALSE,
		);
		
		$form['contactaddress_fieldset'][$i]['address'] = array(
			'#type' => 'textfield',
			'#title' => t('Address'),
			'#size' => 20,
			'#default_value' => isset($addressEntity[$i]->address) ? $addressEntity[$i]->address : NULL,
			'#required' => FALSE,
		);
		$form['contactaddress_fieldset'][$i]['preferred'] = array(
			'#type' => 'hidden',
			'#access' => TRUE,
			'#default_value' => 'N'
		);
		
		$form['contactaddress_fieldset'][$i]['remove'] = array(
			'#type' => 'submit',
			'#name' => $i,
			'#value' => t('Remove address'),
			'#submit' => array('contactaddress_remove'),
			'#ajax' => array(
				'callback' => 'contactaddress_callback',
				'wrapper' => 'contactaddress_fieldset-wrapper',
				'effect' => 'fade',
			),
			'#prefix' => '<div id=."' . $i . '">',
			'#suffix' => '</div>',
		);
		
	}
	
	$form['add_number'] = array(
		'#type' => 'submit',
		'#value' => t('Add address number'),
		'#submit' => array('contactaddress_add'),
		'#ajax' => array(
			'callback' => 'contactaddress_callback',
			'wrapper' => 'contactaddress_fieldset-wrapper',
			'effect' => 'fade',
		),
	);
	
	return $form;
}

/**
 * Add address num field
 */
function contactaddress_add($form, &$form_state) {
	// need to rebuild the html each time we add
	$form_state['num_address'] = $form_state['num_address'] + 1;
	$form_state['rebuild'] = TRUE;
}

/**
 * Remove address num field
 */
function contactaddress_remove($form, &$form_state) {
	// need to rebuild the html each time we remove
	$shift = $form_state['triggering_element']['#name'];
	$form_state['num_address'] = $form_state['num_address'] - 1;
	
	// clear the value and rekey
	unset($form_state['input']['contactaddress_fieldset'][$shift]);	
	$form_state['input']['contactaddress_fieldset'] = array_values($form_state['input']['contactaddress_fieldset']);

	$form_state['rebuild'] = TRUE;
}

/**
 * Callback for the ajax form field to update
 */
function contactaddress_callback($form, &$form_state) {
	return $form['contactaddress_fieldset'];
}

