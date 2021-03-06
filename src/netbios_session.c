//---------------------------------------------------------------------------
//  __________________    _________  _____            _____  .__         ._.
//  \______   \______ \  /   _____/ /     \          /  _  \ |__| ____   | |
//   |    |  _/|    |  \ \_____  \ /  \ /  \        /  /_\  \|  _/ __ \  | |
//   |    |   \|    `   \/        /    Y    \      /    |    |  \  ___/   \|
//   |______  /_______  /_______  \____|__  / /\   \____|__  |__|\___ |   __
//          \/        \/        \/        \/  )/           \/        \/   \/
//
// This file is part of libdsm. Copyright © 2014 VideoLabs SAS
//
// Author: Julien 'Lta' BALLET <contact@lta.io>
//
// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See the COPYING file for more details.
//----------------------------------------------------------------------------

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "bdsm/netbios_session.h"
#include "bdsm/netbios_utils.h"

static int        open_socket_and_connect(netbios_session *s)
{
  if ((s->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    goto error;
  if (connect(s->socket, (struct sockaddr *)&s->remote_addr, sizeof(s->remote_addr)) <0)
    goto error;

  return (1);

  error:
    perror("netbios_session_new, open_socket: ");
    return (0);
}

netbios_session *netbios_session_new(uint32_t ip_addr)
{
  netbios_session *session;
  size_t            packet_size;

  session = (netbios_session *)malloc(sizeof(netbios_session));
  assert(session);
  memset((void *) session, 0, sizeof(netbios_session));

  session->packet_payload_size = NETBIOS_SESSION_PAYLOAD;
  packet_size = sizeof(netbios_session_packet) + session->packet_payload_size;
  session->packet = (netbios_session_packet *)malloc(packet_size);
  assert(session->packet);

  session->remote_addr.sin_family       = AF_INET;
  session->remote_addr.sin_port         = htons(NETBIOS_PORT_SESSION);
  session->remote_addr.sin_addr.s_addr  = ip_addr;
  if (!open_socket_and_connect(session))
  {
    netbios_session_destroy(session);
    return (NULL);
  }

  return(session);
}

void              netbios_session_destroy(netbios_session *s)
{
  if (!s)
    return;
  close(s->socket);
  if (s->packet)
    free(s->packet);
  free(s);
}

int               netbios_session_connect(netbios_session *s,
                                          const char *name)
{
  netbios_session_packet  *received;
  ssize_t                   recv_size;
  char                      *encoded_name;

  assert(s && s->packet && s->socket);

  // Send the Session Request message
  netbios_session_packet_init(s, NETBIOS_OP_SESSION_REQ);
  encoded_name = netbios_name_encode(name, 0, NETBIOS_FILESERVER);
  netbios_session_packet_append(s, encoded_name, strlen(encoded_name) + 1);
  free(encoded_name);
  encoded_name = netbios_name_encode("LIBDSM", 0, NETBIOS_WORKSTATION);
  netbios_session_packet_append(s, encoded_name, strlen(encoded_name) + 1);
  free(encoded_name);

  s->state = NETBIOS_SESSION_CONNECTING;
  if (!netbios_session_packet_send(s))
    goto error;

  // Now receiving the reply from the server.
  recv_size = netbios_session_packet_recv(s);
  if (recv_size < sizeof(netbios_session_packet))
    goto error;

  received = (netbios_session_packet *)&s->recv_buffer;
  // Reply was negative, we are not connected :(
  if (received->opcode != NETBIOS_OP_SESSION_REQ_OK)
  {
    s->state = NETBIOS_SESSION_REFUSED;
    return (0);
  }

  // Reply was OK, a netbios sessions has been established
  s->state = NETBIOS_SESSION_CONNECTED;
  return(1);

  error:
    s->state = NETBIOS_SESSION_ERROR;
    return (0);
}

void              netbios_session_packet_init(netbios_session *s,
                                              uint8_t opcode)
{
  size_t          packet_size;

  assert(s);

  packet_size = s->packet_payload_size + sizeof(netbios_session_packet);
  memset((void *)s->packet, 0, packet_size);

  s->packet_cursor = 0;
  s->packet->opcode = opcode;
}

int               netbios_session_packet_append(netbios_session *s,
                                                const char *data, size_t size)
{
  char  *start;

  assert(s && s->packet);

  if (s->packet_payload_size - s->packet_cursor < size)
    return (0);

  start = ((char *)&s->packet->payload) + s->packet_cursor;
  memcpy(start, data, size);
  s->packet_cursor += size;

  return (1);
}

int               netbios_session_packet_send(netbios_session *s)
{
  ssize_t         to_send;
  ssize_t         sent;

  assert(s && s->packet && s->socket && s->state > 0);

  s->packet->length = htons(s->packet_cursor);
  to_send           = sizeof(netbios_session_packet) + s->packet_cursor;
  sent              = send(s->socket, (void *)s->packet, to_send, 0);

  if (sent != to_send)
  {
    perror("netbios_session_packet_send: Unable to send (full?) packet");
    return (0);
  }

  return (sent);
}

ssize_t           netbios_session_packet_recv(netbios_session *s)
{
  assert(s && s->socket && s->state > 0);

  return (recv(s->socket, (void *)&(s->recv_buffer), NETBIOS_SESSION_BUFFER, 0));
}

