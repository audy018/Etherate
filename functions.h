/*
 * License: MIT
 *
 * Copyright (c) 2012-2018 James Bensley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 * File: Etherate Gerneral Functions
 *
 */



// Send gratuitus Etherate broadcasts
int16_t broadcast_etherate(struct etherate *eth);

// Build the Ethernet headers for sending frames
void build_headers(struct frm *frm);

// Inline these functions as they are called for every packet that is sent
#if !defined (NOINLINE)
// Build the Etherate TLV header
inline void build_tlv(struct frm *frm, uint16_t TLV_TYPE,
                      uint32_t TLV_VALUE)
{
    uint8_t* buffer_offset = frm->tx_data;

    *((uint16_t *) buffer_offset) = TLV_TYPE;
    buffer_offset += sizeof(TLV_TYPE);
    *buffer_offset++ = sizeof(TLV_VALUE);
    *((uint32_t *) buffer_offset) = TLV_VALUE;

    frm->rx_tlv_type  = (uint16_t*) frm->rx_data;
    frm->rx_tlv_value = (uint32_t*) (frm->rx_data + sizeof(TLV_TYPE) +
                                     sizeof(uint8_t));
}

// Build a sub-TLV value after the TLV header
inline void build_sub_tlv(struct frm *frm,
                          uint16_t SUB_TLV_TYPE, uint64_t SUB_TLV_VALUE)
{

    uint8_t *buffer_offset = frm->tx_data + frm->tlv_size;

    *((uint16_t *) buffer_offset) = SUB_TLV_TYPE;
    buffer_offset += sizeof(SUB_TLV_TYPE);
    *buffer_offset++ = sizeof(SUB_TLV_VALUE);
    *((uint64_t *) buffer_offset) = SUB_TLV_VALUE;

    frm->sub_tlv_val = (uint64_t*)buffer_offset;

    frm->rx_sub_tlv_type  = (uint16_t*) (frm->rx_data + frm->tlv_size);
    frm->rx_sub_tlv_value = (uint64_t*) (frm->rx_data + frm->tlv_size +
                                         sizeof(SUB_TLV_TYPE) +
                                         sizeof(uint8_t));
}

#else

// Build the Etherate TLV header
void build_tlv(frm_t *frm, uint16_t tlv_type, uint32_t tlv_value);

// Build a sub-TLV value after the TLV header
void build_sub_tlv(frm_t *frm, uint16_t sub_tlv_type, uint64_t sub_tlv_value);

#endif

// Process the CLI arguments
int16_t cli_args(int argc, char *argv[], struct etherate *eth);

// Explode a char string based on the passed field delimiter
uint8_t explode_char(char *string, char *delim, char *tokens[]);

// Get the MTU of the interface used for the test
int32_t get_interface_mtu_by_name(struct intf *intf);

// Try to automatically chose an interface to run the test on
int16_t get_sock_interface(struct intf *intf);

// List interfaces and hardware (MAC) address
void list_interfaces();

// Print CLI args and usage
void print_usage(struct etherate *eth);

// Remove the interface from promiscuous mode
int16_t remove_interface_promisc(struct intf *intf);

// Free's allocated memory and closes sockets etc
void reset_app(struct etherate *eth);

// Set the interface into promiscuous mode
int16_t set_int_promisc(struct intf *intf);

// Try to open the passed socket on a user specified interface by index
int16_t set_sock_interface_index(struct intf *intf);

// Try to open the passed socket on a user specified interface by name
int16_t set_sock_interface_name(struct intf *intf);

// Signal handler to notify remote host of local application termiantion
void signal_handler(int signal);

// Send the settings from TX to RX
void sync_settings(struct etherate *eth);

// Update the frame size value and check it against the PHY MTU
void update_frame_size(struct etherate *eth);