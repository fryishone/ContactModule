<?php
/**
 * Implements hook_form().
 */
function ContactRegistration_form($form, &$form_state) {
    
    global $user;
	
	$form['#tree'] = TRUE;
	
	// load contact entity
	$contactEntity = getExistingEntities(TRUE, NULL, 'contact', array('uid' => $user->uid), array());
	if (!empty($contactEntity)) {
		$form_state['contactEntity'] = (current($contactEntity));
		$contactEntity = (current($contactEntity));
	}
	else {
		$contactEntity = entity_create('contact', array());
	}
	
	
	// load email entities
	$emailEntityArray = getExistingEntities(TRUE, NULL, 'contactemail', array('contactid' => $contactEntity->contactid), array('preferred' => 'DESC'));
	if (!empty($emailEntityArray)) {
		// multiple values here to be stored
		// we only want the keys to be stored here, just update everytime
		$form_state['emailEntity'] = $emailEntityArray;
		$emailEntity = array_shift($emailEntityArray);
	}
	else {
		$emailEntity = entity_create('contactemail', array());
	}
	
	
	// array is additional emails the $phoneEntity will be for preferred contact
	$phoneEntityArray = getExistingEntities(TRUE, NULL, 'contactphone', array('contactid' => $contactEntity->contactid), array('preferred' => 'DESC'));
	if (!empty($phoneEntityArray)) {
		// multiple values here to be stored
		// only want the keys stored here, just update everytime
		$form_state['phoneEntity'] = $phoneEntityArray;
		$phoneEntity = array_shift($phoneEntityArray);
	}
	else {
		$phoneEntity = entity_create('contactphone', array());
	}

	$addressEntityArray = getExistingEntities(TRUE, NULL, 'contactaddress', array('contactid' => $contactEntity->contactid), array('preferred' => 'DESC'));
	if (!empty($addressEntityArray)) {
		// multiple values here to be stored
		$form_state['addressEntity'] = $addressEntityArray;
		$addressEntity = array_shift($addressEntityArray);
	}
	else {
		$addressEntity = entity_create('contactaddress', array());
	}
	
	// build phonetype list
	$result = getExistingEntities(FALSE, '', 'phonetype', array('active' => 'Y'), array());
	foreach ( $result['phonetype'] as $phonetype => $phonetypeid ) {
		$phoneoptions[$phonetype] = $phonetype;
	}

	// build emailtype list
	$result = getExistingEntities(FALSE, '', 'emailtype', array('active' => 'Y'), array());
	foreach ( $result['emailtype'] as $emailtype=> $emailtypeid ) {
		$emailoptions[$emailtype] = $emailtype;
	}
	
	// build address type list
	$result = getExistingEntities(FALSE, '', 'conaddtype', array('active' => 'Y'), array());
	foreach ( $result['conaddtype'] as $conaddtype=> $conaddtypeid ) {
		$conaddoptions[$conaddtype] = $conaddtype;
	}
	
	// contact subform
	
	$form['contact_form']= array(
		'#type' => 'subform',
		'#subform_id' => 'contact_form',
		'#subform_file' => array('module' => 'Contact', 'name' => 'includes/contact.admin.inc'),
		'#subform_arguments' => array($contactEntity, False),
		'#required' => FALSE,
	);
	
	
	// PHONE SECTION
	
	$form['reqphone_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Preferred phone contact'),
		'#prefix' => '<div id="phone" class="container-inline">',
		'#suffix' => '</div>',
	);
	
	$form['reqphone_fieldset']['phonetype'] = array(
		'#type' =>'select', 
		'#title' =>t('Phone Type'),
		'#options' => $phoneoptions,
		'#required' => TRUE,
		'#default_value' => !empty($phoneEntity->phonetype) ? $phoneEntity->phonetype : NULL,
	);
		
	$form['reqphone_fieldset']['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#size' => 20,
		'#required' => TRUE,
		'#default_value' => $phoneEntity->phone
	);

	$form['reqphone_fieldset']['extension'] = array(
		'#type' => 'textfield',
		'#title' => t('EXT'),
		'#size' => 6,
		'#default_value' => $phoneEntity->extension,
		'#required' => FALSE,
	);
	
	$form['reqphone_fieldset']['preferred'] = array(
		'#type' => 'hidden',
		'#access' => TRUE,
		'#default_value' => 'Y'
	);

	// phone subform
	$form['phone_form'] = array(
		'#type' => 'subform',
		'#subform_id' => 'contactphone_form',
		'#subform_file' => array('module' => 'Contact', 'name' => 'includes/contactphone.admin'),
		'#subform_arguments' => array($phoneEntityArray, $phoneoptions),
		'#required' => FALSE,
	);

	$form['reqemail_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Preferred email contact'),
		'#prefix' => '<div id="email" class="container-inline">',
		'#suffix' => '</div>',
	);
	
	// first email
	$form['reqemail_fieldset']['emailtype'] = array(
		'#type' =>'select', 
		'#title' =>t('Email Type'),
		'#options' => $emailoptions,
		'#required' => TRUE,
		'#default_value' => !empty($emailEntity->emailtype) ? $emailEntity->emailtype : NULL,
	);
		
	$form['reqemail_fieldset']['email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#required' => TRUE,
		'#default_value' => $emailEntity->email,
	);
	
	$form['contactemail_fieldset']['preferred'] = array(
		'#type' => 'hidden',
		'#access' => TRUE,
		'#default_value' => 'Y'
	);
	// email subform
	$form['email_form'] = array(
		'#type' => 'subform',
		'#subform_id' => 'contactemail_form',
		'#subform_file' => array('module' => 'Contact', 'name' => 'includes/contactemail.admin'),
		'#subform_arguments' => array($emailEntityArray, $conaddoptions),
		'#required' => FALSE,
	);
	
	
	$form['reqaddress_fieldset'] = array(
		'#type' => 'fieldset',
		'#title' => t('Preferred address contact'),
		'#prefix' => '<div id="address" class="container-inline">',
		'#suffix' => '</div>',
	);
	
	// first address
	$form['reqaddress_fieldset']['conaddtype'] = array(
		'#type' =>'select', 
		'#title' =>t('Address Type'),
		'#options' => $conaddoptions,
		'#required' => TRUE,
		'#default_value' => !empty($addressEntity->conaddtype) ? $addressEntity->conaddtype : NULL,
	);
		
	$form['reqaddress_fieldset']['address'] = array(
		'#type' => 'textfield',
		'#title' => t('Address'),
		'#required' => TRUE,
		'#default_value' => $addressEntity->address,
	);
	
	$form['contactaddress_fieldset']['preferred'] = array(
		'#type' => 'hidden',
		'#access' => TRUE,
		'#default_value' => 'Y'
	);
	
	// address subform
	$form['address_form'] = array(
		'#type' => 'subform',
		'#subform_id' => 'contactaddress_form',
		'#subform_file' => array('module' => 'Contact', 'name' => 'includes/contactaddress.admin'),
		'#subform_arguments' => array($addressEntityArray, $conaddoptions),
		'#required' => FALSE,
	);
	
	// submission for all values
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save')
    );
	
    return $form;
}

