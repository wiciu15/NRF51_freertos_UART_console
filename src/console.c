/* Copyright (c) 2007 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


/** \file
 *
 * Implementation of console.h.
 *
 */
#include "console.h"
#include "app_uart.h"
#include "nrf.h"
#include "app_error.h"

#define RX_PIN_NUMBER	1
#define TX_PIN_NUMBER	0
#define RTS_PIN_NUMBER	2
#define CTS_PIN_NUMBER	3

#define UART_TX_BUF_SIZE 1024                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 8                           /**< UART RX buffer size. */

static const uint8_t newline_input[] = CONSOLE_NEWLINE_INPUT; /*!< Needed to compare input against to find end of line */
#define NEWLINE_INPUT_LEN (sizeof CONSOLE_NEWLINE_INPUT - 1)  /*!< Subtract one for the zero termination */

static const char hex_tab[] = "0123456789ABCDEF"; /*!< Table of ASCII hexadecimal digits */

// static uint8_t m_IEN_status;  // not referenced

/** Console init
 */

static enum
{
    CONSOLE_UNINIT = 0,
    CONSOLE_AVAILABLE
} m_console = CONSOLE_UNINIT;

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        if(p_event->data.error_code==4){  //fifo overflow occured when console task was suspended or busy
        	app_uart_flush(); //discard any data in fifo
        }else{
        	APP_ERROR_HANDLER(p_event->data.error_code);
        }
    }
}

void console_init(void)
{
	uint32_t err_code;
	const app_uart_comm_params_t comm_params =
	{
			RX_PIN_NUMBER,
			TX_PIN_NUMBER,
			RTS_PIN_NUMBER,
			CTS_PIN_NUMBER,
			APP_UART_FLOW_CONTROL_DISABLED,
			false,
			UART_BAUDRATE_BAUDRATE_Baud38400
	};

	APP_UART_FIFO_INIT(&comm_params,
			UART_RX_BUF_SIZE,
			UART_TX_BUF_SIZE,
			uart_error_handle,
			APP_IRQ_PRIORITY_LOW,
			err_code);
	APP_ERROR_CHECK(err_code);
	m_console = CONSOLE_AVAILABLE;
}

void console_clear_screen(void)
{
	if(m_console == CONSOLE_AVAILABLE){
		console_put_char(12);
	}
}

void console_flush_input_buffer(void){
	app_uart_flush();
}

bool console_available(void)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void console_put_string(uint8_t const * string)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        while (*string != 0)
        {
            app_uart_put(*string++);
        }
    }
}


void console_put_line(uint8_t const * string)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        while (*string != 0)
        {
        	app_uart_put(*string++);
        }
        console_put_string((uint8_t *)CONSOLE_NEWLINE_OUTPUT);
    }
}


void console_put_newline(void)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        console_put_string((uint8_t *)CONSOLE_NEWLINE_OUTPUT);
    }
}


void console_put_chars(uint8_t const * chars, uint8_t num_chars)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        for ( ; num_chars > 0; num_chars--)
        {
        	app_uart_put(*chars++);
        }
    }
}


void console_get_string(uint8_t * string, uint8_t num_chars)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        for ( ; num_chars > 0; num_chars--)
        {
            app_uart_get(&*string++);
  #ifdef CONSOLE_ENABLE_ECHO
            app_uart_put(*(string-1));
  #endif
        }
        *string = 0; /* Add zero terminator */
    }
}

void console_get_line(uint8_t * string, uint8_t max_len)
{
    uint8_t c, k, m;

    if ( m_console == CONSOLE_AVAILABLE )
    {
        c = '\0';
        for ( k = 0; k < max_len - 1 ; k++ )
        {
            app_uart_get(&c);
            if (c == newline_input[0])
            {
                break;
            }
            string[k] = c;
  #ifdef CONSOLE_ENABLE_ECHO
            app_uart_put(c);
  #endif
        }
        string[k] = 0;
        /* Read (and discard) also rest of newline sequence if we found the start of if */
        /* This is were we may discard characters we should not, se the comments in the header file. */
        /* NOTE: We really should check what we read, and notify the caller if it is not really the newline sequence. */
        if ( c == newline_input[0] )
        {
            for( m = 0; m < NEWLINE_INPUT_LEN - 1; m++)  /* We have already read the first character */
            {
                app_uart_get(&c);
            }
  #ifdef CONSOLE_ENABLE_ECHO
            /* We have read a newline, and since echo is enabled, we should also echo back a newline. */
            /* But this should be the output newline, which may differ from the input newline we read. */
            console_put_string((uint8_t *)CONSOLE_NEWLINE_OUTPUT    
  #endif
        }
    }
} //lint !e438 Last value assigned to 'c' not used

uint32_t console_chars_available(void)
{
	return app_uart_bytes_in_rx_fifo();
}

void console_get_chars(uint8_t * chars, uint8_t num_chars)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        for ( ; num_chars > 0; num_chars--)
        {
            app_uart_get(&*chars++);
  #ifdef CONSOLE_ENABLE_ECHO
            app_uart_put(*(chars-1));
  #endif
        }
    }
}


void console_put_char(uint8_t ch)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
    	app_uart_put(ch);
    }
}


uint8_t console_get_char(void)
{
    uint8_t ch = '\0';

    if ( m_console == CONSOLE_AVAILABLE )
    {
        app_uart_get(&ch);

  #ifdef CONSOLE_ENABLE_ECHO
        app_uart_put(ch);
  #endif
    }
    return ch;
}

