<?php

/**
 * Email fields
 */
function contactemail_form($form, &$form_state, $emailEntity = array(), $emailoptions = array()) {

	$form['#tree'] = TRUE;

	$form['contactemail_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Addition email contacts'),
		'#prefix' => '<div id="contactemail_fieldset-wrapper">',
		'#suffix' => '</div>',
	);
	
	if (!isset($form_state['num_email'])) {
		$form_state['num_email'] = empty($emailEntity) ? 0 : count($emailEntity);
	}
	
	for ($i = 0; $i < $form_state['num_email']; $i++) {
	
		$form['contactemail_fieldset'][$i]['emailtype'] = array(
			'#type' =>'select', 
			'#title' =>t('Email Type'),
			'#default_value' => isset($emailEntity[$i]->emailtype) ? $emailEntity[$i]->emailtype : NULL,
			'#options' => $emailoptions,
			'#required' => FALSE,
		);
		
		$form['contactemail_fieldset'][$i]['email'] = array(
			'#type' => 'textfield',
			'#title' => t('Email'),
			'#default_value' => isset($emailEntity[$i]->email) ? $emailEntity[$i]->email : NULL,
			'#required' => FALSE,
		);
		
		$form['contactemail_fieldset'][$i]['preferred'] = array(
			'#type' => 'hidden',
			'#access' => TRUE,
			'#default_value' => 'N'
		);
		$form['contactemail_fieldset'][$i]['emailremove'] = array(
			'#type' => 'submit',
			'#name' => $i,
			'#value' => t('Remove email'),
			'#submit' => array('contactemail_remove'),
			'#ajax' => array(
				'callback' => 'contactemail_callback',
				'wrapper' => 'contactemail_fieldset-wrapper',
				'effect' => 'fade',
			),
		);
	}
	
	$form['add_email'] = array(
		'#type' => 'submit',
		'#value' => t('Add email information'),
		'#submit' => array('contactemail_add'),
		'#ajax' => array(
			'callback' => 'contactemail_callback',
			'wrapper' => 'contactemail_fieldset-wrapper',
			'effect' => 'fade',
		),
	);
	return $form;
}

/**
 * Add email field
 */
function contactemail_add($form, &$form_state) {
	// need to rebuild the html each time we add
	$form_state['num_email'] = $form_state['num_email'] + 1;
	$form_state['rebuild'] = TRUE;
}

/**
 * Remove email num field
 */
function contactemail_remove($form, &$form_state) {
	// need to rebuild the html each time we remove
	$shift = $form_state['triggering_element']['#name'];
	$form_state['num_email'] = $form_state['num_email'] - 1;
	
	// clear the value and rekey
	unset($form_state['input']['contactemail_fieldset'][$shift]);
	$form_state['input']['contactemail_fieldset'] = array_values($form_state['input']['contactemail_fieldset']);

	$form_state['rebuild'] = TRUE;
}

/**
 * Callback for the ajax form field to update
 */
function contactemail_callback($form, &$form_state) {
	return $form['contactemail_fieldset'];
}