//
//  network.h
//  PlayDate
//
//  Created by Dave Hayden on 11/21/24.
//  Copyright (c) 2024 Panic, Inc. All rights reserved.
//

#ifndef network_h
#define network_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ringbuffer.h"
#include "accesscontrol.h"

#define MAX_DATA_CHANNELS 4

#define SIMULATOR_CONNECT_TO_AP_DELAY 3

typedef void (NetCallback)(void* ud);

typedef enum
{
	NET_OK = 0,
	NET_NO_DEVICE = -1,
	NET_BUSY = -2,
	NET_WRITE_ERROR = -3,
	NET_WRITE_BUSY = -4,
	NET_WRITE_TIMEOUT = -5,
	NET_READ_ERROR = -6,
	NET_READ_BUSY = -7,
	NET_READ_TIMEOUT = -8,
	NET_READ_OVERFLOW = -9,
	NET_FRAME_ERROR = -10,
	NET_BAD_RESPONSE = -11,
	NET_ERROR_RESPONSE = -12,
	NET_RESET_TIMEOUT = -13,
	NET_BUFFER_TOO_SMALL = -14,
	NET_UNEXPECTED_RESPONSE = -15,
	NET_NOT_CONNECTED_TO_AP = -16,
	NET_NOT_IMPLEMENTED = -17,
	NET_CONNECT_TIMEOUT = -18,
	NET_CONNECTION_CLOSED = -19,
	NET_PERMISSION_DENIED = -20,
} PDNetErr;

char* netError(PDNetErr err);

// start up wifi, if it isn't already
PDNetErr pd_network_start(void);
void pd_network_stop(void);

// call one of the above on the network task. callback is called on the runloop
void pd_network_setEnabled(bool flag, void (*callback)(PDNetErr err));

/*
typedef enum
{
	kWifiOff,
	kWifiStarting,
	kWifiDisconnected,
	kWifiConnecting,
	kWifiConnected
} PDWifiStatus;

typedef void PDWifiStatusChangeCallback(PDWifiStatus status);
void pd_network_init(PDWifiStatusChangeCallback changeCallback);

PDWifiStatus pd_network_getStatus(void);
*/

static inline bool pd_network_is_subdomain(const char *root, const char *host)
{
	const size_t flen = strlen(host);
	const size_t rlen = strlen(root);

	if (rlen >= flen) return false;

	size_t offset = flen - rlen;
	return !strcmp(host + offset, root);
}

// scan

typedef enum {
	PDWifiSecurityUnknown,
	PDWifiSecurityNone,
	PDWifiSecurityWEP,
	PDWifiSecurityWPA,
	PDWifiSecurityWPA2,
	PDWifiSecurityWPA_WPA2,
	PDWifiSecurityWPA2Enterprise,
} PDWifiSecurity;

struct accesspoint
{
	const char* ssid;
	int rssi;
	int channel;
	PDWifiSecurity sectype;
};

PDNetErr pd_network_scan(int count, void (*scanCallback)(struct accesspoint* aps, void* ud), void* userdata);

// connect

typedef enum {
	kWifiNotConnected = 0,  //!< Not connected to an AP
	kWifiConnected,     //!< Device is actively connected to an AP
	kWifiNotAvailable,  //!< A connection has been attempted and no configured AP was available
} WifiStatus;

WifiStatus pd_network_getConnectionStatus(void);
const char *pd_network_getIPAddress(void);
const char *pd_network_getMACAddress(void);

typedef void PDWifiAPConnectionCallback(int connected);
int pd_network_isConnectedToAP(void);
int pd_network_isUnavailable(void);
PDNetErr pd_network_connectToAP(const char* ssid, const char* seckey, PDWifiAPConnectionCallback* callback);
PDNetErr pd_network_disconnectFromAP(void);

void pd_network_ntp_sync(void);

//! Open a connection to the specified host
//!
//! @returns int a non-negative connection descriptor on success, PDNetErr value on failure
int pd_network_open(accessType access, const char *host, int port, bool usessl, int timeout_ms, RingBuffer* buffer);

//! Close an open connection
//! @param connfd The connection descriptor to close
//!
//! @returns PDNetErr error result
PDNetErr pd_network_close(int connfd);

//! Send data over an open connection
//! @param connfd The connection descriptor to send data over
//! @param buffer Buffer of data to send
//! @param length The number of bytes to send
//!
//! @returns The number of bytes sent or PDNetErr on failure
int pd_network_send(int connfd, const void* buffer, size_t length);

//! inform network connection that there's free space in the buffer
void pd_network_resumeIfNeeded(int fd);

// nb these are called in the network task:
// called when the process loop or the readline function sees a <n>,CLOSED message
void pd_network_setClosedCallback(int fd, NetCallback* callback, void* ud);
// called when data is available to read on the channel
void pd_network_setDataAvailableCallback(int fd, NetCallback* callback, void* ud);

// called after 30 seconds of inactivity
void pd_network_inactivity_timeout(void);

//! Queue a callback to be executed on the network task
PDNetErr pd_network_runCommand(void (*func)(void* data), void* data);

// debug logging for network functions
void pd_network_setloglevel(int n);
void pd_network_debuglog(int level, char* fmt, ...);
#define DEBUGLOG1(...) pd_network_debuglog(1, __VA_ARGS__)
#define DEBUGLOG2(...) pd_network_debuglog(2, __VA_ARGS__)

#endif