/**
 * validation placeholder
 */
function ContactRegistration_form_validate($form, &$form_state) {
  // stub
}

/**
 * Page for contactregistration (called from Contact
 */
function ContactRegistration_page() {

	module_load_include('inc', 'Contact', 'includes/contact.admin');
	module_load_include('inc', 'Contact', 'includes/addresscontact.admin');

	$build[] = drupal_get_form('ContactRegistration_form');

	return $build;
}

/**
 * Implements hook_form_submit().
 */
function ContactRegistration_form_submit($form, &$form_state) {

	// first part is if form is new we need to submit contact to get contact ID back
	
	
	if ( !empty($form_state['contactEntity']) ) {
		//cast back to array to merge values
		$contactOldEntity = (array) $form_state['contactEntity'];
		$form_state['values']['contact_form'] += $contactOldEntity;
		$CONTACTIDVALUE = $contactOldEntity['contactid'];
	}
	
	$contactEntity = $form_state['values']['contact_form'];
	$contactEntity = entity_create('contact', $contactEntity);
	
	if ( $contactEntity->contactid != '' ) 
			$contactEntity->is_new = FALSE;
	
	entity_save('contact', $contactEntity);
	
	
	// if this is a new contact we will need to pull the current contact ID from the database
	if (!isset($CONTACTIDVALUE)) {
		$contactEntity = getExistingEntities(TRUE, NULL, 'contact', array('uid' => $user->uid), array());
		$CONTACTIDVALUE = current($contactEntity)->contactid;
	}
	
	
	
	
	// save phone
	// deal with existing updates first
	if ( !empty($form_state['phoneEntity']) ) {
	
		$prefOldPhoneEntity = (array) array_shift($form_state['phoneEntity']);
		$form_state['values']['reqphone_fieldset'] += $prefOldPhoneEntity;
		
		// hold values of phone entity/form so its easier to deal with
		$phoneEntityArray = isset($form_state['phoneEntity']) ? $form_state['phoneEntity'] : array();
		$phoneformArray = isset($form_state['values']['phone_form']['contactphone_fieldset']) ? $form_state['values']['phone_form']['contactphone_fieldset'] : array();
	}

	// save preferred number
	$prefPhoneEntity = $form_state['values']['reqphone_fieldset'];
	$prefPhoneEntity = entity_create('contactphone', $prefPhoneEntity);
	
	if ( $prefPhoneEntity->contactphoneid != '' ) {
		$prefPhoneEntity->is_new = FALSE;
	}
	else {
		$prefPhoneEntity->contactid = $CONTACTIDVALUE;
	}
	
	if ( $prefPhoneEntity->extension == '' ) {
		unset($prefPhoneEntity->extension);
	}
	submitVariableList('contactphone', $CONTACTIDVALUE, $phoneEntityArray, $phoneformArray);
	entity_save('contactphone', $prefPhoneEntity);

	
	// save email
	// deal with existing updates first
	if ( !empty($form_state['emailEntity']) ) {
	
		$prefOldEmailEntity = (array) array_shift($form_state['emailEntity']);
		$form_state['values']['reqemail_fieldset'] += $prefOldEmailEntity;
		
		// hold values of email entity/form so its easier to deal with
		$emailEntityArray = isset($form_state['emailEntity']) ? $form_state['emailEntity'] : array();
		$emailformArray = isset($form_state['values']['email_form']['contactemail_fieldset']) ? $form_state['values']['email_form']['contactemail_fieldset'] : array();
		
	}

	// save preferred number
	$prefEmailEntity = $form_state['values']['reqemail_fieldset'];
	$prefEmailEntity = entity_create('contactemail', $prefEmailEntity);
	
	if ( $prefEmailEntity->contactemailid != '' ) {
		$prefEmailEntity->is_new = FALSE;
	}
	else {
		$prefEmailEntity->contactid = $CONTACTIDVALUE;
	}
	
	submitVariableList('contactemail', $CONTACTIDVALUE, $emailEntityArray, $emailformArray);
	entity_save('contactemail', $prefEmailEntity);
	
	// save address
	// deal with existing updates first
	if ( !empty($form_state['addressEntity']) ) {
	
		$prefOldAddressEntity = (array) array_shift($form_state['addressEntity']);
		$form_state['values']['reqaddress_fieldset'] += $prefOldAddressEntity;
		
		// hold values of address entity/form so its easier to deal with
		$addressEntityArray = isset($form_state['addressEntity']) ? $form_state['addressEntity'] : array();
		$addressformArray = isset($form_state['values']['address_form']['contactaddress_fieldset']) ? $form_state['values']['address_form']['contactaddress_fieldset'] : array();
	}

	// save preferred number
	$prefAddressEntity = $form_state['values']['reqaddress_fieldset'];
	$prefAddressEntity = entity_create('contactaddress', $prefAddressEntity);
	
	if ( $prefAddressEntity->contactaddressid != '' ) {
		$prefAddressEntity->is_new = FALSE;
	}
	else {
		$prefAddressEntity->contactid = $CONTACTIDVALUE;
	}
	submitVariableList('contactaddress', $CONTACTIDVALUE, $addressEntityArray, $addressformArray);
	entity_save('contactaddress', $prefAddressEntity);

	// send email block
	/*
	if ($sendEmail) {
		
		//send email to user that registered account letting them know it has been registered
		$valid_email = 'noreply@dailystrats.ca';
		$from = 'noreply@dailystrats.ca';
		$to = $user->mail; 
		$params = array(
		'account' => $user,
		'companyname' => $employerEntity['number'],
		);
		
		if (drupal_mail('Employer', 'employerregistration', $to, language_default(), $params, $from, TRUE)) {
			drupal_set_message('Thanks, we will be in contact with more information soon.');
		} 
		else {
			drupal_set_message('There was an error with the email service');
		}
		
		//send email to admin (i.e. employers@saskcareers.ca)
		$to = 'employers@dailystrats.ca';
		$params = array(
		'account' => $user,
		'companyname' => $employerEntity['number'],
		'employer_url' => 'http://dailystrats.ca/employer/' . $employerIDVALUE,
		);
		
		if (drupal_mail('Employer', 'employerregistration_admin', $to, language_default(), $params, $from, TRUE)) {
			//do nothing fornow
		} 
		else {
			drupal_set_message('There was an error with the email service');
		}
    }
	*/
	$form_state['redirect'] = 'contactregistration';
}

