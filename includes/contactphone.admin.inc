<?php

/**
 * Phone number addition fields
 */
function contactphone_form($form, &$form_state, $phoneEntity = array(), $phoneoptions = array()) {

	$form['#tree'] = TRUE;
	// first phone number

	if (!isset($form_state['num_phone'])) {
		$form_state['num_phone'] = empty($phoneEntity) ? 0 : count($phoneEntity);
	}
	
	$form['contactphone_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Additional Phone Numbers'),
		'#prefix' => '<div id="contactphone_fieldset-wrapper" style>',
		'#suffix' => '</div>',
	);
	
	for ($i = 0;$i < $form_state['num_phone']; $i++) {
		
		$form['contactphone_fieldset'][$i]['phonetype'] = array(
			'#type' =>'select',
			'#title' =>t('Phone Type'),
			'#options' => $phoneoptions,
			'#default_value' => isset($phoneEntity[$i]->phonetype) ? $phoneEntity[$i]->phonetype : NULL,
			'#prefix' => '<div id=."' . $i .'" class="container-inline">',
			'#required' => FALSE,
		);
		
		$form['contactphone_fieldset'][$i]['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone Number'),
			'#size' => 20,
			'#default_value' => isset($phoneEntity[$i]->phone) ? $phoneEntity[$i]->phone : NULL,
			'#required' => FALSE,
		);
		$form['contactphone_fieldset'][$i]['preferred'] = array(
			'#type' => 'hidden',
			'#access' => TRUE,
			'#default_value' => 'N'
		);
		
		$form['contactphone_fieldset'][$i]['extension'] = array(
			'#type' => 'textfield',
			'#title' => t('EXT'),
			'#suffix' => '</div>',
			'#size' => 6,
			'#default_value' => $phoneEntity[$i]->extension,
			'#required' => FALSE,
		);
		$form['contactphone_fieldset'][$i]['remove'] = array(
			'#type' => 'submit',
			'#name' => $i,
			'#value' => t('Remove phone number'),
			'#submit' => array('contactphone_remove'),
			'#ajax' => array(
				'callback' => 'contactphone_callback',
				'wrapper' => 'contactphone_fieldset-wrapper',
				'effect' => 'fade',
			),
			'#prefix' => '<div id=."' . $i . '">',
			'#suffix' => '</div>',
		);
	}
	
	$form['add_number'] = array(
		'#type' => 'submit',
		'#value' => t('Add phone number'),
		'#submit' => array('contactphone_add'),
		'#ajax' => array(
			'callback' => 'contactphone_callback',
			'wrapper' => 'contactphone_fieldset-wrapper',
			'effect' => 'fade',
		),
	);
	return $form;
}

/**
 * Add phone num field
 */
function contactphone_add($form, &$form_state) {
	// need to rebuild the html each time we add
	$form_state['num_phone'] = $form_state['num_phone'] + 1;
	$form_state['rebuild'] = TRUE;
}

/**
 * Remove phone num field
 */
function contactphone_remove($form, &$form_state) {
	// need to rebuild the html each time we remove
	$shift = $form_state['triggering_element']['#name'];
	$form_state['num_phone'] = $form_state['num_phone'] - 1;
	
	// clear the value and rekey
	unset($form_state['input']['contactphone_fieldset'][$shift]);	
	$form_state['input']['contactphone_fieldset'] = array_values($form_state['input']['contactphone_fieldset']);

	$form_state['rebuild'] = TRUE;
}

/**
 * Callback for the ajax form field to update
 */
function contactphone_callback($form, &$form_state) {
	return $form['contactphone_fieldset'];
}

