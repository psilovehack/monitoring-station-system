package com.jlccaires.mss.server;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import com.jlccaires.mss.client.MSSService;
import com.jlccaires.mss.server.serial.SerialComm;

public class MSSServiceImpl extends RemoteServiceServlet implements MSSService {

	private static final long serialVersionUID = -111362649763187804L;
	
	SerialComm com = new SerialComm();

	public String sendCommand(String command) {

		return com.print(command);
	}

}