function getFileType($filename) {
    $filetype = substr($filename, strrpos($filename, ".") + 1);   
    return strtolower($filetype);
}

/* currently not being used */
/* It disables the default upload button that comes with this #managed_file form */
function ContactRegistration_file_element_process($element, &$form_state, $form) {
	//$element = file_managed_file_process($element, $form_state, $form);
	return $element;
}


function submitVariableList($entityName, $CONTACTIDVALUE, $entityArray = array(), $formArray = array()) {
	$updateList = array();
	$deleteList = array();
		// process unknown number of fields
	if ( count($entityArray > 0) ) {
		for ($index = 0; $index< count($entityArray); $index++) {
				
			if ( array_key_exists( $index, $formArray) ) {
		
				$tempEntity = $formArray[$index];
				$tempEntity += (array)$entityArray[$index];
				$tempEntity = entity_create($entityName, $tempEntity);
					
				$tempEntity->contactid = $CONTACTIDVALUE;
				$tempEntity->is_new = FALSE;
					
				$updateList += array($index => $tempEntity);
				//unset every value we map to an existing entity so it doesnt get saved twice
				unset($formArray[$index]);
			}
			else {
				$property = $entityName . 'id';
				$deleteList += array($index => $entityArray[$index]->$property);
			}
		}
	}
	//delete 
	if (count($deleteList) > 0 ) 
		entity_delete_multiple($entityName, $deleteList);
	
	// update
	foreach ($updateList as $key => $value) {
		entity_save($entityName, $value);
	}
	// insert
	
	foreach ($formArray as $key => $value ) {
		$value['contactid'] = $CONTACTIDVALUE;
		$entity = entity_create($entityName, $value);
		dpm($entity);
		entity_save($entityName, $entity);
	}
}

/**
 *
 * @params $returnFullEntity(return entire entity), $employerID(if exists employerid), $entityName(the entity to query)
 * $propertyConditions (filter conditions), $sortValues(sort by values)
 * @returns keys (or full entity based on property conditions)
 */
 
function getExistingEntities($returnFullEntity = TRUE, $employerID = '', $entityName, $propertyConditions = array(), $sortValues = array()) {
		
		$query = new EntityFieldQuery();
		$query->entityCondition( 'entity_type', $entityName );
		if ( !empty($employerID) ) {
			$query->propertyCondition('employerId', $employerID, '=' );
		}
		
		foreach ( $propertyConditions as $propertyCondition => $propertyValue ) {
			$query->propertyCondition($propertyCondition, $propertyValue, '=');
		}
		foreach ( $sortValues as $sortColumn => $direction ) {
			$query->propertyOrderBy($sortColumn, $direction);
		}
		
		$results         = $query->execute();
		if ( $returnFullEntity ) {
			$results = !empty( $results[ $entityName ] ) ? $results[ $entityName ] : array();
			$results   = !empty( $results ) ? entity_load_multiple_by_name($entityName, array_keys( $results ), null, null) : array();
		}
		return $results;
}

