////////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"
#include <Windows.h>
#include <chrono>
#include <bitset>

std::string server_ip = "192.168.137.79";
int server_port = 4001;

float timeout_start;


void send_command(CClient &client, std::string cmd)
{
	std::string str;

	client.tx_str(cmd);
	std::cout << "\nClient Tx: " << cmd;
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	if (cmd == "im")
	{
		cv::Mat im;
		if (client.rx_im(im) == true)
		{
			timeout_start = cv::getTickCount();
			if (im.empty() == false)
			{
				std::cout << "\nClient Rx: Image received";
				cv::imshow("rx", im);
				cv::waitKey(10);
			}
		}
		else
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				// No response, disconnect and reconnect
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}
	else
	{
		if (client.rx_str(str) == true)
		{
			timeout_start = cv::getTickCount();
			std::cout << "\nClient Rx: " << str;
		}
		else
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				// No response, disconnect and reconnect
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	CClient client;
	int cmd = -1;

	timeout_start = cv::getTickCount();
	client.connect_socket(server_ip, server_port);

	std::string rx_ack;
	std::bitset <9> keystate;
	do
	{
		keystate.reset();
		
		send_command(client, "im");
		if (GetKeyState('W') & 0x8000)
		{
			keystate[0] = 1;
		}
		if (GetKeyState('A') & 0x8000)
		{
			keystate[1] = 1;
		}
		if (GetKeyState('S') & 0x8000)
		{
			keystate[2] = 1;
		}
		if (GetKeyState('D') & 0x8000)
		{
			keystate[3] = 1;
		}
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			keystate[4] = 1;
		}
		if (GetKeyState(VK_UP) & 0x8000)
		{
			keystate[5] = 1;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			keystate[6] = 1;
		}
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			keystate[7] = 1;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			keystate[8] = 1;
		}

		send_command(client, keystate.to_string());
	} while (!(GetKeyState('Q') & 0x8000));
}
