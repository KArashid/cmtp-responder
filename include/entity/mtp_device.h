/*
 * Copyright (c) 2012, 2013 Samsung Electronics Co., Ltd.
 * Copyright (c) 2019 Collabora Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MTP_DEVICE_H_
#define _MTP_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mtp_store.h"
#include "mtp_property.h"

/*This number can be changed based on MAX number or stores allowed*/
#define	MAX_NUM_DEVICE_STORES		1

#define MTP_STANDARD_VERSION		0x64
#define MTP_VENDOR_EXTN_ID		0x06
#define MTP_VENDOR_EXTN_VERSION		0x64

/* Defines the status of the Device */
typedef enum {
	DEVICE_STATUSOK = 0,		/* Device OK */
	DEVICE_DEVICEERROR = 4		/* Fatal device error, can't continue */
} device_status_t;

/* Defines the phase in which device is*/
typedef enum {
	DEVICE_PHASE_NOTREADY = 0,	/* busy state */
	DEVICE_PHASE_IDLE = 1,		/* idle state */
	DEVICE_PHASE_DATAIN = 3,	/* data-in phase */
	DEVICE_PHASE_DATAOUT = 4,	/* data out phase */
	DEVICE_PHASE_RESPONSE = 5	/* response phase */
} device_phase_t;

/*
 * device_info_t structure
 * brief Structure for MTP device information.
 */
typedef struct {
	mtp_uint16 std_version;		/* Version of the MTP spec supported */
	mtp_uint32 vendor_extn_id;	/* Vendor extension ID */
	mtp_uint16 vendor_extn_version;	/* Vendor extension version */
	ptp_string_t vendor_extn_desc;	/* Vendor extension description */
	mtp_uint16 functional_mode;	/* Funtional mode. */
	mtp_uint16 *ops_supported;	/* Array of operations supported */
	mtp_uint16 *events_supported;	/* Array of events supported */
	mtp_uint16 *device_prop_supported;/* Array of device prop supported */
	mtp_uint16 *capture_fmts;	/* Array of captured fmts supported */
	mtp_uint16 *object_fmts;	/* Array of file fmts supported */
	ptp_string_t manufacturer;	/* The manufacturer's name. */
	ptp_string_t model;		/* model name string */
	ptp_string_t device_version;	/* Device version string */
	ptp_string_t serial_no;		/* The serial number string. */
} device_info_t;

/*
 * MTP device structure.
 * A structure containing an instance of the MTP device.
 */
typedef struct {
	device_status_t status;		/* device status */
	device_phase_t phase;		/* device phase */
	device_info_t device_info;	/* Device information */
	mtp_store_t *store_list;	/* pointer to List of stores */
	mtp_uchar num_stores;		/* Number of valid stores */
	/* Used when SendObjectInfo does not specift store_id */
	mtp_uint32 default_store_id;
	/* Used when SendObjectInfo doesn't specify Parent Object Handle */
	mtp_uint32 default_hparent;
	mtp_bool is_mounted[MAX_NUM_DEVICE_STORES];
} mtp_device_t;

extern mtp_device_t *g_device;

/*
 * void _init_mtp_device(void)
 * This function initializes mtp device structure.
 * @return	none.
 */
void _init_mtp_device(void);

/*
 * mtp_uint32 _get_device_info_size()
 * This function returns the size of device_info structure
 * @return	returns the size of this device_info.
 */
mtp_uint32 _get_device_info_size(void);

/*
 * mtp_uint32 _pack_device_info(mtp_uchar *buf, mtp_uint32 buf_sz)
 * This Functions Fills the device_info structure
 * @param[in]	buf	address of a buffer
 * @param[in]	buf_sz	size of the buffer
 * @return the length of useful data in the buffer.
 */
mtp_uint32 _pack_device_info(mtp_uchar *buf, mtp_uint32 buf_sz);

/*
 * void _reset_mtp_device()
 * This functions resets device state to IDLE/Command Ready
 * @return	none
 */
void _reset_mtp_device(void);

/*
 * mtp_bool _device_install_storage(void)
 * This function add the storage.
 * @return	If success, returns TRUE. Otherwise returns FALSE.
 */
mtp_bool _device_install_storage(void);

/* mtp_bool _device_uninstall_storage(void)
 * This function removes the storage.
 * @return	If success, returns TRUE. Otherwise returns FALSE.
 */
mtp_bool _device_uninstall_storage(void);

/*
 * mtp_store_t *_device_get_store(mtp_uint32 store_id)
 * This function will get the store with store_id.
 * @param[in]	store_id	ID of the store
 * @return	the pointer to the store with store ID, otherwise NULL.
 */
mtp_store_t *_device_get_store(mtp_uint32 store_id);

/*
 * mtp_uint32 _device_get_store_ids(ptp_array_t *store_ids)
 * This function returns a list of storage ID's of all stores in the device.
 * @param[out]	store_ids		reference to the store ID list
 * @return	the number of Storage IDs in the list.
 */
mtp_uint32 _device_get_store_ids(ptp_array_t *store_ids);

/*
 * mtp_obj_t *_device_get_object_with_handle(mtp_uint32 obj_handle)
 * This fuctions returns the object with the obj_handle
 * @param[in]	obj_handle	object handle of the desired object
 * @return	the object pointer if find; otherwise NULL;
 */
mtp_obj_t *_device_get_object_with_handle(mtp_uint32 obj_handle);

/*
 * mtp_obj_t *_device_get_object_with_path(mtp_char *full_path)
 * This function return the pointer to the object if it exists.
 * @param[in]	full_path		points to the object full path
 * @return	the object pointer if found; otherwise NULL;
 */
mtp_obj_t *_device_get_object_with_path(mtp_char *full_path);

/*
 * mtp_uint16 _device_delete_object(mtp_uint32 obj_handle, mtp_uint32 fmt)
 * This function will delete the objects entry corresponding to the
 * object handle and the formatcode.
 * @param[in]	obj_handle	Object handle number
 * @param[in]	fmt		Object formatCode
 * @return	the response of the delete.
 */
mtp_uint16 _device_delete_object(mtp_uint32 obj_handle, mtp_uint32 fmt);

/*
 * mtp_store_t *_device_get_store_containing_obj(mtp_uint32 obj_handle)
 * This function will return the pointer to the store where the the object is.
 * @param[in]	obj_handle	Object handle number
 * @return	the store pointer if find, otherwise NULL.
 */
mtp_store_t *_device_get_store_containing_obj(mtp_uint32 obj_handle);
mtp_store_t *_device_get_store_at_index(mtp_uint32 index);

#ifdef __cplusplus
}
#endif

#endif /* _MTP_DEVICE_H_ */
