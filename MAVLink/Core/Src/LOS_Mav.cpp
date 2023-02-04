/*
 * LOS_Mav.cpp
 *
 *  Created on: Feb 2, 2023
 *      Author: Stanley Tang
 */

#include "LOS_Mav.hpp"

Los_Mav& Los_Mav::getInstance(void)
{
	static Los_Mav singleton;
	return singleton;
}

Los_Mav::Los_Mav()
{
	memset(&last_status, 0, sizeof(mavlink_status_t));
}

uint8_t Los_Mav::readMessage(mavlink_message_t &message)
{
	uint8_t byte = 0;
	uint8_t end_of_msg = 0;
	mavlink_status_t status = {};
	uint8_t success = 1;

	/* Parse incoming packet one byte at a time. */
	for (uint16_t i = 0; i < MAVLINK_MAX_PACKET_LEN; ++i) {
		HAL_UART_Receive(LOS_MAV_HUART_HANDLE, &byte, 1, 100);

		end_of_msg = mavlink_parse_char(MAVLINK_COMM_1, byte, &message, &status);

		if (last_status.packet_rx_drop_count != status.packet_rx_drop_count) {
			/* Packet dropped! */
			success = 0;
		}

		last_status = status;

		if (end_of_msg) {
			break;
		}
	}

	return success;
}

void Los_Mav::writeMessage(const mavlink_message_t &msg)
{
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

	HAL_UART_Transmit(LOS_MAV_HUART_HANDLE, buf, len, 1000);
}

void Los_Mav::sendHeartbeat()
{
	mavlink_heartbeat_t heartbeat = {};
	heartbeat.type = MAV_TYPE_GCS; /* We are representing ground (control) station. */
	heartbeat.autopilot = MAV_AUTOPILOT_INVALID;

	mavlink_message_t message = {};
	mavlink_msg_heartbeat_encode(system_id, component_id, &message, &heartbeat);

	writeMessage(message);
}

void Los_Mav::sendNavCommand(float x, float y, float z, float radius)
{
	mavlink_command_long_t command_long = {};
	command_long.target_system = plane_system_id;
	command_long.target_component = plane_component_id;
	command_long.command = MAV_CMD_NAV_WAYPOINT;
	command_long.confirmation = 1;
	command_long.param2 = radius;
	command_long.param5 = x;
	command_long.param6 = y;
	command_long.param7 = z;

	mavlink_message_t message = {};
	mavlink_msg_command_long_encode(system_id, component_id, &message, &command_long);

	writeMessage(message);
}

uint8_t Los_Mav::receiveMessage(MAVLink_Message_t& mavlink_message)
{
	mavlink_message_t message = {};
	uint8_t success = 0;

	if (readMessage(message)) {
		success = 1;
		mavlink_message.message_id = message.msgid;

		switch (message.msgid) {
			case MAVLINK_MSG_ID_HEARTBEAT:
				mavlink_msg_heartbeat_decode(&message, &(mavlink_message.heartbeat));
				break;
			case MAVLINK_MSG_ID_COMMAND_ACK:
				mavlink_msg_command_ack_decode(&message, &(mavlink_message.command_ack));
				break;
			default:
				success = 0;
		}
	}

	return success;
}
