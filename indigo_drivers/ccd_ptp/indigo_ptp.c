	// Copyright (c) 2019 CloudMakers, s. r. o.
	// All rights reserved.
	//
	// You can use this software under the terms of 'INDIGO Astronomy
	// open-source license' (see LICENSE.md).
	//
	// THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
	// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
	// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
	// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	// version history
	// 2.0 by Peter Polakovic <peter.polakovic@cloudmakers.eu>

/** INDIGO PTP implementation
 \file indigo_ptp.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <float.h>
#include <libusb-1.0/libusb.h>

#include "indigo_ptp.h"

char *ptp_operation_code_label(uint16_t code) {
	switch (code) {
		case ptp_operation_Undefined: return "Undefined";
		case ptp_operation_GetDeviceInfo: return "GetDeviceInfo";
		case ptp_operation_OpenSession: return "OpenSession";
		case ptp_operation_CloseSession: return "CloseSession";
		case ptp_operation_GetStorageIDs: return "GetStorageIDs";
		case ptp_operation_GetStorageInfo: return "GetStorageInfo";
		case ptp_operation_GetNumObjects: return "GetNumObjects";
		case ptp_operation_GetObjectHandles: return "GetObjectHandles";
		case ptp_operation_GetObjectInfo: return "GetObjectInfo";
		case ptp_operation_GetObject: return "GetObject";
		case ptp_operation_GetThumb: return "GetThumb";
		case ptp_operation_DeleteObject: return "DeleteObject";
		case ptp_operation_SendObjectInfo: return "SendObjectInfo";
		case ptp_operation_SendObject: return "SendObject";
		case ptp_operation_InitiateCapture: return "InitiateCapture";
		case ptp_operation_FormatStore: return "FormatStore";
		case ptp_operation_ResetDevice: return "ResetDevice";
		case ptp_operation_SelfTest: return "SelfTest";
		case ptp_operation_SetObjectProtection: return "SetObjectProtection";
		case ptp_operation_PowerDown: return "PowerDown";
		case ptp_operation_GetDevicePropDesc: return "GetDevicePropDesc";
		case ptp_operation_GetDevicePropValue: return "GetDevicePropValue";
		case ptp_operation_SetDevicePropValue: return "SetDevicePropValue";
		case ptp_operation_ResetDevicePropValue: return "ResetDevicePropValue";
		case ptp_operation_TerminateOpenCapture: return "TerminateOpenCapture";
		case ptp_operation_MoveObject: return "MoveObject";
		case ptp_operation_CopyObject: return "CopyObject";
		case ptp_operation_GetPartialObject: return "GetPartialObject";
		case ptp_operation_InitiateOpenCapture: return "InitiateOpenCapture";
		case ptp_operation_GetNumDownloadableObjects: return "GetNumDownloadableObjects";
		case ptp_operation_GetAllObjectInfo: return "GetAllObjectInfo";
		case ptp_operation_GetUserAssignedDeviceName: return "GetUserAssignedDeviceName";
		case ptp_operation_MTPGetObjectPropsSupported: return "MTPGetObjectPropsSupported";
		case ptp_operation_MTPGetObjectPropDesc: return "MTPGetObjectPropDesc";
		case ptp_operation_MTPGetObjectPropValue: return "MTPGetObjectPropValue";
		case ptp_operation_MTPSetObjectPropValue: return "MTPSetObjectPropValue";
		case ptp_operation_MTPGetObjPropList: return "MTPGetObjPropList";
		case ptp_operation_MTPSetObjPropList: return "MTPSetObjPropList";
		case ptp_operation_MTPGetInterdependendPropdesc: return "MTPGetInterdependendPropdesc";
		case ptp_operation_MTPSendObjectPropList: return "MTPSendObjectPropList";
		case ptp_operation_MTPGetObjectReferences: return "MTPGetObjectReferences";
		case ptp_operation_MTPSetObjectReferences: return "MTPSetObjectReferences";
		case ptp_operation_MTPUpdateDeviceFirmware: return "MTPUpdateDeviceFirmware";
		case ptp_operation_MTPSkip: return "MTPSkip";
	}
	return "???";
}

char *ptp_response_code_label(uint16_t code) {
	switch (code) {
		case ptp_response_Undefined: return "Undefined";
		case ptp_response_OK: return "OK";
		case ptp_response_GeneralError: return "GeneralError";
		case ptp_response_SessionNotOpen: return "SessionNotOpen";
		case ptp_response_InvalidTransactionID: return "InvalidTransactionID";
		case ptp_response_OperationNotSupported: return "OperationNotSupported";
		case ptp_response_ParameterNotSupported: return "ParameterNotSupported";
		case ptp_response_IncompleteTransfer: return "IncompleteTransfer";
		case ptp_response_InvalidStorageID: return "InvalidStorageID";
		case ptp_response_InvalidObjectHandle: return "InvalidObjectHandle";
		case ptp_response_DevicePropNotSupported: return "DevicePropNotSupported";
		case ptp_response_InvalidObjectFormatCode: return "InvalidObjectFormatCode";
		case ptp_response_StoreFull: return "StoreFull";
		case ptp_response_ObjectWriteProtected: return "ObjectWriteProtected";
		case ptp_response_StoreReadOnly: return "StoreReadOnly";
		case ptp_response_AccessDenied: return "AccessDenied";
		case ptp_response_NoThumbnailPresent: return "NoThumbnailPresent";
		case ptp_response_SelfTestFailed: return "SelfTestFailed";
		case ptp_response_PartialDeletion: return "PartialDeletion";
		case ptp_response_StoreNotAvailable: return "StoreNotAvailable";
		case ptp_response_SpecificationByFormatUnsupported: return "SpecificationByFormatUnsupported";
		case ptp_response_NoValidObjectInfo: return "NoValidObjectInfo";
		case ptp_response_InvalidCodeFormat: return "InvalidCodeFormat";
		case ptp_response_UnknownVendorCode: return "UnknownVendorCode";
		case ptp_response_CaptureAlreadyTerminated: return "CaptureAlreadyTerminated";
		case ptp_response_DeviceBusy: return "DeviceBusy";
		case ptp_response_InvalidParentObject: return "InvalidParentObject";
		case ptp_response_InvalidDevicePropFormat: return "InvalidDevicePropFormat";
		case ptp_response_InvalidDevicePropValue: return "InvalidDevicePropValue";
		case ptp_response_InvalidParameter: return "InvalidParameter";
		case ptp_response_SessionAlreadyOpen: return "SessionAlreadyOpen";
		case ptp_response_TransactionCancelled: return "TransactionCancelled";
		case ptp_response_SpecificationOfDestinationUnsupported: return "SpecificationOfDestinationUnsupported";
		case ptp_response_MTPUndefined: return "MTPUndefined";
		case ptp_response_MTPInvalidObjectPropCode: return "MTPInvalidObjectPropCode";
		case ptp_response_MTPInvalidObjectProp_Format: return "MTPInvalidObjectProp_Format";
		case ptp_response_MTPInvalidObjectProp_Value: return "MTPInvalidObjectProp_Value";
		case ptp_response_MTPInvalidObjectReference: return "MTPInvalidObjectReference";
		case ptp_response_MTPInvalidDataset: return "MTPInvalidDataset";
		case ptp_response_MTPSpecificationByGroupUnsupported: return "MTPSpecificationByGroupUnsupported";
		case ptp_response_MTPSpecificationByDepthUnsupported: return "MTPSpecificationByDepthUnsupported";
		case ptp_response_MTPObjectTooLarge: return "MTPObjectTooLarge";
		case ptp_response_MTPObjectPropNotSupported: return "MTPObjectPropNotSupported";
	}
	return "???";
}

char *ptp_event_code_label(uint16_t code) {
	switch (code) {
		case ptp_event_Undefined: return "Undefined";
		case ptp_event_CancelTransaction: return "CancelTransaction";
		case ptp_event_ObjectAdded: return "ObjectAdded";
		case ptp_event_ObjectRemoved: return "ObjectRemoved";
		case ptp_event_StoreAdded: return "StoreAdded";
		case ptp_event_StoreRemoved: return "StoreRemoved";
		case ptp_event_DevicePropChanged: return "DevicePropChanged";
		case ptp_event_ObjectInfoChanged: return "ObjectInfoChanged";
		case ptp_event_DeviceInfoChanged: return "DeviceInfoChanged";
		case ptp_event_RequestObjectTransfer: return "RequestObjectTransfer";
		case ptp_event_StoreFull: return "StoreFull";
		case ptp_event_DeviceReset: return "DeviceReset";
		case ptp_event_StorageInfoChanged: return "StorageInfoChanged";
		case ptp_event_CaptureComplete: return "CaptureComplete";
		case ptp_event_UnreportedStatus: return "UnreportedStatus";
		case ptp_event_AppleDeviceUnlocked: return "AppleDeviceUnlocked";
		case ptp_event_AppleUserAssignedNameChanged: return "AppleUserAssignedNameChanged";
	}
	return "???";
}

char *ptp_property_code_label(uint16_t code) {
	switch (code) {
		case ptp_property_Undefined: return "Undefined";
		case ptp_property_BatteryLevel: return "BatteryLevel";
		case ptp_property_FunctionalMode: return "FunctionalMode";
		case ptp_property_ImageSize: return "ImageSize";
		case ptp_property_CompressionSetting: return "CompressionSetting";
		case ptp_property_WhiteBalance: return "WhiteBalance";
		case ptp_property_RGBGain: return "RGBGain";
		case ptp_property_FNumber: return "FNumber";
		case ptp_property_FocalLength: return "FocalLength";
		case ptp_property_FocusDistance: return "FocusDistance";
		case ptp_property_FocusMode: return "FocusMode";
		case ptp_property_ExposureMeteringMode: return "ExposureMeteringMode";
		case ptp_property_FlashMode: return "FlashMode";
		case ptp_property_ExposureTime: return "ExposureTime";
		case ptp_property_ExposureProgramMode: return "ExposureProgramMode";
		case ptp_property_ExposureIndex: return "ExposureIndex";
		case ptp_property_ExposureBiasCompensation: return "ExposureBiasCompensation";
		case ptp_property_DateTime: return "DateTime";
		case ptp_property_CaptureDelay: return "CaptureDelay";
		case ptp_property_StillCaptureMode: return "StillCaptureMode";
		case ptp_property_Contrast: return "Contrast";
		case ptp_property_Sharpness: return "Sharpness";
		case ptp_property_DigitalZoom: return "DigitalZoom";
		case ptp_property_EffectMode: return "EffectMode";
		case ptp_property_BurstNumber: return "BurstNumber";
		case ptp_property_BurstInterval: return "BurstInterval";
		case ptp_property_TimelapseNumber: return "TimelapseNumber";
		case ptp_property_TimelapseInterval: return "TimelapseInterval";
		case ptp_property_FocusMeteringMode: return "FocusMeteringMode";
		case ptp_property_UploadURL: return "UploadURL";
		case ptp_property_Artist: return "Artist";
		case ptp_property_CopyrightInfo: return "CopyrightInfo";
		case ptp_property_SupportedStreams: return "SupportedStreams";
		case ptp_property_EnabledStreams: return "EnabledStreams";
		case ptp_property_VideoFormat: return "VideoFormat";
		case ptp_property_VideoResolution: return "VideoResolution";
		case ptp_property_VideoQuality: return "VideoQuality";
		case ptp_property_VideoFrameRate: return "VideoFrameRate";
		case ptp_property_VideoContrast: return "VideoContrast";
		case ptp_property_VideoBrightness: return "VideoBrightness";
		case ptp_property_AudioFormat: return "AudioFormat";
		case ptp_property_AudioBitrate: return "AudioBitrate";
		case ptp_property_AudioSamplingRate: return "AudioSamplingRate";
		case ptp_property_AudioBitPerSample: return "AudioBitPerSample";
		case ptp_property_AudioVolume: return "AudioVolume";
		case ptp_property_MTPSynchronizationPartner: return "MTPSynchronizationPartner";
		case ptp_property_MTPDeviceFriendlyName: return "MTPDeviceFriendlyName";
		case ptp_property_MTPVolumeLevel: return "MTPVolumeLevel";
		case ptp_property_MTPDeviceIcon: return "MTPDeviceIcon";
		case ptp_property_MTPSessionInitiatorInfo: return "MTPSessionInitiatorInfo";
		case ptp_property_MTPPerceivedDeviceType: return "MTPPerceivedDeviceType";
		case ptp_property_MTPPlaybackRate: return "MTPPlaybackRate";
		case ptp_property_MTPPlaybackObject: return "MTPPlaybackObject";
		case ptp_property_MTPPlaybackContainerIndex: return "MTPPlaybackContainerIndex";
		case ptp_property_MTPPlaybackPosition: return "MTPPlaybackPosition";
	}
	char name[INDIGO_NAME_SIZE];
	sprintf(name, "%04x", code);
	return name;
}

char *ptp_vendor_label(uint16_t code) {
	switch (code) {
		case ptp_vendor_eastman_kodak: return "Eastman Kodak";
		case ptp_vendor_seiko_epson: return "Seiko Epson";
		case ptp_vendor_agilent: return "Agilent";
		case ptp_vendor_polaroid: return "Polaroid";
		case ptp_vendor_agfa_gevaert: return "Agfa-Gevaert";
		case ptp_vendor_microsoft: return "Microsoft";
		case ptp_vendor_equinox: return "Equinox";
		case ptp_vendor_viewquest: return "Viewquest";
		case ptp_vendor_stmicroelectronics: return "STMicroelectronics";
		case ptp_vendor_nikon: return "Nikon";
		case ptp_vendor_canon: return "Canon";
		case ptp_vendor_fotonation: return "Fotonation";
		case ptp_vendor_pentax: return "Pentax";
		case ptp_vendor_fuji: return "Fuji";
		case ptp_vendor_ndd_medical_technologies: return "ndd Medical Technologies";
		case ptp_vendor_samsung: return "Samsung";
		case ptp_vendor_parrot: return "Parrot";
		case ptp_vendor_panasonic: return "Panasonic";
		case ptp_vendor_sony: return "Sony";
	}
	return "???";
}

void ptp_dump_container(int line, const char *function, indigo_device *device, ptp_container *container) {
	char buffer[PTP_MAX_CHARS];
	int offset = 0;
	switch (container->type) {
		case ptp_container_command:
			offset = sprintf(buffer, "request %s (%04x) %08x [", PRIVATE_DATA->operation_code_label(container->code), container->code, container->transaction_id);
			break;
		case ptp_container_data:
			offset = sprintf(buffer, "data %04x %08x +%u bytes", container->code, container->transaction_id, container->length - PTP_CONTAINER_HDR_SIZE);
			break;
		case ptp_container_response:
			offset = sprintf(buffer, "response %s (%04x) %08x [", PRIVATE_DATA->response_code_label(container->code), container->code, container->transaction_id);
			break;
		case ptp_container_event:
			offset = sprintf(buffer, "event %s (%04x) %08x [", PRIVATE_DATA->event_code_label(container->code), container->code, container->transaction_id);
			break;
		default:
			offset = sprintf(buffer, "unknown %04x %08x", container->code, container->transaction_id);
			break;
	}
	if (container->type == ptp_container_command || container->type == ptp_container_response || container->type == ptp_container_event) {
		if (container->length > 12) {
			offset += sprintf(buffer + offset, "%08x", container->payload.params[0]);
		}
		if (container->length > 16) {
			offset += sprintf(buffer + offset, ", %08x", container->payload.params[1]);
		}
		if (container->length > 20) {
			offset += sprintf(buffer + offset, ", %08x", container->payload.params[2]);
		}
		if (container->length > 24) {
			offset += sprintf(buffer + offset, ", %08x", container->payload.params[3]);
		}
		if (container->length > 28) {
			offset += sprintf(buffer + offset, ", %08x", container->payload.params[4]);
		}
		sprintf(buffer + offset, "]");
	}
	indigo_debug("%s[%d, %s]: %s", DRIVER_NAME, line, function,  buffer);
}

void ptp_dump_device_info(int line, const char *function, indigo_device *device) {
	indigo_log("%s[%d, %s]: device info", DRIVER_NAME, line, function);
	indigo_log("PTP %.2f + %s (%04x), %s %.2f", PRIVATE_DATA->info_standard_version / 100.0, ptp_vendor_label(PRIVATE_DATA->info_vendor_extension_id), PRIVATE_DATA->info_vendor_extension_id, PRIVATE_DATA->info_vendor_extension_desc, PRIVATE_DATA->info_vendor_extension_version / 100.0);
	indigo_log("%s [%s], %s, #%s", PRIVATE_DATA->info_model, PRIVATE_DATA->info_device_version, PRIVATE_DATA->info_manufacturer, PRIVATE_DATA->info_serial_number);
	indigo_log("operations:");
	for (uint16_t *operation = PRIVATE_DATA->info_operations_supported; *operation; operation++) {
		indigo_log("  %04x %s", *operation, PRIVATE_DATA->operation_code_label(*operation));
	}
	indigo_log("events:");
	for (uint16_t *event = PRIVATE_DATA->info_events_supported; *event; event++) {
		indigo_debug("  %04x %s", *event, PRIVATE_DATA->event_code_label(*event));
	}
	indigo_log("properties:");
	for (uint16_t *property = PRIVATE_DATA->info_properties_supported; *property; property++) {
		indigo_log("  %04x %s", *property, PRIVATE_DATA->property_code_label(*property));
	}
}

uint8_t *ptp_copy_string(uint8_t *source, char *target) {
	int length = *source++;
	for (int i = 0; i < length; i++) {
		*target++ = *source++;
		source++;
	}
	return source;
}

uint8_t *ptp_copy_uint8(uint8_t *source, uint8_t *target) {
	*target = *source++;
	return source;
}

uint8_t *ptp_copy_uint16(uint8_t *source, uint16_t *target) {
	*target = *(uint16_t *)source;
	return source + sizeof(uint16_t);
}

uint8_t *ptp_copy_uint32(uint8_t *source, uint32_t *target) {
	*target = *(uint32_t *)source;
	return source + sizeof(uint32_t);
}

uint8_t *ptp_copy_uint64(uint8_t *source, char *target) {
	uint32_t u32_1, u32_2;
	source = ptp_copy_uint32(source, &u32_1);
	source = ptp_copy_uint32(source, &u32_2);
	sprintf(target, "%04x%04x", u32_2, u32_1);
	return source;
}

uint8_t *ptp_copy_uint128(uint8_t *source, char *target) {
	uint32_t u32_1, u32_2, u32_3, u32_4;
	source = ptp_copy_uint32(source, &u32_1);
	source = ptp_copy_uint32(source, &u32_2);
	source = ptp_copy_uint32(source, &u32_3);
	source = ptp_copy_uint32(source, &u32_4);
	sprintf(target, "%04x%04x%04x%04x", u32_4, u32_3, u32_2, u32_1);
	return source;
}

uint8_t *ptp_copy_uint16_array(uint8_t *source, uint16_t *target, uint32_t *count) {
	uint32_t length;
	source = ptp_copy_uint32(source, &length);
	assert(length < PTP_MAX_ELEMENTS);
	for (int i = 0; i < length; i++) {
		source = ptp_copy_uint16(source, target++);
	}
	*target = 0;
	if (count)
		*count = length;
	return source;
}

uint8_t *ptp_copy_uint32_array(uint8_t *source, uint32_t *target, uint32_t *count) {
	uint32_t length;
	source = ptp_copy_uint32(source, &length);
	assert(length < PTP_MAX_ELEMENTS);
	for (int i = 0; i < length; i++) {
		source = ptp_copy_uint32(source, target++);
	}
	*target = 0;
	if (count)
		*count = length;
	return source;
}

void ptp_append_uint16_32_array(uint16_t *target, uint32_t *source) {
	int index = 0;
	for (index = 0; target[index]; index++)
		;
	for (int i = 0; source[i]; i++)
		target[index++] = source[i];
	target[index] = 0;
}

uint8_t *ptp_copy_device_info(uint8_t *source, indigo_device *device) {
	source = ptp_copy_uint16(source, &PRIVATE_DATA->info_standard_version);
	source = ptp_copy_uint32(source, &PRIVATE_DATA->info_vendor_extension_id);
	source = ptp_copy_uint16(source, &PRIVATE_DATA->info_vendor_extension_version);
	source = ptp_copy_string(source, PRIVATE_DATA->info_vendor_extension_desc);
	source = ptp_copy_uint16(source, &PRIVATE_DATA->info_functional_mode);
	source = ptp_copy_uint16_array(source, PRIVATE_DATA->info_operations_supported, NULL);
	source = ptp_copy_uint16_array(source, PRIVATE_DATA->info_events_supported, NULL);
	source = ptp_copy_uint16_array(source, PRIVATE_DATA->info_properties_supported, NULL);
	source = ptp_copy_uint16_array(source, PRIVATE_DATA->info_capture_formats_supported, NULL);
	source = ptp_copy_uint16_array(source, PRIVATE_DATA->info_image_formats_supported, NULL);
	source = ptp_copy_string(source, PRIVATE_DATA->info_manufacturer);
	source = ptp_copy_string(source, PRIVATE_DATA->info_model);
	source = ptp_copy_string(source, PRIVATE_DATA->info_device_version);
	source = ptp_copy_string(source, PRIVATE_DATA->info_serial_number);
	if (PRIVATE_DATA->info_vendor_extension_id == ptp_vendor_microsoft) {
		if (strstr(PRIVATE_DATA->info_manufacturer, "Nikon")) {
			PRIVATE_DATA->info_vendor_extension_id = ptp_vendor_nikon;
			PRIVATE_DATA->info_vendor_extension_version = 100;
			strcpy(PRIVATE_DATA->info_vendor_extension_desc, "Nikon & Microsoft PTP Extensions");
		} else if (strstr(PRIVATE_DATA->info_manufacturer, "Canon")) {
			PRIVATE_DATA->info_vendor_extension_id = ptp_vendor_canon;
			PRIVATE_DATA->info_vendor_extension_version = 100;
			strcpy(PRIVATE_DATA->info_vendor_extension_desc, "Canon & Microsoft PTP Extensions");
		}
	} else if (strstr(PRIVATE_DATA->info_manufacturer, "Nikon")) {
		PRIVATE_DATA->info_vendor_extension_id = ptp_vendor_nikon;
		PRIVATE_DATA->info_vendor_extension_version = 100;
		strcpy(PRIVATE_DATA->info_vendor_extension_desc, "Nikon Extension");
	} else if (strstr(PRIVATE_DATA->info_manufacturer, "Sony")) {
		PRIVATE_DATA->info_vendor_extension_id = ptp_vendor_nikon;
		PRIVATE_DATA->info_vendor_extension_version = 100;
		strcpy(PRIVATE_DATA->info_vendor_extension_desc, "Sony Extension");
	}
	return source;
}

uint8_t *ptp_copy_property(uint8_t *source, indigo_device *device, ptp_property *target) {
	uint8_t form;
	memset(target, 0, sizeof(ptp_property));
	source = ptp_copy_uint16(source, &target->code);
	source = ptp_copy_uint16(source, &target->type);
	source = ptp_copy_uint8(source, &target->writable);
	switch (target->type) {
		case ptp_uint8_type: {
			uint8_t value;
			source = ptp_copy_uint8(source + sizeof(uint8_t), &value);
			target->value.number.value = value;
			break;
		}
		case ptp_int8_type: {
			int8_t value;
			source = ptp_copy_uint8(source + sizeof(uint8_t), (uint8_t *)&value);
			target->value.number.value = value;
			break;
		}
		case ptp_uint16_type: {
			uint16_t value;
			source = ptp_copy_uint16(source + sizeof(uint16_t), &value);
			target->value.number.value = value;
			break;
		}
		case ptp_int16_type: {
			int16_t value;
			source = ptp_copy_uint16(source + sizeof(uint16_t), (uint16_t *)&value);
			target->value.number.value = value;
			break;
		}
		case ptp_uint32_type: {
			uint32_t value;
			source = ptp_copy_uint32(source + sizeof(uint32_t), &value);
			target->value.number.value = value;
			break;
		}
		case ptp_int32_type: {
			int32_t value;
			source = ptp_copy_uint32(source + sizeof(uint32_t), (uint32_t *)&value);
			target->value.number.value = value;
			break;
		}
		case ptp_uint64_type:
		case ptp_int64_type: {
			source = ptp_copy_uint64(source + 2 * sizeof(uint32_t), target->value.text.value);
			break;
		}
		case ptp_uint128_type:
		case ptp_int128_type: {
			source = ptp_copy_uint128(source + 4 * sizeof(uint32_t), target->value.text.value);
			break;
		}
		case ptp_str_type: {
			source += *source *2 + 1;
			source = ptp_copy_string(source, target->value.text.value);
			break;
		}
		default:
			assert(false);
	}
	source = ptp_copy_uint8(source, &form);
	switch (form) {
		case ptp_none_form:
			if (target->type <= ptp_uint32_type) {
				target->value.number.min = DBL_MIN;
				target->value.number.max = DBL_MAX;
				target->value.number.step = 0;
			}
			break;
		case ptp_range_form:
			switch (target->type) {
				case ptp_uint8_type: {
					uint8_t min, max, step;
					source = ptp_copy_uint8(source, &min);
					source = ptp_copy_uint8(source, &max);
					source = ptp_copy_uint8(source, &step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_int8_type: {
					int8_t min, max, step;
					source = ptp_copy_uint8(source, (uint8_t *)&min);
					source = ptp_copy_uint8(source, (uint8_t *)&max);
					source = ptp_copy_uint8(source, (uint8_t *)&step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_uint16_type: {
					uint16_t min, max, step;
					source = ptp_copy_uint16(source, &min);
					source = ptp_copy_uint16(source, &max);
					source = ptp_copy_uint16(source, &step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_int16_type: {
					int16_t min, max, step;
					source = ptp_copy_uint16(source, (uint16_t *)&min);
					source = ptp_copy_uint16(source, (uint16_t *)&max);
					source = ptp_copy_uint16(source, (uint16_t *)&step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_uint32_type: {
					uint32_t min, max, step;
					source = ptp_copy_uint32(source, &min);
					source = ptp_copy_uint32(source, &max);
					source = ptp_copy_uint32(source, &step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_int32_type: {
					int32_t min, max, step;
					source = ptp_copy_uint32(source, (uint32_t *)&min);
					source = ptp_copy_uint32(source, (uint32_t *)&max);
					source = ptp_copy_uint32(source, (uint32_t *)&step);
					target->value.number.min = min;
					target->value.number.max = max;
					target->value.number.step = step;
					break;
				}
				case ptp_uint64_type:
				case ptp_int64_type:
					source += 3 * 2 * sizeof(uint32_t);
					break;
				case ptp_uint128_type:
				case ptp_int128_type:
					source += 3 * 4 * sizeof(uint32_t);
					break;
				default:
					assert(false);
			}
			break;
		case ptp_enum_form:
			source = ptp_copy_uint16(source, &target->count);
			for (int i = 0; i < target->count; i++) {
				switch (target->type) {
					case ptp_uint8_type: {
						uint8_t value;
						source = ptp_copy_uint8(source, &value);
						target->value.sw.values[i] = value;
						break;
					}
					case ptp_int8_type: {
						int8_t value;
						source = ptp_copy_uint8(source, (uint8_t *)&value);
						target->value.sw.values[i] = value;
						break;
					}
					case ptp_uint16_type: {
						uint16_t value;
						source = ptp_copy_uint16(source, &value);
						target->value.sw.values[i] = value;
						break;
					}
					case ptp_int16_type: {
						int16_t value;
						source = ptp_copy_uint16(source, (uint16_t *)&value);
						target->value.sw.values[i] = value;
						break;
					}
					case ptp_uint32_type: {
						uint32_t value;
						source = ptp_copy_uint32(source, &value);
						target->value.sw.values[i] = value;
						break;
					}
					case ptp_int32_type: {
						int32_t value;
						source = ptp_copy_uint32(source, (uint32_t *)&value);
						target->value.sw.values[i] = value;
						break;
					}
					default:
						assert(false);
				}
			}
			break;
	}
	ptp_update_property(device, target);
	return source;
}

bool ptp_open(indigo_device *device) {
	pthread_mutex_lock(&PRIVATE_DATA->mutex);
	int rc = 0;
	struct libusb_device_descriptor	device_descriptor;
	libusb_device *dev = PRIVATE_DATA->dev;
	rc = libusb_get_device_descriptor(dev, &device_descriptor);
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_get_device_descriptor() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	rc = libusb_open(dev, &PRIVATE_DATA->handle);
	libusb_device_handle *handle = PRIVATE_DATA->handle;
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_open() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	if (rc >= 0) {
		if (libusb_kernel_driver_active(handle, 0) == 1) {
			rc = libusb_detach_kernel_driver(handle, 0);
			INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_detach_kernel_driver() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
		}
	}
	struct libusb_config_descriptor *config_descriptor = NULL;
	const struct libusb_interface *interface = NULL;
	for (int config = 0; config < device_descriptor.bNumConfigurations; config++) {
		rc = libusb_get_config_descriptor(dev, config, &config_descriptor);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_get_config_descriptor(%d) -> %s", config, rc < 0 ? libusb_error_name(rc) : "OK");
		if (rc < 0)
			break;
		for (int iface = 0; iface < config_descriptor->bNumInterfaces; iface++) {
			interface = config_descriptor->interface + iface;
			if (interface->altsetting->bInterfaceClass == 0x06 && interface->altsetting->bInterfaceSubClass == 0x01 && interface->altsetting->bInterfaceProtocol == 0x01) {
				INDIGO_DRIVER_DEBUG(DRIVER_NAME, "PTP CONFIG = %d IFACE = %d", config_descriptor->bConfigurationValue, interface->altsetting->bInterfaceNumber);
				break;
			}
			interface = NULL;
		}
		if (interface)
			break;
		libusb_free_config_descriptor(config_descriptor);
		config_descriptor = NULL;
	}
	if (rc >= 0 && config_descriptor) {
		int configuration_value = config_descriptor->bConfigurationValue;
		rc = libusb_set_configuration(handle, configuration_value);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_set_configuration(%d) -> %s", configuration_value, rc < 0 ? libusb_error_name(rc) : "OK");
	}
	if (rc >= 0 && interface) {
		int interface_number = interface->altsetting->bInterfaceNumber;
		rc = libusb_claim_interface(handle, interface_number);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_claim_interface(%d) -> %s", interface_number, rc < 0 ? libusb_error_name(rc) : "OK");
		int alt_settings = config_descriptor->interface->altsetting->bAlternateSetting;
		rc = libusb_set_interface_alt_setting(handle, interface_number, alt_settings);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_set_interface_alt_setting(%d, %d) -> %s", interface_number, alt_settings, rc < 0 ? libusb_error_name(rc) : "OK");
	}
	if (rc >= 0 && interface) {
		const struct libusb_endpoint_descriptor *ep = interface->altsetting->endpoint;
		int count = interface->altsetting->bNumEndpoints;
		for (int i = 0; i < count; i++) {
			if (ep[i].bmAttributes == LIBUSB_TRANSFER_TYPE_BULK) {
				int address = ep[i].bEndpointAddress;
				if ((address & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
					PRIVATE_DATA->ep_in = address;
				} else if ((address & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT) {
					PRIVATE_DATA->ep_out = address;
				}
			} else if (ep[i].bmAttributes == LIBUSB_TRANSFER_TYPE_INTERRUPT) {
				int address = ep[i].bEndpointAddress;
				if ((address & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
					PRIVATE_DATA->ep_int = address;
				}
			}
		}
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "PTP EP OUT = %02x IN = %02x INT = %02x", PRIVATE_DATA->ep_out, PRIVATE_DATA->ep_in, PRIVATE_DATA->ep_int);
	}
	if (config_descriptor)
		libusb_free_config_descriptor(config_descriptor);
	pthread_mutex_unlock(&PRIVATE_DATA->mutex);
	return true;
}

bool ptp_request(indigo_device *device, uint16_t code, int count, ...) {
	ptp_container request;
	request.length = PTP_CONTAINER_COMMAND_SIZE(count);
	request.type = ptp_container_command;
	request.code = code;
	request.transaction_id = PRIVATE_DATA->transaction_id++;
	int length = 0;
	va_list argp;
	va_start(argp, count);
	for (int i = 0; i < count; i++)
		request.payload.params[i] = (va_arg(argp, uint32_t));
	va_end(argp);
	for (int i = count; i < 5; i++)
		request.payload.params[i] = 0;
	PTP_DUMP_CONTAINER(&request);
	pthread_mutex_lock(&PRIVATE_DATA->mutex);
	int rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_out, (unsigned char *)&request, request.length, &length, PTP_TIMEOUT);
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer(%d) -> %s", length, rc < 0 ? libusb_error_name(rc) : "OK");
	if (rc < 0) {
		rc = libusb_clear_halt(PRIVATE_DATA->handle, PRIVATE_DATA->ep_out);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_clear_halt() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
		rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_out, (unsigned char *)&request, request.length, &length, PTP_TIMEOUT);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	}
	pthread_mutex_unlock(&PRIVATE_DATA->mutex);
	return rc >= 0;
}

bool ptp_read(indigo_device *device, uint16_t *code, void **data, int *size) {
	pthread_mutex_lock(&PRIVATE_DATA->mutex);
	ptp_container header;
	int length = 0;
	int rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_in, (unsigned char *)&header, sizeof(header), &length, PTP_TIMEOUT);
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	if (rc < 0) {
		pthread_mutex_unlock(&PRIVATE_DATA->mutex);
		return false;
	}
	PTP_DUMP_CONTAINER(&header);
	if (header.type != ptp_container_data) {
		pthread_mutex_unlock(&PRIVATE_DATA->mutex);
		INDIGO_DRIVER_ERROR(DRIVER_NAME, "ptp_read() failed -> %s", PRIVATE_DATA->response_code_label(header.code));
		return false;
	}
	int total = header.length - PTP_CONTAINER_HDR_SIZE;
	if (code)
		*code = header.code;
	if (size)
		*size = total;
	unsigned char *buffer = malloc(total);
	assert(buffer != NULL);
	memcpy(buffer, &header.payload, length - PTP_CONTAINER_HDR_SIZE);
	int offset = length;
	total -= length;
	while (total > 0) {
		rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_in, buffer + offset, total, &length, PTP_TIMEOUT);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
		if (rc < 0) {
			pthread_mutex_unlock(&PRIVATE_DATA->mutex);
			free(buffer);
			return false;
		}
		offset += length;
		total -= length;
	}
	pthread_mutex_unlock(&PRIVATE_DATA->mutex);
	*data = buffer;
	return true;
}

bool ptp_response(indigo_device *device, uint16_t *code, int count, ...) {
	ptp_container response;
	int length = 0;
	memset(&response, 0, sizeof(response));
	pthread_mutex_lock(&PRIVATE_DATA->mutex);
	int rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_in, (unsigned char *)&response, sizeof(response), &length, PTP_TIMEOUT);
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	if (rc < 0) {
		rc = libusb_clear_halt(PRIVATE_DATA->handle, PRIVATE_DATA->ep_in);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_clear_halt() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
		rc = libusb_bulk_transfer(PRIVATE_DATA->handle, PRIVATE_DATA->ep_in, (unsigned char *)&response, sizeof(response), &length, PTP_TIMEOUT);
		INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_bulk_transfer() -> %s", rc < 0 ? libusb_error_name(rc) : "OK");
	}
	pthread_mutex_unlock(&PRIVATE_DATA->mutex);
	if (rc < 0)
		return false;
	PTP_DUMP_CONTAINER(&response);
	if (code)
		*code = response.code;
	va_list argp;
	va_start(argp, count);
	for (int i = 0; i < count; i++)
		*va_arg(argp, uint32_t *) = response.payload.params[i];
	va_end(argp);
	return response.code == ptp_response_OK;
}

void ptp_close(indigo_device *device) {
	pthread_mutex_lock(&PRIVATE_DATA->mutex);
	libusb_close(PRIVATE_DATA->handle);
	INDIGO_DRIVER_DEBUG(DRIVER_NAME, "libusb_close()");
	PRIVATE_DATA->handle = NULL;
	pthread_mutex_unlock(&PRIVATE_DATA->mutex);
}

bool ptp_initialise(indigo_device *device) {
	void *buffer = NULL;
	if (ptp_request(device, ptp_operation_GetDeviceInfo, 0) && ptp_read(device, NULL, &buffer, NULL) && ptp_response(device, NULL, 0)) {
		ptp_copy_device_info(buffer, device);
		PTP_DUMP_DEVICE_INFO();
		if (buffer)
			free(buffer);
		buffer = NULL;
		uint16_t *properties = PRIVATE_DATA->info_properties_supported;
		for (int i = 0; properties[i]; i++) {
			if (ptp_request(device, ptp_operation_GetDevicePropDesc, 1, properties[i]) && ptp_read(device, NULL, &buffer, NULL) && ptp_response(device, NULL, 0)) {
				ptp_copy_property(buffer, device, PRIVATE_DATA->properties + i);
				if (buffer)
					free(buffer);
				buffer = NULL;
			}
		}
		return true;
	}
	if (buffer)
		free(buffer);
	return false;
}

bool ptp_update_property(indigo_device *device, ptp_property *property) {
	bool define = false, delete = false;
	if (property->property == NULL) {
		define = true;
		char name[INDIGO_NAME_SIZE], group[16];
		strcpy(name, PRIVATE_DATA->property_code_label(property->code));
		if (strncmp(name, "DSLR_", 5))
			strcpy(group, "Advanced");
		else
			strcpy(group, "DSLR");
		indigo_property_perm perm = property->writable ? INDIGO_RW_PERM : INDIGO_RO_PERM;
		if (property->count == 0) {
			if (property->type == ptp_str_type) {
				property->property = indigo_init_text_property(NULL, device->name, name, group, name, INDIGO_OK_STATE, perm, 1);
				indigo_init_text_item(property->property->items, "VALUE", "Value", property->value.text.value);
			} else {
				property->property = indigo_init_number_property(NULL, device->name, name, group, name, INDIGO_OK_STATE, perm, 1);
				indigo_init_number_item(property->property->items, "VALUE", "Value", property->value.number.min, property->value.number.max, property->value.number.step, property->value.number.value);
			}
		} else {
			indigo_init_switch_property(NULL, device->name, name, group, name, INDIGO_OK_STATE, perm, INDIGO_ONE_OF_MANY_RULE, property->count);
			char str[INDIGO_VALUE_SIZE];
			for (int i = 0; i < property->count; i++) {
				sprintf(str, "%g", property->value.sw.values[i]);
				indigo_init_switch_item(property->property->items + i, str, str, property->value.sw.value == property->value.sw.values[i]);
			}
		}
	} else {
		delete = true;
		if (property->count == 0) {
			if (property->type == ptp_str_type) {
				strcpy(property->property->items->text.value, property->value.text.value);
			} else if (property->value.number.min == property->property->items->number.min && property->value.number.max == property->property->items->number.max && property->value.number.step == property->property->items->number.step) {
				property->property->items->number.value = property->value.number.value;
			} else {
				property->property->items->number.min = property->value.number.min;
				property->property->items->number.max = property->value.number.max;
				property->property->items->number.step = property->value.number.step;
				property->property->items->number.value = property->value.number.value;
				define = true;
			}
		} else {
			if (property->property->count != property->count) {
				property->property = indigo_resize_property(property->property, property->count);
				define = true;
			}
			char str[INDIGO_VALUE_SIZE];
			for (int i = 0; i < property->count; i++) {
				sprintf(str, "%g", property->value.sw.values[i]);
				if (strcmp(property->property->items[i].name, str)) {
					strcpy(property->property->items[i].name, str);
					define = true;
				}
				property->property->items[i].sw.value = (property->value.sw.value == property->value.sw.values[i]);
			}
		}
	}
	if (IS_CONNECTED) {
		if (define) {
			if (delete)
				indigo_delete_property(device, property->property, NULL);
			indigo_define_property(device, property->property, NULL);
		} else {
			indigo_update_property(device, property->property, NULL);
		}
	}
	return true;
}