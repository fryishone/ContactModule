<?php
/**
 * @file
 * The file for ADMIN forms and functionality for the DATATYPE SUB entity
 */

 
/**
 * Generates the form based on the values given. Defaults are blank.
 *
 * Implements hook_form().
 * @returns $form
 */
function conaddtype_form($form, &$form_state, $entity) {
	
	$form ['conaddtype'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Address type' ),
			'#default_value' => $entity->conaddtype,
			'#required' => TRUE,
	);
	$form ['description'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Description' ),
			'#default_value' => $entity->description,
			'#required' => TRUE
	);
	
	$form ['active'] = array (
			'#type' => 'select',
			'#title' => t ( 'Active' ),
			'#options' => array(
				'N' => t('No'),
				'Y' => t('Yes'),
			),
			'#default_value' => $entity->active,
			'#required' => TRUE
	);
	
	$form ['submit'] = array (
			'#type' => 'submit',
			'#value' => t ( 'Save' )
	);

	$form['actions'] = array(
			'#type' => 'actions',
			'delete_link' => array(
					'#markup' => !empty($entity->conaddtype) ? l(t('Delete'), ADMIN_CONTENT_CONADDTYPE_MANAGE_URI . $entity->conaddtype . '/delete',
 array('attributes' => array('id' => array('conaddtype-delete-' . $entity->conaddtype), 'class' => array('button remove')), 'query' => array('destination' => ADMIN_CONTENT_CONADDTYPE_URI))) : ''));
	return $form;
	
}

/**
 * No validation implemented as of yet
 *
 * Implements hook_form_validate().
 */
function conaddtype_form_validate($form, &$form_state) {
		/** Need to enter some validation checks prior to database submission */
}

/**
 * Implements hook_form_submit().
 * gathers form submission values and saves to database
 */
function conaddtype_form_submit($form, &$form_state) {

	$entity = entity_ui_form_submit_build_entity($form, $form_state);
	$entity->is_new = False;
	$entityController = entity_get_controller('conaddtype');
	
	//check if DISPLAYTYPE (PRIM KEY) has changed to create new vale
	if ( $form_state['complete form']['conaddtype']['#default_value'] != $entity->conaddtype ) {
		$entity->is_new = True;
	}

	$saveStatus = $entityController->save($entity);
	
	if ( $saveStatus == SAVED_NEW || $saveStatus == SAVED_UPDATED )
		drupal_set_message( t( '@conaddtype has been saved.', array('@conaddtype' => $entity->conaddtype)) );
	else
		drupal_set_message( t( 'Error saving @conaddtype', array('@conaddtype' => $entity->conaddtype)) );
	
	$form_state['redirect'] = ADMIN_CONTENT_CONADDTYPE_URI;
}



/**
 *
 *	ALL CODE PAST THIS POINT IS NOT IMPLEMENTED correctly.
 *
*/

/**
 * Confirmation before bulk deleting.
 */
function datatype_bulk_delete($form, &$form_state, $statuss) {
	$statuss = explode('|', $statuss);
	$employerstatus = Employer_load_multiple($statuss);

	$form = array();
	$form_state['status'] = $statuss;

	$variables = array(
			'type' => 'ul',
			'items' => array(),
			'title' => '',
			'attributes' => array(),
	);

	foreach ($employerstatuss as $employerstatus) {
		$variables['items'][] = $employerstatuss->status;
	}

	$form['summary'] = array(
			'#markup' => theme_item_list($variables),
	);

	return confirm_form($form, t('Delete all Status?'), 'admin/content/employerstatus', t('Placeholder description'), t('Delete all'), t('Cancel'));
}

/**
 * Implements hook_submit().
 */
function datatype_bulk_delete_submit($form, &$form_state) {
	$statuss = $form_state['statuss'];
	Employer_Status_delete_multiple($statuss);

	drupal_set_message(t('employer status deleted'));
	drupal_goto('admin/content/employerstatus');
}
