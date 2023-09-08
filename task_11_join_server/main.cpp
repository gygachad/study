#include <iostream>

#include "join_server.h"
#include "asio_server.h"

using namespace std;

int main(int argc, const char* argv[])
{
	/*
	string cmd_line;
	shared_ptr<ICmd_dispatcher> cmd_dispatched = make_shared<join_server>();

	while (getline(cin, cmd_line))
	{
		if (cmd_line == "exit")
			break;

		//execute
		string answer = cmd_dispatched->execute(cmd_line);

		cout << answer << endl;
	}
	*/
	if (argc < 2)
	{
		cout << "Usage: join_server port_num" << endl;
		return 0;
	}

	uint16_t port = atoi(argv[1]);
	shared_ptr<ICmd_dispatcher> cmd_dispatched = make_shared<join_server>();

	bulk_server srv(port, cmd_dispatched);

	srv.start();

	string cmd = "";

	while (getline(cin, cmd))
	{
		if (cmd == "stop")
			break;
	}

	srv.stop();

	return 0;
}
