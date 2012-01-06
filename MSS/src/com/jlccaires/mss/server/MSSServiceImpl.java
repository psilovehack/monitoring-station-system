package com.jlccaires.mss.server;

import java.util.ArrayList;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;
import com.jlccaires.mss.client.MSSService;
import com.jlccaires.mss.server.serial.SerialComm;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

public class MSSServiceImpl extends RemoteServiceServlet implements MSSService, ServletContextListener {

	private static final long serialVersionUID = -111362649763187804L;
	
	SerialComm com = new SerialComm();

	/**
	 * Sends a command to serial port
	 */
	public String sendCommand(String command) {

		return com.print(command);
	}
	
	/**
	 * List all comports on server
	 */
	public ArrayList<String> getPorts() {
		return com.listPorts();
	}
	
	/**
	 * Connects to a specific port
	 */
	public String connect(String portId) throws Exception{
		return com.connect(portId);
	}
	
	/**
	 * Closes actual connection
	 */
	public String close() {
		com.close();
		return "closed.";
		
	}
	
	public void contextDestroyed(ServletContextEvent arg0) {
		com.close();
		com = null;
	}
	public void contextInitialized(ServletContextEvent arg0) {
	}

}
