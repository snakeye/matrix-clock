char* commands[] = {
		"AT+ORGL\r\n",
		"AT+NAME=\"Matrix Clock\"\r\n",
		"AT+PSWD=1022\r\n",
		"AT+UART=38400,0,0\r\n",
		"AT+RESET\r\n",
		0
		};
		
	char response[64];

	_delay_ms(1000);

	for(uint8_t i = 0; commands[i] != 0; i++)
	{
		display_clear_canvas();
		display_draw_string(0, 0, commands[i]);
		display_update();

		_delay_ms(1000);

		usart_transmit_sync(commands[i]);
		
		usart_receive_sync(response, 64);

		display_clear_canvas();
		display_draw_string(0, 0, response);
		display_update();

		_delay_ms(1000);
	}