void console_put_decbyte(uint8_t b) // b is in the range [0 255]
{
    uint8_t b0;
    uint8_t b1;

    if ( m_console == CONSOLE_AVAILABLE )
    {
        b0 = (b % 10); // Remainder of b when divided by 10
        b /= 10;       // forces w into the range [0 25]

        b1 = (b % 10); // Remainder of b when divided by 10
        b /= 10;       // forces w into the range [0 2]

        if (b != 0)
        {
        	app_uart_put(b + '0');
        	app_uart_put(b1 + '0');
        	app_uart_put(b0 + '0');
        }
        else if (b1 != 0)
        {
        	app_uart_put(b1 + '0');
        	app_uart_put(b0 + '0');
        }
        else
        {
        	app_uart_put(b0 + '0');
        }
    }
}

void console_put_decword(uint16_t w)  // w is in the range [0 65535]
{
    uint8_t w0;
    uint8_t w1;
    uint8_t w2;
    uint8_t w3;

    if ( m_console == CONSOLE_AVAILABLE )
    {
        w0 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 6553]

        w1 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 655]

        w2 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 65]

        w3 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 6]

        if (w != 0)
        {
            app_uart_put((uint8_t)w + '0');  /* We may safely cast w to the smaller type, as we have */
                                                 /* made sure (above) that its value will fit. */
            app_uart_put(w3 + '0');
            app_uart_put(w2 + '0');
            app_uart_put(w1 + '0');
            app_uart_put(w0 + '0');
        }
        else if (w3 != 0)
        {
            app_uart_put(w3 + '0');
            app_uart_put(w2 + '0');
            app_uart_put(w1 + '0');
            app_uart_put(w0 + '0');
        }
        else if (w2 != 0)
        {
            app_uart_put(w2 + '0');
            app_uart_put(w1 + '0');
            app_uart_put(w0 + '0');
        }
        else if (w1 != 0)
        {
            app_uart_put(w1 + '0');
            app_uart_put(w0 + '0');
        }
        else
        {
            app_uart_put(w0 + '0');
        }
    }
}


void console_put_dec32bit(uint32_t ww)  // ww is in the range [0 4294967295]
{
    uint8_t ww0;
    uint8_t ww1;
    uint8_t ww2;
    uint8_t ww3;
    uint8_t ww4;
    uint8_t ww5;
    uint8_t ww6;
    uint8_t ww7;
    uint8_t ww8;

    if ( m_console == CONSOLE_AVAILABLE )
    {
        ww0 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429496729]

        ww1 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42949672]

        ww2 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4294967]

        ww3 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429496]

        ww4 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42949]

        ww5 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4294]

        ww6 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429]

        ww7 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42]

        ww8 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4]


        if (ww != 0)
        {
            app_uart_put((uint8_t)ww + '0');  /* We may safely cast ww to the smaller type, as we have */
                                                  /* made sure (above) that its value will fit. */
            app_uart_put(ww8 + '0');
            app_uart_put(ww7 + '0');
            app_uart_put(ww6 + '0');
            app_uart_put(ww5 + '0');
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww8 != 0)
        {
            app_uart_put(ww8 + '0');
            app_uart_put(ww7 + '0');
            app_uart_put(ww6 + '0');
            app_uart_put(ww5 + '0');
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww7 != 0)
        {
            app_uart_put(ww7 + '0');
            app_uart_put(ww6 + '0');
            app_uart_put(ww5 + '0');
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww6 != 0)
        {
            app_uart_put(ww6 + '0');
            app_uart_put(ww5 + '0');
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww5 != 0)
        {
            app_uart_put(ww5 + '0');
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww4 != 0)
        {
            app_uart_put(ww4 + '0');
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww3 != 0)
        {
            app_uart_put(ww3 + '0');
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww2 != 0)
        {
            app_uart_put(ww2 + '0');
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else if (ww1 != 0)
        {
            app_uart_put(ww1 + '0');
            app_uart_put(ww0 + '0');
        }
        else
        {
            app_uart_put(ww0 + '0');
        }
    }
}


void console_put_hexnybble(uint8_t n)
{
    if ( m_console == CONSOLE_AVAILABLE )
    {
        app_uart_put((uint8_t)hex_tab[n & 0x0f]);
    }
}


void console_put_hexbyte(uint8_t b)
{
    console_put_hexnybble(b >> 4);
    console_put_hexnybble(b & 0x0f);
}


void console_put_hexword(uint16_t w)
{
    console_put_hexbyte((uint8_t)(w >> 8));
    console_put_hexbyte(w & 0xff);
}

void console_put_hexbytearray(uint8_t * p, uint8_t n)
{
    uint8_t i;
    if (n > 0)
    {
        console_put_hexbyte(p[0]);
    }
    for (i = 1; i < n; ++i)
    {
        console_put_string((uint8_t *)"-");
        console_put_hexbyte(p[i]);
    }
}

uint8_t console_get_hexnybble(void)
{
    uint8_t c;
    if ( m_console == CONSOLE_AVAILABLE )
    {
        app_uart_get(&c);}
  #ifdef CONSOLE_ENABLE_ECHO
        app_uart_put(c);
  #endif

        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            return (c - 'a') + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            return (c - 'A') + 10;
        }
        else
        {
 /** @note We have encountered something that is not a hexadecimal digit.
  *  This is an error, and it is not reported.  An ASSERT(false) would be suitable here. */
            return 0;
    }

    return 0;  // @note This may not be the correct value, what should be returned if m_console != CONSOLE_AVAILABLE ?
}


uint8_t console_get_hexbyte(void)
{
    uint8_t nh = console_get_hexnybble();
    return (uint8_t)((nh << 4) | console_get_hexnybble());
}


uint16_t console_get_hexword(void)
{
    uint16_t bh = console_get_hexbyte();
    return (bh << 8) | console_get_hexbyte();
}
/*
bool console_tx_completed(void)
{
    return hal_uart_tx_buf_empty();
}
*/


