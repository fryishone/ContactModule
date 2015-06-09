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
function emailtype_form($form, &$form_state, $entity) {
	
	$form ['emailtype'] = array (
			'#type' => 'textfield',
			'#title' => t ( 'Email type' ),
			'#default_value' => $entity->emailtype,
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
					'#markup' => !empty($entity->emailtype) ? l(t('Delete'), ADMIN_CONTENT_EMAILTYPE_MANAGE_URI . $entity->emailtype . '/delete',
 array('attributes' => array('id' => array('emailtype-delete-' . $entity->emailtype), 'class' => array('button remove')), 'query' => array('destination' => ADMIN_CONTENT_EMAILTYPE_URI))) : ''));
	return $form;
	
}

/**
 * No validation implemented as of yet
 *
 * Implements hook_form_validate().
 */
function emailtype_form_validate($form, &$form_state) {
		/** Need to enter some validation checks prior to database submission */
}

/**
 * Implements hook_form_submit().
 * gathers form submission values and saves to database
 */
function emailtype_form_submit($form, &$form_state) {
	
	$entity = entity_ui_form_submit_build_entity($form, $form_state);
	$entity->is_new = False;
	$entityController = entity_get_controller('emailtype');

	//check if DISPLAYTYPE (PRIM KEY) has changed to create new vale
	if ( $form_state['complete form']['emailtype']['#default_value'] != $entity->emailtype ) {
		$entity->is_new = True;
	}

	$saveStatus = $entityController->save($entity);
	
	if ( $saveStatus == SAVED_NEW || $saveStatus == SAVED_UPDATED )
		drupal_set_message( t( '@emailtype has been saved.', array('@emailtype' => $entity->emailtype)) );
	else
		drupal_set_message( t( 'Error saving @emailtype', array('@emailtype' => $entity->emailtype)) );
	
	$form_state['redirect'] = ADMIN_CONTENT_EMAILTYPE_URI;
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
