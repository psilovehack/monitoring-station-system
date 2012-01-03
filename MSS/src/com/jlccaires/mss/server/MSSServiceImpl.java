package com.jlccaires.mss.server;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import com.jlccaires.mss.client.MSSService;
import com.jlccaires.mss.server.serial.SerialComm;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

public class MSSServiceImpl extends RemoteServiceServlet implements MSSService, ServletContextListener {

	private static final long serialVersionUID = -111362649763187804L;
	
	SerialComm com = new SerialComm();

	public String sendCommand(String command) {

		return com.print(command);
	}
	
	public void contextDestroyed(ServletContextEvent arg0) {
		com.close();
	}
	public void contextInitialized(ServletContextEvent arg0) {
		
	}

}
