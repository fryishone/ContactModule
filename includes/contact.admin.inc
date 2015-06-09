<?php

/**
 * implements hook_form()
 */
function contact_form($form, &$form_state, $contact, $submit = True){

	
	$query = new EntityFieldQuery();

	$users = entity_load('user');

	foreach ( $users as $user => $userid ) {
		$useroptions[$userid->uid] = $userid->name;
	}
	
	$form['uid']	=array(
	  '#type' =>'select', 
	  '#title' =>t('Username'),
	  '#default_value' => !empty($contact->uid) ? $contact->uid : '' ,
	  '#options' => $useroptions,
	  '#required' =>TRUE,
	);
	
	$form['firstname']	=array(
	  '#type' =>'textfield',
	  '#title' =>t('First Name'),
	  '#default_value' => $contact->firstname,
	  '#required' =>TRUE,
	);
	$form['prefername'] = array(
	  '#type'	 => 'textfield',
	  '#title'	 => t('Preferred Name'),
	  '#default_value' => $contact->prefername,
	  '#required'=> FALSE,
	);
	$form['middlename'] = array(
	  '#type'	 => 'textfield',
	  '#title'	 => t('Middle Name'),
	  '#default_value' => $contact->middlename,
	  '#required'=> FALSE,
	);
	$form['lastname'] = array(
	  '#type'	 => 'textfield',
	  '#title'	 => t('Last Name'),
	  '#default_value' => $contact->lastname,
	  '#required'=> FALSE,
	);
	if ( $submit ) {
		$form['actions'] =array(
			'#type'	=>	'actions',
			'submit' => array(
				'#type' => 'submit',
				'#value' => isset($contact->contactid) ? t('Update Contact') : t('Save Contact'),
			),
			'delete_link' => array(
				'#markup' => isset($contact->contactid) ? l(t('Delete'),
				ADMIN_CONTENT_CONTACT_MANAGE_URI . $contact->contactid . '/delete',
				array('attributes'=> array('id'=>array('status-delete-' . $contact->contactid),
				'class' => array('button remove')),
				'query'=>array('destination'=> ADMIN_CONTENT_CONTACT_URI))) : '' ),
		);
	}
	return $form;
}

/**
 * implements hook_form_validate()
 */
function contact_form_validate($form, &$form_state){
	//stub
}

/**
 * implements hook_form_submit()
 */
function contact_form_submit($form, &$form_state){

	$contact = entity_ui_form_submit_build_entity($form,$form_state);
	$entityController = entity_get_controller('contact');
	
	$contact->uname = $form['uid']['#options'][$contact->uid];
	
	$saveStatus = $entityController->save($contact);
	
	if ( $saveStatus == SAVED_NEW || $saveStatus == SAVED_UPDATED )
		drupal_set_message( t( '@entity has been saved.', array('@entity' => $contact->firstname)) );
	else
		drupal_set_message( t( 'Error saving @entity', array('@entity' => $contact->firstname)) );
	
	$form_state['redirect'] = ADMIN_CONTENT_CONTACT_URI;
}



// NOT IMPLEMENTED
function contact_bulk_delete($form, &$form_state, $contactids){
	$contactids = explode('|', $contactids);
	$contacts = Contact_load_multiple($contactids);

	$form = array();
	$form_state['contactid'] = $contactids;

	$variables = array(
	  	'type'	=>'ul',
		'items' =>array(),
		'title' => '',
		'attributes' =>array(),
	);

	foreach ($contacts as $contact){
		$variables['items'][] = $contact->contactid;
	}

	$form['summary'] = array(
	  '#markup' => theme_item_list($variables),
	);

	return confirm_form($form, t('Delete All Contacts?'), 'admin/content/contactAdmin', t('Description'), t('Delete All'), t('Cancel'));

}

// NOT IMPELEMENTED
function contact_bulk_delete_submit($form, &$form_state){
	$contactids = $form_state['contactid'];
	contact_Entity_delete_multiple($contactids);

	drupal_set_message(t('contacts deleted'));
	drupal_goto('admin/content/contactAdmin');
}










