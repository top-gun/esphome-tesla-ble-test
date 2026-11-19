        ESP_LOGD(tag, "    frontTrunk: %s", closure_state_to_string(msg->closureStatuses.frontTrunk));
        ESP_LOGD(tag, "    chargePort: %s", closure_state_to_string(msg->closureStatuses.chargePort));
    }
    ESP_LOGD(tag, "  vehicleLockState: %s", vehicle_lock_state_to_string(msg->vehicleLockState));
    ESP_LOGD(tag, "  vehicleSleepStatus: %s", vehicle_sleep_status_to_string(msg->vehicleSleepStatus));
    ESP_LOGD(tag, "  userPresence: %s", user_presence_to_string(msg->userPresence));
}

void log_vssec_signed_message_status(const char *tag, const VCSEC_SignedMessage_status *status)
{
    ESP_LOGI(tag, "  SignedMessage status:");
    ESP_LOGI(tag, "    counter: %" PRIu32, status->counter);
    ESP_LOGI(tag, "    signed_message_information: %s", vssec_signed_message_information_to_string(status->signedMessageInformation));
}

void log_vssec_whitelist_operation_status(const char *tag, const VCSEC_WhitelistOperation_status *status)
{
    char hex_buf[2 * sizeof(status->signerOfOperation.publicKeySHA1.bytes) + 1];
    ESP_LOGI(tag, "  WhitelistOperation status:");
    // has_signerOfOperation;
    if (status->has_signerOfOperation)
    {
        ESP_LOGD(tag, "    signerOfOperation:");
        ESP_LOGD(tag, "      public_key: %s", format_hex_to(hex_buf, status->signerOfOperation.publicKeySHA1.bytes, status->signerOfOperation.publicKeySHA1.size));
    }
    ESP_LOGI(tag, "    operation_status: %s", vcsec_operation_status_to_string(status->operationStatus));
    ESP_LOGI(tag, "    information: %s", vssec_whitelist_operation_information_to_string(status->whitelistOperationInformation));
}

void log_vcsec_command_status(const char *tag, const VCSEC_CommandStatus *msg)
{
    ESP_LOGD(tag, "VCSEC_CommandStatus:");
    ESP_LOGD(tag, "  commandStatus: %s", vcsec_operation_status_to_string(msg->operationStatus));

    ESP_LOGD(tag, "  which_sub_message: %d", msg->which_sub_message);
    switch (msg->which_sub_message)
    {
    case VCSEC_CommandStatus_signedMessageStatus_tag:
        log_vssec_signed_message_status(tag, &msg->sub_message.signedMessageStatus);
        break;
    case VCSEC_CommandStatus_whitelistOperationStatus_tag:
        log_vssec_whitelist_operation_status(tag, &msg->sub_message.whitelistOperationStatus);
        break;
    default:
        ESP_LOGD(tag, "  unknown sub_message");
    }
}

void carserver_result_reason_to_string(const char *tag, const CarServer_ResultReason *reason)
{
    ESP_LOGI(tag, "  ResultReason:");
    ESP_LOGI(tag, "    which_reason: %d", reason->which_reason);
    switch (reason->which_reason)
    {
    case CarServer_ResultReason_plain_text_tag:
        ESP_LOGI(tag, "    plain_text: %s", reason->reason.plain_text);
        break;
    default:
        ESP_LOGD(tag, "    unknown reason");
    }
}

const char *carserver_operation_status_to_string(CarServer_OperationStatus_E status)
{
    switch (status)
    {
    case CarServer_OperationStatus_E_OPERATIONSTATUS_OK:
        return "OK";
    case CarServer_OperationStatus_E_OPERATIONSTATUS_ERROR:
        return "ERROR";
    default:
        return "UNKNOWN_STATUS";
    }
}

void log_carserver_response(const char *tag, const CarServer_Response *msg)
{
    ESP_LOGD(tag, "CarServerResponse:");
    if (msg->has_actionStatus)
    {
        ESP_LOGD(tag, "  ActionStatus:");
        ESP_LOGD(tag, "    result: %s", carserver_operation_status_to_string(msg->actionStatus.result));
        if (msg->actionStatus.has_result_reason)
        {
            switch (msg->actionStatus.result_reason.which_reason)
            {
            case CarServer_ResultReason_plain_text_tag:
                ESP_LOGI(tag, "    reason: %s", msg->actionStatus.result_reason.reason.plain_text);
                break;
            default:
                ESP_LOGI(tag, "    unknown reason");
            }
        }
    }
    ESP_LOGD (tag, "  vehicleData:");
    log_vehicle_data (tag, &msg->response_msg.vehicleData);
    ESP_LOGD (tag, "    has_charge_state: %d", msg->response_msg.vehicleData.has_charge_state);
    ESP_LOGD (tag, "    charge state = %ld", msg->response_msg.vehicleData.charge_state.optional_battery_level.battery_level);
    ESP_LOGD (tag, "    has_climate_state: %d", msg->response_msg.vehicleData.has_climate_state);
    ESP_LOGD (tag, "    has_drive_state: %d", msg->response_msg.vehicleData.has_drive_state);
    ESP_LOGD (tag, "    has_location_state: %d", msg->response_msg.vehicleData.has_location_state);
    ESP_LOGD (tag, "    has_closures_state: %d", msg->response_msg.vehicleData.has_closures_state);
    ESP_LOGD (tag, "    has_charge_schedule_state: %d", msg->response_msg.vehicleData.has_charge_schedule_state);
    ESP_LOGD (tag, "    has_preconditioning_schedule_state: %d", msg->response_msg.vehicleData.has_preconditioning_schedule_state);
    ESP_LOGD (tag, "    has_tire_pressure_state: %d", msg->response_msg.vehicleData.has_tire_pressure_state);
    ESP_LOGD (tag, "    has_media_state: %d", msg->response_msg.vehicleData.has_media_state);
    ESP_LOGD (tag, "    has_media_detail_state: %d", msg->response_msg.vehicleData.has_media_detail_state);
    ESP_LOGD (tag, "    has_software_update_state: %d", msg->response_msg.vehicleData.has_software_update_state);
    ESP_LOGD (tag, "    has_parental_controls_state: %d", msg->response_msg.vehicleData.has_parental_controls_state);

    switch (msg->which_response_msg)
    {
    case CarServer_Response_vehicleData_tag:
        break;
    case CarServer_Response_getSessionInfoResponse_tag:
        ESP_LOGI(tag, "  getSessionInfoResponse:");
        log_session_info(tag, &msg->response_msg.getSessionInfoResponse);
        break;
    case CarServer_Response_getNearbyChargingSites_tag:
        ESP_LOGI(tag, "  getNearbyChargingSites:");
        break;
    case CarServer_Response_ping_tag:
        ESP_LOGI(tag, "  ping:");
        ESP_LOGI(tag, "    ping: %ld", msg->response_msg.ping.ping_id);
        break;
    default:
        // do nothing
        break;
